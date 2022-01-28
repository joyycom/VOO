#include "LoginController.h"

#include <QDebug>

#include <base/http/BaseRsp.h>
#include <base/http/SEHttpClient.h>

LoginController *LoginController::m_currentInstance = NULL;
LoginController *LoginController::instance() {
    return m_currentInstance;
}

LoginController::LoginController(QObject* parent) {
    m_currentInstance = this;
    auto login = Login::instance();
    login->m_callback.onLoginFail = [=](){
        qInfo()<<"Login Failed!";
    };
    login->m_callback.onLoginSuccess = [=](uint64_t uid){
        emit loginSuccess();
        QString otp = QString::fromStdString(Login::instance()->getOtp(AppConstance::uid()));
        if (!otp.isEmpty()) {
            ThreadPools::instance()->mainThread(
                        [this, otp] {
                SEHttpClient(AppConstance::serverDomain() + "/v1/sky/userInfo/getLoginUserInfo")
                        .addParam("sign", otp)
                        .get([=](const QString &response) {
                    BaseRsp rsp(response);
                    if (rsp.isSuccess()) {
                        QString nickname = rsp.data().toObject().take("nick").toString();
                        SharedPref::instance()->setString(KEY_APP_NICKNAME, nickname);
                        emit updateNickname(nickname);
                    }
                });
            },
            0);
        }
    };
    login->m_callback.onShowMessage = [=](std::string msg){
        emit showMessage(QString::fromStdString(msg));
    };

}

LoginController::~LoginController() {
    auto login = Login::instance();
    login->m_callback.onLoginFail = nullptr;
    login->m_callback.onLoginSuccess = nullptr;
    login->m_callback.onShowMessage = nullptr;
}

bool LoginController::isLogin() { return AppConstance::uid() > 0; }

QString LoginController::getNickname() { return AppConstance::nickname(); }

QString LoginController::getRegUrl() { return "https://vms.vooedit.com/"; }

QString LoginController::getForgetUrl() { return "https://vms.vooedit.com/"; }

void LoginController::requestLogin(QString account, QString password) {
    Login::instance()->requestLogin(account.toStdString(), password.toStdString());
}

void LoginController::requestLogout() {
    Login::instance()->requestLogout(AppConstance::uid());
    AppConstance::clearUserInfo();
    emit logout();
}
