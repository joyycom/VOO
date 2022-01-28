#include "Login.h"
#ifdef Q_OS_OSX
#include <QLocale>

#include "base/utils/AppConstance.h"
#include "base/utils/qt-detect-user-country.h"

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
