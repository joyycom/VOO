#include "Login.h"
#ifdef Q_OS_WIN
#    include <QLocale>

#    include "base/utils/AppConstance.h"



void Login::init() {

}

void Login::autoLogin(uint64_t uid) {

}

void Login::requestLogin(std::string account, std::string password) {

}

void Login::requestLogout(uint64_t uid) {

}

std::string Login::getOtp(uint64_t uid) {
    return "";
}
#endif
