#include "LocalResourceModel.h"

#include <base/utils/AppConstance.h>
#include <skymedia/SkyResource.h>
#include <src/base/utils/AppPath.h>
#include <src/project/SEProject.h>

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

SKYMEDIA_USING_NAMESPACE

LocalResourceModel::LocalResourceModel() {
}

bool availableFileSuffix(QList<QString> suffix_list, QString suffix) {
    for (int i = 0; i < suffix_list.size(); i++) {
        if (suffix_list.at(i).toUpper() == suffix.toUpper()) {
            return true;
        }
    }
    return false;
}

LocalResourceModel::LocalResourceModel(QString filePath) {
    QFile file(filePath);
    QFileInfo fileInfo(filePath);

    m_fileType = fileInfo.suffix().toUpper();
    m_fileName = fileInfo.fileName();
    m_fileCover = "qrc:/image/assets/res_bg_placeholder.png";
    m_filePath = filePath;
    m_videoDuration = "00:00";
    m_audioDuration = "00:00";
    m_audioAnchor = "Unknow";
    if (FileUtils::isImageExtension(fileInfo.fileName())) {
        m_fileCover = "image://skyimage/" + filePath + "?type=2&time=0.1&width=100&height=100";
    } else if (FileUtils::isVideoExtension(fileInfo.fileName())) {
        SkyResource res(filePath.toUtf8().data());
        qDebug() << "m_fileType : m_videoDuration???__" << res.getVideoDuration();
        m_videoDuration = QDateTime::fromTime_t(res.getVideoDuration()).toUTC().toString("mm:ss");
        //        qDebug() << "m_fileType : m_videoDuration" << m_videoDuration;
        //        SkyVideoDecoder m_decoder = SEProject::current()->dom()->timeline()->openVideoDecoder(res,
        //        SkyUseEnv_NoGL); SkyVideoFrame frame = m_decoder.retrieveVideo(0.1, SkyRetrieveMode_Accurate, true);
        //        if (!frame.isGPUTexture()) {
        //            QImage image(frame.getRGBA8Data(), frame.getWidth(), frame.getHeight(), QImage::Format_RGBA8888);
        //            QString imagePath = AppPath::tempPath() + "video_" +
        //            QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".jpg"; image.save(imagePath, "JPG");
        //            m_fileCover = imagePath;
        //            qDebug() << "m_fileType : m_fileCover" << m_fileCover;
        //        }
        m_fileCover = "image://skyimage/" + filePath + "?type=1&time=0&width=100&height=100";
    } else if (FileUtils::isAudioExtension(fileInfo.fileName())) {
        qDebug() << "m_fileType : m_videoDuration???";
        SkyResource res(filePath.toUtf8().data());
        qDebug() << "m_fileType : m_videoDuration???__" << res.getAudioDuration();
        m_audioDuration = QDateTime::fromTime_t(res.getAudioDuration()).toUTC().toString("mm:ss");
        qDebug() << "m_fileType : m_videoDuration get";
        qDebug() << "m_fileType : m_videoDuration" << m_audioDuration;
        //        m_fileCover = "image://skyimage/" + filePath + "?type=1&time=0&width=100&height=100";
    }
}

LocalResourceModel::LocalResourceModel(QDir folderPath, QString ofFile, QString title) {
    folderPath.setFilter(QDir::Files);
    QFileInfoList list = folderPath.entryInfoList();
    QString skyFile;
    QString coverFile = "qrc:/image/assets/icon_default_effect.png";
    for (int i = 0; i < list.count(); i++) {
        QFileInfo fileInfo = list.at(i);
        if (ofFile == nullptr && fileInfo.suffix() == "ofeffect") {
            ofFile = fileInfo.absoluteFilePath();
        } else if (fileInfo.suffix() == "sky" || fileInfo.suffix() == "proj") {
            skyFile = fileInfo.absoluteFilePath();
        } else if (fileInfo.fileName() == "thumb.png" || fileInfo.fileName() == "thumb.jpg" ||
                   fileInfo.fileName() == "cover.png" || fileInfo.fileName() == "cover.jpg") {
            coverFile = "file:/" + fileInfo.absoluteFilePath();
        }
    }
    if (title == nullptr) {
        m_fileName = folderPath.dirName();
    } else {
        m_fileName = title;
    }
    m_fileType = "MORE";
    m_fileCover = coverFile;
    m_filePath = ofFile;
    if (!QFileInfo::exists(skyFile)) {
        skyFile = folderPath.path() + "/template.proj";
        QFile configFile(skyFile);
        if (configFile.open(QIODevice::WriteOnly)) {
            QJsonObject configObject;
            QUuid IID_MyInterface(0x67c8770b, 0x44f1, 0x410a, 0xab, 0x9a, 0xf9, 0xb5, 0x44, 0x6f, 0x13, 0xee);
            configObject.insert("id", IID_MyInterface.toString());
            configObject.insert("title", m_fileName);
            configObject.insert("remoteId", "");
            configObject.insert("version", AppConstance::appVer());
            configObject.insert("remoteId", "");
            configObject.insert("anchor", "unknow");
            configObject.insert("summary", "");
            configObject.insert("type", "Effect");
            if (QFile::exists(coverFile)) {
                configObject.insert("thumb", "bundle://" + QFileInfo(coverFile).fileName());
            } else {
                configObject.insert("thumb", "");
            }
            configObject.insert("skyFile", "");
            if (QFile::exists(ofFile)) {
                configObject.insert("ofFile", "bundle://" + QFileInfo(ofFile).fileName());
            } else {
                configObject.insert("ofFile", "");
            }
            configObject.insert("resFile", "");
            configObject.insert("inputConfig", "");
            QString time = QString::number(QDateTime::currentDateTime().toTime_t());
            configObject.insert("createTimestamp", time);
            configObject.insert("lastChangedTimestamp", time);
            configObject.insert("isDebug", "0");
            QJsonDocument document;
            document.setObject(configObject);
            configFile.write(document.toJson());
            configFile.flush();
            configFile.close();
        }
    }
}

