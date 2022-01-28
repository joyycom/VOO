#ifndef StoreResourceModel_H
#define StoreResourceModel_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>

#include <timeline/models/skyresourcebean.h>

class StoreResourceModel {
public:
    StoreResourceModel(const QJsonObject &jo);
    StoreResourceModel();

    QList<QString> supportCategory;

    QString id() const;
    QString name() const;
    QString cover() const;
    QString preview() const;
    QString sourceUrl() const;
    QString cate() const;
    QStringList subCate() const;
    QString tinyCate() const;
    QString required() const;
    QString ofVersion() const;
    QString sdkVersion() const;
    QString createTime() const;
    QString updateTime() const;
    QString userId() const;
    QString userName() const;
    QString duration() const;

    bool avaliable() const;
    QString getLocalDir() const;
    bool isVenusEffect() const;

    void download(std::function<void(const bool, const QString &)> complateHandler);

private:
    QString m_id;
    QString m_name;
    QString m_cover;
    QString m_preview;
    QString m_sourceUrl;
    QString m_cate;
    QStringList m_subCate;
    QString m_tinyCate;
    QString m_required;
    QString m_ofVersion;
    QString m_sdkVersion;
    QString m_createTime;
    QString m_updateTime;
    QString m_userId;
    QString m_userName;
    QString m_duration;
    bool m_isVenusEffect = false;
    bool m_isDownloaded = false;
};

class StoreResourceListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum StoreResourceListRole {
        IdRole = Qt::UserRole + 1,
        UrlRole = Qt::UserRole + 2,
        NameRole = Qt::UserRole + 3,
        CoverRole = Qt::UserRole + 4,
        PreviewRole = Qt::UserRole + 5,
        ExistRole = Qt::UserRole + 6,
        DurationRole = Qt::UserRole + 7,
        IsSelectedRole = Qt::UserRole + 8,
        CateRole = Qt::UserRole + 9,
        IsVenusEffectRole = Qt::UserRole + 10
    };

    StoreResourceListModel(StoreResourceListModel &model);

    StoreResourceListModel(QObject *parent = 0);

    void addData(const StoreResourceModel &data);
    void insertData(int index, const StoreResourceModel &data);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    void setSelectedIndex(int index);

    int curSelectedIndex() const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void clearAll();
    StoreResourceModel dataWith(int index);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int m_selectedIndex;
    QList<StoreResourceModel> datas;
};

#endif
