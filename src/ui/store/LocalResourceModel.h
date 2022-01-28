#ifndef LocalResourceModel_H
#define LocalResourceModel_H

#include <QAbstractListModel>
#include <QDir>
#include <QObject>
#include <QStringList>

#include "base/utils/fileutils.h"

class LocalResourceModel {
public:
    LocalResourceModel(QString filePath);
    LocalResourceModel(QDir folderPath, QString ofFile = nullptr, QString title = nullptr);
    LocalResourceModel(QJsonObject jsonConfig);
    LocalResourceModel();

    QString fileName() const;
    QString fileCover() const;
    QString filePath() const;
    QString fileType() const;

    // video
    QString videoDuration() const;

    // audio
    QString audioDuration() const;
    QString audioAnchor() const;

    // utils
    QJsonObject toJsonObject();

private:
    QString m_fileName;
    QString m_fileCover;
    QString m_filePath;
    QString m_fileType;
    QString m_videoDuration;
    QString m_audioDuration;
    QString m_audioAnchor;
};

class LocalResourceListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum LocalResourceListRole {
        FileNameRole = Qt::UserRole + 1,
        FileCoverRole = Qt::UserRole + 2,
        FileTypeRole = Qt::UserRole + 3,
        VideoDurationRole = Qt::UserRole + 4,
        AudioDurationRole = Qt::UserRole + 5,
        AudioAnchorRole = Qt::UserRole + 6,
        FilePathRole = Qt::UserRole + 7,
        IsSelectedRole = Qt::UserRole + 8
    };

    LocalResourceListModel(LocalResourceListModel &model);

    LocalResourceListModel(QObject *parent = 0);
    ~LocalResourceListModel();

    void addData(const LocalResourceModel &data);
    void insertData(int index, const LocalResourceModel &data);

    void deleteData(const int index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int dataCount();
    void clearAll();
    LocalResourceModel dataWith(int index);

    Q_INVOKABLE QString filePath(int index){
        return datas[index].filePath();
    }

    bool isExistFile(QString path);

    void setSelectedIndex(int index);

    int curSelectedIndex() const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int m_selectedIndex;
    QList<LocalResourceModel> datas;
};

#endif
