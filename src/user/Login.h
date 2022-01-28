#ifndef LOGIN_H
#define LOGIN_H
#include <functional>

#include "base/utils/AppConstance.h"
#include "base/utils/SharedPref.h"
#include "base/utils/threadpools.h"

struct LoginCallback final {
    std::function<void(std::string msg)> onShowMessage = nullptr;
    std::function<void(uint64_t uid)> onLoginSuccess = nullptr;
    std::function<void()> onLoginFail = nullptr;
};
class Login {

public:
    static Login *instance() {
        static Login loginHelper;
        return &loginHelper;
    };
    Login() {

    }
    void init();
    void autoLogin(uint64_t uid);
    void requestLogin(std::string account, std::string password);
    void requestLogout(uint64_t uid);
    std::string getOtp(uint64_t uid);
    bool isLogin() {
        return m_loginSuccess;
    }

public:
    bool m_loginSuccess = false;
    LoginCallback m_callback;
};
#endif