LocalResourceModel::LocalResourceModel(QJsonObject jo) {
    m_fileType = jo.value("fileType").toString();
    m_fileName = jo.value("fileName").toString();
    m_fileCover = jo.value("fileCover").toString();
    m_filePath = jo.value("filePath").toString();
    m_videoDuration = jo.value("videoDuration").toString();
    m_audioDuration = jo.value("audioDuration").toString();
    m_audioAnchor = jo.value("audioAnchor").toString();
}

QJsonObject LocalResourceModel::toJsonObject() {
    QJsonObject jo;
    jo["fileType"] = m_fileType;
    jo["fileName"] = m_fileName;
    jo["fileCover"] = m_fileCover;
    jo["filePath"] = m_filePath;
    jo["videoDuration"] = m_videoDuration;
    jo["audioDuration"] = m_audioDuration;
    jo["audioAnchor"] = m_audioAnchor;
    return jo;
}

// property
QString LocalResourceModel::fileType() const {
    return m_fileType;
}
QString LocalResourceModel::fileName() const {
    return m_fileName;
}
QString LocalResourceModel::fileCover() const {
    return m_fileCover;
}
QString LocalResourceModel::filePath() const {
    return m_filePath;
}
QString LocalResourceModel::videoDuration() const {
    return m_videoDuration;
}
QString LocalResourceModel::audioAnchor() const {
    return m_audioAnchor;
}
QString LocalResourceModel::audioDuration() const {
    return m_audioDuration;
}

LocalResourceListModel::LocalResourceListModel(QObject *parent) : QAbstractListModel(parent) {
}

LocalResourceListModel::~LocalResourceListModel() {
    datas.clear();
}

LocalResourceListModel::LocalResourceListModel(LocalResourceListModel &model) : QAbstractListModel(model.parent()) {
    datas.append(model.datas);
}

void LocalResourceListModel::insertData(int index, const LocalResourceModel &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    datas.insert(index, data);
    endInsertRows();
}

void LocalResourceListModel::addData(const LocalResourceModel &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    datas << data;
    endInsertRows();
}

bool LocalResourceListModel::isExistFile(QString path){
    for(auto data:datas){
        if(data.filePath() == path){
            return true;
        }
    }
    return false;
}

void LocalResourceListModel::deleteData(const int index) {
    if (index >= datas.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    datas.removeAt(index);
    endRemoveRows();
    qDebug() << "deleteData Index:" << index << " Size:" << datas.size();
}

int LocalResourceListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return datas.size();
}

QVariant LocalResourceListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= datas.count())
        return QVariant();

    const LocalResourceModel &data = datas[index.row()];
    if (role == FileCoverRole)
        return data.fileCover();
    else if (role == FilePathRole)
        return data.filePath();
    else if (role == FileNameRole)
        return data.fileName();
    else if (role == FileTypeRole)
        return data.fileType();
    else if (role == VideoDurationRole)
        return data.videoDuration();
    else if (role == AudioAnchorRole)
        return data.audioAnchor();
    else if (role == AudioDurationRole)
        return data.audioDuration();
    else if (role == IsSelectedRole)
        return index.row() == m_selectedIndex;
    return QVariant();
}

QHash<int, QByteArray> LocalResourceListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FileCoverRole] = "fileCover";
    roles[FilePathRole] = "filePath";
    roles[FileNameRole] = "fileName";
    roles[FileTypeRole] = "fileType";
    roles[VideoDurationRole] = "videoDuration";
    roles[AudioAnchorRole] = "audioAnchor";
    roles[AudioDurationRole] = "audioDuration";
    roles[IsSelectedRole] = "isSelected";
    return roles;
}

LocalResourceModel LocalResourceListModel::dataWith(int index) {
    return datas[index];
}

int LocalResourceListModel::dataCount() {
    return datas.size();
}

void LocalResourceListModel::clearAll() {
    beginResetModel();
    datas.clear();
    endResetModel();
}

void LocalResourceListModel::setSelectedIndex(int i) {
    int preIndex = m_selectedIndex;
    m_selectedIndex = i;

    if (preIndex >= 0) {
        QModelIndex changedIndex = index(preIndex);
        QVector<int> vector;
        vector.append(IsSelectedRole);

        emit dataChanged(changedIndex, changedIndex, vector);
    }

    if (i >= 0) {
        QModelIndex changedIndex = index(i);
        QVector<int> vector;
        vector.append(IsSelectedRole);

        emit dataChanged(changedIndex, changedIndex, vector);
    }
}

int LocalResourceListModel::curSelectedIndex() const {
    return m_selectedIndex;
}
