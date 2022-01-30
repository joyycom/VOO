#include "Login.h"
#ifdef Q_OS_WIN
#    include <QLocale>

#    include "base/utils/AppConstance.h"



void Login::init() {

}

void Login::autoLogin(uint64_t uid) {

}

void Login::requestLogin(std::string account, std::string password) {
    auto login = Login::instance();
    login->m_loginSuccess = false;
    auto callback = login->m_callback;
    if (login->m_loginSuccess) {
        callback.onLoginSuccess(0);
    } else {
        callback.onLoginFail();
        callback.onShowMessage("Login Failed");
    }
}

void Login::requestLogout(uint64_t uid) {

}

std::string Login::getOtp(uint64_t uid) {
    return "";
}
#endif
