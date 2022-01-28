#include "UploadEffectController.h"

#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include "base/utils/SharedPref.h"
#include "base/http/BaseRsp.h"
#include "base/http/SEHttpClient.h"
#include "base/utils/AppConstance.h"
#include "base/utils/AppPath.h"
#include "base/utils/UploadUtils.h"
#include "base/utils/threadpools.h"
#include "base/utils/unitconverutils.h"
#include "project/SEProject.h"
#include "user/Login.h"
#include <base/utils/ZipUtils.h>

UploadEffectController::UploadEffectController(QObject *parent):QObject(parent){

}

void UploadEffectController::setEffectPath(QString effectPath){
    QFileInfo effect(effectPath);
    m_effectDir = effect.absoluteDir().absolutePath();
    QString findProjFile = SEProjectProj::projFilePath(m_effectDir);
    m_effectProj = new SEProjectProj(findProjFile);
    if(m_effectProj->type() != Effect || m_effectProj->entryFile() != effectPath){
        m_effectProj->setFile("");
        m_effectProj->setType(Effect);
        m_effectProj->setFile(effect.fileName());
        m_effectProj->save(m_effectDir);
    }

    m_effectInfo = m_effectProj->getExtraData("MaterialInfo").toObject();
    m_curTeamId = m_effectProj->getExtraData("TeamId").toString();
    if(m_effectInfo.isEmpty()){
        m_effectInfo.insert("name",m_effectProj->title());
        m_effectInfo.insert("cover", "http://biu-cn.dwstatic.com/sky/20211221/effect_default_icon.png");
    }
    if(!m_effectProj->remoteId().isEmpty()){
        loadRemoteMaterialInfo(m_effectProj->remoteId());
    }
    updateUserTeams();
    loadMaterialCategory();
    emit effectInfoLoaded();
}

UploadEffectController::~UploadEffectController(){
    if(m_effectProj){
        delete m_effectProj;
        m_effectProj = nullptr;
    }
}

void UploadEffectController::uploadEffect(QString name,QString cnName,QString coverPath, int category,int team){
    QJsonObject params;
    //    m_effectInfo.insert("cover", m_coverUrl);
    //    m_effectInfo.insert("sourceUrl", m_packUrl);
    QJsonObject languageName;
    languageName.insert("zh", cnName);
    languageName.insert("en", name);
    m_effectInfo.insert("languageName", languageName);
    m_effectInfo.insert("name", cnName);

    m_effectInfo.insert("cover", coverPath);
    m_effectInfo.insert("type", category);

    QJsonObject extConf;
    m_effectInfo.insert("extConf", extConf);
    m_effectInfo.insert("teamId", m_userTeamIds[team]);
    qInfo() << "ExportDialogDataSource,contribute_request:" << params;
    m_effectProj->setExtraData("MaterialInfo",m_effectInfo);
    m_effectProj->realSave(m_effectDir);

    auto zipPath = AppPath::tempPath(m_effectProj->title()+"_upload.zip");
    if(QFileInfo::exists(zipPath)){
        QFile::remove(zipPath);
    }

    ZipUtils::compressFile(m_effectDir,zipPath);
    m_effectInfo.insert("sourceUrl",zipPath);
    performUploadMaterialInfo();
    emit effectInfoLoaded();

}

