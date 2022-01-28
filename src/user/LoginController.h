#ifndef LOGIN_CONTROLLER_H
#define LOGIN_CONTROLLER_H
#include <QObject>

#include "Login.h"

class LoginController : public QObject {
    Q_OBJECT
public:
    explicit LoginController(QObject *parent = nullptr);
    ~LoginController();

    static LoginController* instance();

    Q_INVOKABLE bool isLogin();

    Q_INVOKABLE QString getNickname();

    Q_INVOKABLE QString getRegUrl();

    Q_INVOKABLE QString getForgetUrl();

    Q_INVOKABLE void requestLogin(QString account, QString password);

    Q_INVOKABLE void requestLogout();

signals:
    void showMessage(QString msg);
    void loginSuccess();
    void updateNickname(QString nickname);
    void logout();

private:
    static LoginController *m_currentInstance;

};
#endif
