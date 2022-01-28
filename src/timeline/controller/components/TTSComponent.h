#ifndef TTSComponent_H
#define TTSComponent_H

#include "BaseComponent.h"

#include <QNetworkReply>
#include <QSound>

class TTSBean {
public:
    TTSBean(QString id, QString name,QList<TTSBean>& subBeans) : m_id(id), m_name(name),m_subBeans(subBeans) {
    }

    TTSBean(QString id, QString name) : m_id(id), m_name(name) {

    }

    TTSBean(const TTSBean &bean) {
        m_id = bean.m_id;
        m_name = bean.m_name;
        m_subBeans = bean.m_subBeans;
    }

    QString name() const {
        return m_name;
    }

    QString id() const{
        return m_id;
    }

    QList<TTSBean> subBeans(){
        return m_subBeans;
    }

    bool isDownloaded() const{
        return m_isDownloaded;
    }

    void setDownloaded(bool downloaded){
        m_isDownloaded = downloaded;
    }

private:
    bool m_isDownloaded = false;
    QString m_id = "";
    QString m_name = "";
    QList<TTSBean> m_subBeans;
};

class TTSBeanListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum TTSBeanModel {
        IconRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        SelectedRole = Qt::UserRole + 3,
        DownloadedRole = Qt::UserRole + 4
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        return m_datas.count();
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        const TTSBean &bean = m_datas[index.row()];
        if (role == NameRole)
            return bean.name();
        else if (role == SelectedRole) {
            return m_selectedIndex == index.row();
        } else if(role == DownloadedRole){
            return bean.isDownloaded();
        }
        return "";
    }

    void addData(const QList<TTSBean> &data);

    TTSBean dataWith(int index);

    void setCurItemDownloaded(bool isDownloaded);

    void clearAll();

    void setSelectedIndex(int i);

    TTSBean curSelectedBean();

protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[IconRole] = "icon";
        roles[NameRole] = "name";
        roles[SelectedRole] = "isSelected";
        roles[DownloadedRole] = "isDownloaded";

        return roles;
    }

private:
    QList<TTSBean> m_datas;
    int m_selectedIndex = -1;
};


class TTSComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(TTSBeanListModel *languageModel READ languageModel NOTIFY languageModelChanged)
    Q_PROPERTY(TTSBeanListModel *voiceModel READ voiceModel NOTIFY voiceModelChanged)

public:
    TTSComponent(QObject *parent = nullptr);
    ~TTSComponent();

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE TTSBeanListModel* voiceModel(){
        return m_voiceModel;
    }

    Q_INVOKABLE TTSBeanListModel* languageModel(){
        return m_languageModel;
    }

    Q_INVOKABLE void selectLanguageIndex(int index);

    Q_INVOKABLE void selectVoiceIndex(int index);

    Q_INVOKABLE void addCurVoiceResource();



signals:
    void voiceModelChanged(TTSBeanListModel* model);
    void languageModelChanged(TTSBeanListModel* model);


private:
    QList<TTSBean> loadTtsSettings();

    void downloadTTSVoice();

    void playCurVoice(QString path);

    QSound* m_voiceSound = nullptr;
    QNetworkReply* m_curReply = nullptr;

    TTSBeanListModel* m_languageModel = new TTSBeanListModel;
    TTSBeanListModel* m_voiceModel = new TTSBeanListModel;
};

#endif // TTSComponent_H