void UploadEffectController::performUploadMaterialInfo(){
    QString coverPath = m_effectInfo["cover"].toString();
    QUrl coverUrl(coverPath);
    if(coverUrl.isLocalFile() && QFileInfo::exists(coverUrl.toLocalFile())){
        UploadUtils::upload(
                    coverUrl.toLocalFile(),
                    [=](qint64 bytesSent, qint64) mutable {

        },
        [=](const bool &success, const QString &packUrl) {
            if(success){
                m_effectInfo.insert("cover", packUrl);
                performUploadMaterialInfo();
            }else{
                emit effectUploadResult(-1,tr("Upload cover failed!"));
            }
        });
        qDebug()<<"Upload coverPath file:"<<coverPath;

        return;
    }

    QString zipPath =  m_effectInfo["sourceUrl"].toString();

    if(QFileInfo::exists(zipPath)){
        UploadUtils::upload(
                    zipPath,
                    [=](qint64 bytesSent, qint64) mutable {

        },
        [=](const bool &success, const QString &packUrl) {
            if(success){
                qDebug()<<" Upload zip Success! "<<packUrl;
                m_effectInfo.insert("sourceUrl", packUrl);
                performUploadMaterialInfo();
            }else{
                emit effectUploadResult(-1,tr("Upload zip failed!"));
            }
        });
        qDebug()<<"Upload zip file:"<<zipPath;
        return;
    }

    qInfo() << "ExportDialogDataSource,performSaveMaterialInfo:" << m_effectInfo;
    QJsonObject materialObj;
    QString allowKeys[7] = {"idStr","cover","sourceUrl","teamId", "name", "languageName","type"};

    for(QString key:allowKeys){
        if(m_effectInfo.contains(key)){
            if(key == "idStr"){
                materialObj.insert("id",m_effectInfo[key]);
            }else{
                materialObj.insert(key,m_effectInfo[key]);
            }
        }
    }

    QString otp = QString::fromStdString(Login::instance()->getOtp(AppConstance::uid()));
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/material/saveOrUpdateMaterial?sign=" + otp)
            .addJsonParam(materialObj)
            .post([=](const QString &response) {
        BaseRsp rsp(response);
        qInfo() << "ExportDialogDataSource,contribute_response:" << response;
        if (rsp.isSuccess()) {
            auto materialInfo = rsp.data().toObject();
            if(m_effectProj){
                m_effectProj->setExtraData("MaterialInfo", materialInfo);
                QString remoteId = materialInfo.value("idStr").toString();
                m_effectProj->setRemoteId(remoteId);
                m_effectProj->realSave(m_effectDir);
                m_effectInfo = materialInfo;
                emit effectUploadResult(0,tr("Upload Effect Success!"));
            }
        }else{
            emit effectUploadResult(-1,rsp.message());
        }
    });
}

void UploadEffectController::loadRemoteMaterialInfo(QString id){
    QString otp = QString::fromStdString(Login::instance()->getOtp(AppConstance::uid()));
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/material/getMaterialById?sign=" + otp+"&id="+id)
            .get([=](const QString &response) {
        BaseRsp rsp(response);
        if (rsp.isSuccess()) {
            auto materialInfo = rsp.data().toObject();
            if(m_effectProj && !materialInfo.isEmpty()){
                m_effectProj->setExtraData("MaterialInfo", materialInfo);
                m_effectProj->realSave(m_effectDir);
                m_effectInfo = materialInfo;
                emit effectInfoLoaded();
            }
        }else{
            qWarning()<<"loadRemoteMaterialInfo Failed!"<<rsp.message();
        }
    },[=](const QString &err){
        qWarning()<<"loadRemoteMaterialInfo Error!"<<err;
    });
}

void UploadEffectController::loadUserTeams(){
    QString otp = QString::fromStdString(Login::instance()->getOtp(AppConstance::uid()));
    if (Login::instance()->isLogin() && !otp.isEmpty()) {
        SEHttpClient(AppConstance::serverDomain() + "/v1/sky/team/listUserTeamsInVms")
                .addParam("sign", otp)
                .get([=](const QString &response) {
            BaseRsp rsp(response);
            if (rsp.isSuccess()) {
                QJsonArray array = rsp.data().toArray();
                QString json = QJsonDocument(array).toJson();
                SharedPref::instance()->setString("export_template_team_data",json);
                updateUserTeams();
            }
        });
    }

}

int UploadEffectController::getEffectTeam(){
    for(int i=0;i<m_userTeamIds.length();++i){
        if(m_userTeamIds[i] == m_curTeamId){
            return i;
        }
    }
    return 0;
}

void UploadEffectController::updateUserTeams(){
    auto userTeams = SharedPref::instance()->readString("export_template_team_data");
    m_userTeams.clear();
    m_userTeams.append(tr("Public Effect library"));
    m_userTeamIds.clear();
    m_userTeamIds.append("0");

    QJsonArray array = QJsonDocument::fromJson(userTeams.toUtf8()).array();
    for (int i = 0; i < array.size(); i++) {
        QJsonValue item = array.at(i);
        QJsonValue teamJson = item.toObject().take("team");
        QJsonValue memberJson = item.toObject().take("member");
        QJsonObject teamObj = teamJson.toObject();
        QJsonObject memberObj = memberJson.toObject();
        int role = memberObj.take("role").toInt();
        if (role == 1 || role == 2) {
            m_userTeams.append(teamObj.take("name").toString());
            m_userTeamIds.append(teamObj.take("id").toString());
        }
    }

    emit effectTeamsChanged(m_userTeams);
    emit effectTeamChanged(getEffectTeam());
}

void UploadEffectController::cancel(){

}

void UploadEffectController::loadMaterialCategory(){
    m_categorys.clear();
    m_categorys.append(tr("Sticker"));
    m_categorys.append(tr("Effect"));
    m_categorys.append(tr("Text"));
    m_categorys.append(tr("Transition"));
    m_categorys.append(tr("Filter"));
    emit effectCategoryChanged(m_categorys);
}

