#include "BaseInputListCreator.h"

#include <QList>
#include <QMap>

#include <skymedia/SkyEffect.h>

#include "SEProject.h"

#ifndef FASHIONLINEINPUTLISTCREATOR_H
#define FASHIONLINEINPUTLISTCREATOR_H

const QString GLTF_GROUP = "GLTF";

class FashionlineInputListCreator : public BaseInputListCreator
{
Q_OBJECT
public:
    FashionlineInputListCreator(const QString& rootDir,const QString& tempOutputDir,skymedia::SkyTimeline* timeline,const QString& fashionlineCfg);

    bool createInputListFile() override;

signals:
    void exportLogInfo(int type, QString log);
private:
    void processTemplateType();

    QMap<QString,QList<SkyEffect*>*> extractFashionLineGroupData();

    void exportPreviewConf(QString previewCsv,const QMap<QString,QList<SkyEffect*>*>& map);

    QString saveSnapFrame(double time,QString path);

    void exportInputConf(QString printingcvs,
                         QString podxls,
                         QString colorcvs,
                         const QMap<QString, QList<SkyEffect *> *> &map);

    QJsonObject generateInputColorConf(QString colorcvs,
                         const QMap<QString, QList<SkyEffect *> *> &map, QMap<QString,QString>* colorList);

    void exportOutputConf(QString outputCsv, const QMap<QString,QList<SkyEffect*>*>& map);

    void tryPrepareExportCvsData(const QMap<QString,QList<SkyEffect*>*>& map);

    void appendCvsData(QString fromPath, QString toPath, QString title,QString data);

    SkyClip* findOwerClip(SkyTimeline* timeline,SkyEffect * effect);

    void uploadImageFilePath(QMap<QString,QString>* fileUrls);
    QJsonValue replaceJsonObjectPath(QJsonValue json,QMap<QString,QString>* fileUrls);

    void performUploadImageFilePath(QMap<QString,QString>* fileUrls);


private:
    QString m_fashionlineCfgDir = "";

};

#endif // FASHIONLINEINPUTLISTCREATOR_H
