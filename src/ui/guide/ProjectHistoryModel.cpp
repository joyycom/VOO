#include "ProjectHistoryModel.h"
#include "src/project/SEProject.h"
#include <QJsonObject>
#include <QDir>
#include "base/utils/AppPath.h"

ProjectHistoryModel::ProjectHistoryModel(const QString &localPath) {
    SEProject prj(localPath);
    m_name = prj.title();
    SEProjectProj *projInfo = prj.proj();
    m_cover = projInfo->thumb();
    SEProjectConfig *projConfig = prj.config();
    m_size = projConfig->size();
    m_duration = projConfig->duration();
    m_localpath = localPath;
}

ProjectHistoryModel::ProjectHistoryModel(const QJsonObject& jo) {
    m_name = jo.value("name").toString();
    m_cover = fixPath(jo.value("cover").toString());
    m_size = jo.value("size").toString();
    m_duration = jo.value("duration").toString();
    m_localpath = fixPath(jo.value("localPath").toString());
    QFileInfo localPathFile(m_localpath);
    if (localPathFile.isDir()) {
        m_localpath = SEProjectProj::findProjectFile(m_localpath);
    }
}

QString ProjectHistoryModel::fixPath(QString path) {
    QFile file(path);
    if (!file.exists()) {
        path = path.mid(path.indexOf("tmp_project"));
        QFile file2(AppPath::rootPath(path));
        if (file2.exists()) {
            return file2.fileName();
        }
    }
    return path;
}

QString ProjectHistoryModel::subPath(QString path) {
    return path.replace(AppPath::rootPath() + "/", "");
}

QJsonObject ProjectHistoryModel::toJsonObject() {
    QJsonObject jo;
    jo["name"] = m_name;
    jo["cover"] = subPath(m_cover);
    jo["size"] = m_size;
    jo["duration"] = m_duration;
    jo["localPath"] = subPath(m_localpath);
    return jo;
}

void ProjectHistoryModel::remove() {
    QFile::remove(m_localpath);
    QString exportFile = QFileInfo(AppPath::sharedPrefPath(), m_name + ".export").filePath();
    QString exportCrcFile = QFileInfo(AppPath::sharedPrefPath(), m_name + ".export.crc").filePath();
    QFile::remove(exportFile);
    QFile::remove(exportCrcFile);
}

QString ProjectHistoryModel::localpath() const {
    return m_localpath;
}
QString ProjectHistoryModel::name() const {
    return m_name;
}
QString ProjectHistoryModel::cover() const {
    return m_cover;
}
QString ProjectHistoryModel::size() const {
    return m_size;
}
QString ProjectHistoryModel::duration() const {
    return m_duration;
}

void ProjectHistoryModel::updateDir(QString dir) {
    QString name =  QFileInfo(m_localpath).fileName();
    m_localpath = QFileInfo(dir, QFileInfo(m_localpath).fileName()).filePath();
    m_cover = QFileInfo(dir, QFileInfo(m_cover).fileName()).filePath();
}

void ProjectHistoryModel::setName(QString name) {
    m_name = name;
    SEProject prj(m_localpath);
    prj.setTitle(name);
}

ProjectHistoryListModel::ProjectHistoryListModel(QObject *parent) : QAbstractListModel(parent) {
}

ProjectHistoryListModel::~ProjectHistoryListModel() {
    qDeleteAll(datas);
    datas.clear();
}

ProjectHistoryListModel::ProjectHistoryListModel(ProjectHistoryListModel &model) : QAbstractListModel(model.parent()) {
    datas.append(model.datas);
}

void ProjectHistoryListModel::addData(ProjectHistoryModel *data) {
    beginInsertRows(QModelIndex(), 0, 0);
    datas.insert(0, data);
    endInsertRows();
}

void ProjectHistoryListModel::addDataToEnd(ProjectHistoryModel *data) {
    datas << data;
}

void ProjectHistoryListModel::update(int index, ProjectHistoryModel *data) {
    delete datas[index];
    datas[index] = data;
}

void ProjectHistoryListModel::replace(ProjectHistoryModel *data, ProjectHistoryModel *newData) {
    int index = datas.indexOf(data);
    update(index, newData);
}

int ProjectHistoryListModel::indexOf(ProjectHistoryModel *data) {
    return datas.indexOf(data);
}

int ProjectHistoryListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return datas.size();
}

QVariant ProjectHistoryListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= datas.count())
        return QVariant();

    ProjectHistoryModel *data = datas[index.row()];
    if (role == NameRole)
        return data->name();
    else if (role == CoverRole) {
        return data->cover();
    } else if (role == SizeRole)
        return data->size();
    else if (role == DurationRole)
        return data->duration();
    else if (role == LocalPathRole) {
        return data->localpath();
    }

    return QVariant();
}

QHash<int, QByteArray> ProjectHistoryListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CoverRole] = "cover";
    roles[SizeRole] = "size";
    roles[DurationRole] = "duration";
    roles[LocalPathRole] = "localPath";
    return roles;
}

ProjectHistoryModel *ProjectHistoryListModel::dataWith(int index) {
    return datas[index];
}

void ProjectHistoryListModel::removeAt(int index) {
    if (index >= 0 && index < datas.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        ProjectHistoryModel *item = datas.at(index);
        QFileInfo itemFile(item->localpath());
        QDir itemDir = itemFile.dir();
        if (itemDir.exists()) {
            itemDir.removeRecursively();
        }
        datas.removeAt(index);
        delete item;
        endRemoveRows();
    }
}

void ProjectHistoryListModel::moveToTop(int index) {
    if (datas.size() > 1 && index > 0) {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), 0);
        datas.move(index, 0);
        endMoveRows();
    }
}

void ProjectHistoryListModel::resetModel() {
    beginResetModel();
    endResetModel();
}

int ProjectHistoryListModel::dataCount() {
    return datas.size();
}

void ProjectHistoryListModel::clearAll() {
    beginResetModel();
    qDeleteAll(datas);
    datas.clear();
    endResetModel();
}
