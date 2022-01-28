#ifndef ANIMATIONLISTCOMPONENT_H
#define ANIMATIONLISTCOMPONENT_H

#include "BaseComponent.h"
#include <QAbstractListModel>
#include <QUrl>
#include <QFile>
#include <QDebug>

class AnimationBean {
public:
    AnimationBean(QString iconURL, QString name) : m_iconURL(iconURL), m_name(name) {
    }
    AnimationBean(const AnimationBean &animation) {
        m_id = animation.m_id;
        m_type = animation.m_type;
        m_iconURL = animation.m_iconURL;
        m_name = animation.m_name;
        m_dynamicIcon = animation.m_dynamicIcon;
        m_source = animation.m_source;
        m_localPath = animation.m_localPath;
    }
    AnimationBean(const QJsonObject &object);

    bool isDownload() const;
    QString getDownloadedDynamicIcon() const;
    static QString type2Path(const QString &type);
    void download(std::function<void(const bool, const QString &, const QString &)> complateHandler);

    QString iconURL() const {
        return m_iconURL;
    }
    QString name() const {
        return m_name;
    }
    QString dynamicIcon() const {
        return m_dynamicIcon;
    }
    QString source() const {
        return m_source;
    }
    QString type() const {
        return m_type;
    }
    QString localPath() const {
        return m_localPath;
    }

    QString id() const{
        return m_id;
    }

private:
    QString m_id = "";
    QString m_type = "";
    QString m_iconURL = "";
    QString m_dynamicIcon = "";
    QString m_name = "";
    QString m_source = "";
    QString m_localPath = "";
};

class AnimationsListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum AnimationModel {
        IconRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        PathRole = Qt::UserRole + 3,
        SelectedRole = Qt::UserRole + 4,
        DynamicRole = Qt::UserRole + 5,
        isDownloadRole = Qt::UserRole + 6
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        return m_datas.count();
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        const AnimationBean &animation = m_datas[index.row()];
        if (role == IconRole)
            return animation.iconURL();
        else if (role == NameRole)
            return animation.name();
        else if (role == PathRole)
            return animation.localPath();
        else if (role == SelectedRole) {
            return m_selectedIndex == index.row();
        } else if (role == DynamicRole) {
            return animation.dynamicIcon();
        } else if (role == isDownloadRole) {
            return animation.isDownload();
        }
        return "";
    }

    void addData(const QList<AnimationBean> &data);

    AnimationBean dataWith(int index);

    void clearAll();

    void setSelectedIndex(int i);
    int findAnimationBeanIndex(QString id);

protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[IconRole] = "icon";
        roles[NameRole] = "name";
        roles[PathRole] = "path";
        roles[SelectedRole] = "isSelected";
        roles[DynamicRole] = "dynamicIcon";
        roles[isDownloadRole] = "isDownload";
        return roles;
    }

private:
    QList<AnimationBean> m_datas;
    int m_selectedIndex = 0;
};

class AnimationListComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(AnimationsListModel *listModel READ listModel NOTIFY listModelChanged)
public:
    AnimationListComponent(QObject *parent = nullptr);

    ~AnimationListComponent();

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    void onOfParamsChanged(QString key, SkyVariant skyVariant) override;

    void onClipParamsChanged() override;

    void loadDatas(const QString &response);

    void changeAnimationType(int itemIndex);

    int getCurSelectedIndex();

public slots:
    double minDuration();
    double maxDuration();

    AnimationsListModel *listModel() const;

    void onAnimationTypeChange(int itemIndex);

    double defDurationForSection();
    double durationForSection();

    void onAnimationDurationChange(float duration);
    void onAnimationDurationChangeByUser(float preDuration, float duration);

    void clickDownload(int index);

signals:
    void maxDurationChanged();

    void listModelChanged();

    void onDownloading(bool downloading, int index);

    void durationForSectionChanged(float duration);

private:
    SkyTimeRange calcPreviewAnimationTimeRange();

    void updateAnimationMaxDuration();

    const QString EnterAnimationDuringKey = "0:EnterAnimDuration";
    const QString ExitAnimationDuringKey = "0:ExitAnimDuration";


    QString m_paramTitle = "";
    QString m_url = "";
    QString m_animationKey = "";
    QString m_effectiveTime = "";
    QString m_durationKey = "";
    QString m_oppositeKey = "";
    QStringList m_exclusiveKeys;
    double m_maxDuration = 0;
    AnimationsListModel *m_listModels = new AnimationsListModel;
};

#endif // ANIMATIONLISTCOMPONENT_H
