#include "StoreResourceModel.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QUrl>
#include <QTemporaryFile>
#include "src/base/http/SEHttpClient.h"
#include "src/base/utils/ZipUtils.h"
#include <QCoreApplication>
#include <QProcess>
#include <base/utils/JsonUtils.h>
#include <src/base/utils/AppPath.h>

StoreResourceModel::StoreResourceModel() {
}

StoreResourceModel::StoreResourceModel(const QJsonObject &jo) {
    m_id = jo.value("idStr").toString();
    m_name = jo.value("name").toString();
    m_cover = jo.value("cover").toString();
    m_preview = jo.value("preview").toString();
    if (m_preview == m_cover) {
        m_preview = "";
    }
    if (m_preview.length() == 0 && m_cover.contains(".webp")) {
        m_preview = m_cover;
    }
    m_sourceUrl = jo.value("sourceUrl").toString();
    m_cate = jo.value("cate").toString();
    m_subCate = jo.value("subCate").toString().split(",");
    m_tinyCate = jo.value("tinyCate").toString();
    m_required = jo.value("required").toString();
    m_ofVersion = jo.value("ofVersion").toString();
    m_sdkVersion = jo.value("sdkVersion").toString();
    m_createTime = jo.value("createTime").toString();
    m_updateTime = jo.value("updateTime").toString();
    if (jo.contains("user")) {
        m_userId = jo.value("user").toObject().value("uid").toString();
        m_userName = jo.value("user").toObject().value("username").toString();
    }
    if (jo.contains("extConf")) {
        m_duration = jo.value("extConf").toObject().value("duration").toString();
    }
}

QString StoreResourceModel::id() const {
    return m_id;
}
QString StoreResourceModel::name() const {
    return m_name;
}
QString StoreResourceModel::cover() const {
    return m_cover;
}
QString StoreResourceModel::preview() const {
    if (m_preview.length() > 0) {
        return m_preview;
    } else {
        return m_preview;
    }
}
QString StoreResourceModel::sourceUrl() const {
    return m_sourceUrl;
}
QString StoreResourceModel::cate() const {
    return m_cate;
}
QStringList StoreResourceModel::subCate() const {
    return m_subCate;
}
QString StoreResourceModel::tinyCate() const {
    return m_tinyCate;
}
QString StoreResourceModel::required() const {
    return m_required;
}
QString StoreResourceModel::ofVersion() const {
    return m_ofVersion;
}
QString StoreResourceModel::sdkVersion() const {
    return m_sdkVersion;
}
QString StoreResourceModel::createTime() const {
    return m_createTime;
}
QString StoreResourceModel::updateTime() const {
    return m_updateTime;
}
QString StoreResourceModel::userId() const {
    return m_userId;
}
QString StoreResourceModel::userName() const {
    return m_userName;
}
QString StoreResourceModel::duration() const {
    return m_duration;
}

bool StoreResourceModel::isVenusEffect() const {
    QString rootPath = getLocalDir();
    QString uiinfoPath = rootPath + "/uiinfo.conf";
    if (QFileInfo(uiinfoPath).exists()) {
        auto config = JsonUtils::jsonObjectFromFile(uiinfoPath).object();
        auto venusObj = config.value("venus");
        return venusObj.isArray() && venusObj.toArray().size() > 0;
    }
    return false;
}

bool StoreResourceModel::avaliable() const {
    QString rootPath = getLocalDir();
    QDir rootDir(rootPath);
    if (rootDir.exists()) {
        rootDir.setFilter(QDir::Files);
        QFileInfoList list = rootDir.entryInfoList();
        int i = 0;
        QFileInfo projFile;
        while (i < list.size()) {
            QFileInfo fileInfo = list.at(i);
            if (fileInfo.suffix() == "sky" || fileInfo.suffix() == "proj") {
                return true;
            }
            i++;
        }

        // remove dir file
        rootDir.removeRecursively();
    }

    return false;
}

QString StoreResourceModel::getLocalDir() const {
    if (m_sourceUrl == nullptr || m_sourceUrl.length() == 0) {
        return nullptr;
    }
    QUrl url(m_sourceUrl);
    return AppPath::materialPath(url.fileName().split(".", QString::SkipEmptyParts).at(0));
    ;
}

void prepareDownloadPath() {
}

void StoreResourceModel::download(std::function<void(const bool, const QString &)> complateHandler) {
    QString rootPath = getLocalDir();
    if (avaliable()) {
        complateHandler(true, rootPath);
        return;
    }
    QDir rootDir(rootPath);
    rootDir.mkpath(rootPath);
    QUrl url(m_sourceUrl);
    QString downloadTempPath = AppPath::materialPath(url.fileName());
    QFile *file = new QFile(downloadTempPath);
    if (file->exists()) {
        file->remove();
    }
    file->open(QIODevice::ReadWrite);
    SEHttpClient(m_sourceUrl)
        .download(
            [=](const QByteArray &data) {
                file->write(data);
            },
            [=] {
                file->flush();
                file->close();

                // unzip
                if (ZipUtils::decompressFromZip(downloadTempPath, rootPath)) {
                    file->deleteLater();
                }
                complateHandler(true, rootPath);
            });
}

StoreResourceListModel::StoreResourceListModel(QObject *parent) : QAbstractListModel(parent) {
}

StoreResourceListModel::StoreResourceListModel(StoreResourceListModel &model) : QAbstractListModel(model.parent()) {
    datas.append(model.datas);
}

void StoreResourceListModel::insertData(int index, const StoreResourceModel &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    datas.insert(index, data);
    endInsertRows();
}

void StoreResourceListModel::addData(const StoreResourceModel &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    datas << data;
    endInsertRows();
}

int StoreResourceListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return datas.size();
}

QVariant StoreResourceListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= datas.count())
        return QVariant();

    const StoreResourceModel &data = datas[index.row()];
    if (role == IdRole)
        return data.id();
    else if (role == UrlRole)
        return data.sourceUrl();
    else if (role == NameRole)
        return data.name();
    else if (role == CoverRole)
        return data.cover();
    else if (role == PreviewRole)
        return data.preview();
    else if (role == ExistRole)
        return data.avaliable();
    else if (role == DurationRole)
        return data.duration();
    else if (role == IsSelectedRole)
        return index.row() == m_selectedIndex;
    else if (role == CateRole)
        return data.cate();
    else if (role == IsVenusEffectRole) {
        return data.isVenusEffect();
    }
    return QVariant();
}

QHash<int, QByteArray> StoreResourceListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[UrlRole] = "url";
    roles[NameRole] = "name";
    roles[CoverRole] = "cover";
    roles[PreviewRole] = "preview";
    roles[ExistRole] = "exist";
    roles[DurationRole] = "duration";
    roles[IsSelectedRole] = "isSelected";
    roles[CateRole] = "cate";
    roles[IsVenusEffectRole] = "isVenusEffect";
    return roles;
}

StoreResourceModel StoreResourceListModel::dataWith(int index) {
    return datas[index];
}

int StoreResourceListModel::curSelectedIndex() const {
    return m_selectedIndex;
}

void StoreResourceListModel::setSelectedIndex(int i) {
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

void StoreResourceListModel::clearAll() {
    beginResetModel();
    datas.clear();
    endResetModel();
}
