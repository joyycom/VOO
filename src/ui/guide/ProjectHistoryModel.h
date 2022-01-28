#ifndef ProjectHistoryModel_H
#define ProjectHistoryModel_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>
#include <QFileInfo>

class ProjectHistoryModel {
public:
    ProjectHistoryModel(const QString &localPath);
    ProjectHistoryModel(const QJsonObject& jsonConfig);

    QString fixPath(QString path);
    QString subPath(QString path);
    QJsonObject toJsonObject();
    void remove();

    QString localpath() const;
    QString cover() const;
    QString name() const;
    QString size() const;
    QString duration() const;

    void updateDir(QString dir);
    void setName(QString name);

private:
    QString m_localpath;
    QString m_cover;
    QString m_name;
    QString m_size;
    QString m_duration;
};

class ProjectHistoryListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum StoreResourceListRole {
        CoverRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        SizeRole = Qt::UserRole + 3,
        DurationRole = Qt::UserRole + 4,
        LocalPathRole = Qt::UserRole + 5
    };

    ProjectHistoryListModel(ProjectHistoryListModel &model);
    ProjectHistoryListModel(QObject *parent = 0);
    ~ProjectHistoryListModel();

    void addData(ProjectHistoryModel *data);
    void addDataToEnd(ProjectHistoryModel *data);
    void update(int index, ProjectHistoryModel *data);
    void replace(ProjectHistoryModel *data, ProjectHistoryModel *newData);
    int indexOf(ProjectHistoryModel *data);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int dataCount();

    void removeAt(int index);

    void clearAll();

    ProjectHistoryModel *dataWith(int index);

    void moveToTop(int index);

    void resetModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<ProjectHistoryModel *> datas;
};

#endif
