#ifndef UploadEffectController_H
#define UploadEffectController_H

#include <QObject>
#include <QUrl>

#include <project/SEProjectProj.h>



class UploadEffectController : public QObject {
    Q_OBJECT

    Q_PROPERTY(QStringList effectTeams READ getEffectTeams NOTIFY effectTeamsChanged)
    Q_PROPERTY(QStringList effectCates READ getEffectCategorys NOTIFY effectCategoryChanged)


public:
    explicit UploadEffectController(QObject *parent = nullptr);
    ~UploadEffectController();

    Q_INVOKABLE void setEffectPath(QString effectPath);

    Q_INVOKABLE QString getEffectName() const{
        if(m_effectInfo.contains("languageName")){
            return m_effectInfo.value("languageName").toObject().value("en").toString("");
        }
        return m_effectInfo.value("name").toString("");
    }
    Q_INVOKABLE QString getEffectCnName() const{
        if(m_effectInfo.contains("languageName")){
            return m_effectInfo.value("languageName").toObject().value("zh").toString("");
        }
        return m_effectInfo.value("name").toString("");
    }
    Q_INVOKABLE QString getEffectCoverPath() const{
        QString cover = m_effectInfo.value("cover").toString("");
        if(cover.isEmpty()){
            return "http://biu-cn.dwstatic.com/sky/20211221/effect_default_icon.png";
        }else{
            return cover;
        }
    }

    Q_INVOKABLE int getEffectCategory() const{
        return m_effectInfo.value("type").toInt(1);
    }
    Q_INVOKABLE int getEffectTeam();
    Q_INVOKABLE QStringList getEffectCategorys(){
        return m_categorys;
    }
    Q_INVOKABLE QStringList getEffectTeams() const{
        return m_userTeams;
    }

    Q_INVOKABLE void uploadEffect(QString name,QString cnName,QString coverPath, int category,int team);

    Q_INVOKABLE void cancel();

signals:
    void effectInfoLoaded();
    void effectTeamsChanged(QStringList teams);
    void effectCategoryChanged(QStringList cates);

    void effectTeamChanged(int effectTeam);
    void effectUploadResult(int code,QString msg);

private:
    void loadRemoteMaterialInfo(QString id);
    void loadMaterialCategory();
    void loadUserTeams();
    void updateUserTeams();
    void performUploadMaterialInfo();
    void loadMaterialFinish(const QString& categories);

private:

    QStringList m_userTeamIds;
    QStringList m_userTeams;
    QStringList m_categorys;

    QString m_effectDir;
    QJsonObject m_effectInfo;
    QString m_curTeamId = "";
    SEProjectProj* m_effectProj = nullptr;

};

#endif // UploadEffectController_H
