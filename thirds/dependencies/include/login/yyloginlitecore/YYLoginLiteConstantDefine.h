//
//  YYLoginLiteConstantDefine.h
//  yyloginlitesdk
//
//  Created by YushiPark on 2020/1/21.
//  Copyright © 2020 yy. All rights reserved.
//

#ifndef YYLoginLiteConstantDefine_h
#define YYLoginLiteConstantDefine_h

#define SDKLoginErrorDomain @"com.login.duowan"

static NSString * const kYYLoginLiteHTTPTest2BaseURL = @"https://os-aq-test.duowan.com";
static NSString * const kYYLoginLiteHTTPBaseURL = @"https://lgglobal.duowan.com";

// 登录
typedef NS_ENUM(NSInteger, YYLOGIN_ERROR_CODE){
    YYLOGIN_PARAM_MISSING = -1,          // 缺少参数
    YYLOGIN_PARAM_DENIED = -2,           // 没有接入权限
    YYLOGIN_THIRD_AUTH_FAIL = -3,        // 第三方认证失败
    YYLOGIN_PARAM_INVALID = -4,          // 参数非法
    YYLOGIN_OPERATION_ILLEGAL = -5,      // 非法操作
    YYLOGIN_AUTH_FAIL = -6,              // 认证失败
    YYLOGIN_FREQ_LIMIT = -7,             // 频率限制
    YYLOGIN_VERIFY_FAIL = -8,            // 验证失败
    YYLOGIN_USRE_NOT_EXIST = -9,         // 账号不存在
    YYLOGIN_REG_FAIL = -10,              // 注册失败
    YYLOGIN_REG_WRTITE_COOKIE_FAIL = -11,// 注册写cookie失败
    YYLOGIN_IP_LIMIT = -12,              // IP限制
    YYLOGIN_TIME_EXPIRE = -13,           // 时间过期
    YYLOGIN_USER_BAN = -14,              // 账号封禁
    YYLOGIN_BIND_EXIST = -15,            // 帐号已经存在绑定信息
    YYLOGIN_VERIFY_MOBILE = -16,         // 需要绑定/验证手机
    YYLOGIN_NOT_ALLOW_LOGIN = -17,       // 不允许登录
    YYLOGIN_VERIFY_MOBILE_STOKEN = -18,  // 需要绑定手机，但是会在这基础上，多返回一个stoken，用于极光一键强绑手机
    YYLOGIN_SYSTEM_ERROR  = -999,        // 系统错误
    YYLOGIN_TIME_OUT = -1001,            // 请求超时
    
    YYLOGIN_VER_CANCEL = -400,           //用户取消二次验证
    YYLOGIN_PARSE_ERROR = -404,          // 解析PB失败
    YYLOGIN_SUCCESS = 0,                 //登录成功
    YYLOGIN_NEXT_VER = 1,                //二次验证
    YYLOGIN_PWD_ERR = 2,                 //密码错误
    YYLOGIN_USER_NOT = 3,                //帐号不存在
    YYLOGIN_USER_LOCK = 4,               //帐号锁定（用户自己锁定）
    YYLOGIN_REJECT = 5,                  //安全风险临时拒绝登录
    YYLOGIN_BAND = 6,                    //封禁
    YYLOGIN_PARAM_ERR = 7,               //参数错误
    YYLOGIN_REGUSER_EXIST = 8,              //注册用户已存在

    YYLOGIN_SMS_LIMIT = 10,              //下发短信验证码超过最大次数
    YYLOGIN_SMSCODE_ERR = 11,            //短信验证码错误
    YYLOGIN_SMSCODE_EXPIRE = 12,         //短信验证码过期
    YYLOGIN_VCODE_ERR = 13,              //二次验证码错误
    YYLOGIN_VCODE_EXPIRE = 14,           //二次验证码已过期
    YYLOGIN_SMS_RATE = 15,            //下发短信验证码频率限制
    YYLOGIN_SMSCODE_VLIMIT = 16,            //短信验证码错误次数限制，需重新获取验证码
    
    YYLOGIN_CREDIT_ERR = 20,             //凭证错误或无效
    YYLOGIN_CREDIT_EXPIRE = 21,          //凭证过期
    YYLOGIN_DEVICE_ERR = 22,             //凭证设备不匹配
    YYLOGIN_APPID_ERR = 23,              //凭证appid不匹配
    YYLOGIN_GEN_CREDIT_ERR = 24,         //产凭证错误
    
    YYLOGIN_SESS_EXPIRE = 31,            //sessoindata已失效
    YYLOGIN_SESS_DECODE = 32,            //sessoindata解码失败
    YYLOGIN_SESS_VER_ERR = 33,           //sessoindata校验失败
    YYLOGIN_SESS_NULL = 34,              //sessoindata为空
    
    YYLOGIN_BIND_HAS_USER = 40,          // 手机用户已存在，请绑定其他手机号码
    YYLOGIN_BIND_HAS_MOBILE = 41,        // 用户已绑定手机，请用手机号登录
    YYLOGIN_BIND_HAS_THIRD = 42,         // 用户已绑定第三方帐号，请用第三方帐号登录

    YYLOGIN_BIND_REJECTAR = 50,          // 风控拒绝
    YYLOGIN_REJECT_OFF = 51,             // 下线拒绝
    
    QRCODE_NO_CONFIRM = 60,              //QRCODE已扫描，未确认，客户端提示扫码端确认，稍后重试登录
    QRCODE_NO_SCAN = 61,                 //QRCODE未扫描，客户端继续展示二维码，稍后重试登录
    QRCODE_EXPIRE = 62,                  //QRCODE过期，客户端终止重试
    QRCODE_NOT_EXIST = 63,               //QRCODE不存在，客户端终止重试
    QRCODE_CANCLE = 64,                  //QRCODE已被用户取消，客户端终止重试
    QRCODE_USER = 65,                    //QRCODE用户取消，用户错误
    
    YYLOGIN_CLIENT_RETRY = 99,           //客户端重试，db或服务错误，需要客户端自动重试
    YYLOGIN_SYS_BUSY = 100,              //服务器限流，客户端不要重试，慎防雪崩
};

// 绑三方登录
typedef NS_ENUM(NSInteger, YYBING_ERROR_CODE) {
    YYBING_SUCC = 0,               //绑定成功
    YYBING_PARAM_MISSING = -1,     //缺少参数
    YYBING_PERMISSION_DENIED = -2, //没有接入权限
    YYBING_THIRD_AUTH_FAIL = -3,   //第三方认证失败
    YYBING_PARAM_INVAILD = -4,     //参数非法
    YYBING_AUTH_FAIL = -6,         //票据认证失败
    YYBING_BIND_EXISTS = -15,      //帐号已经存在绑定信息
    YYBING_SYSTEM_ERROR = -999,    //系统错误
};

// 绑手机
typedef NS_ENUM(NSInteger, YYREGISTER_ERROR_CODE){
    YYREGISTERErr_Csuccess = 0,
    YYREGISTERErr_CerrParam = 1,
    YYREGISTERErr_CerrSys = 2,
    
    /** 手机号已存在 */
    YYREGISTERErr_CerrMobExits = 3,
    
    /** 用户不存在，例如改密操作 */
    YYREGISTERErr_CerrNoUser = 4,
    
    /** 邮箱已存在 */
    YYRegisterErr_CerrEmailExits = 5,
    
    /** 频率限制 */
    YYClientRegisterErr_CerrBusyLimit = 6,
    
    /** otp校验错误 */
    YYClientRegisterErr_CerrOtpVer = 7,
    
    /** 手机不存在 */
    YYClientRegisterErr_CerrMobNotExits = 8,

    /** 邮箱不存在 */
    YYClientRegisterErr_CerrEmailNotExits = 9,
    
    /** 通行证已存在 */
    YYClientRegisterErr_CerrUsernameExits = 13,

    /** 通行证不存在 */
    YYClientRegisterErr_CerrUsernameNotExits = 14,
    
    /** 二次验证//20210318改动 */
    YYClientRegisterErr_CerrNextVer = 31,

    /** 二次验证码错误//20210318改动 */
    YYClientRegisterErr_CerrVcodeErr = 32,

    /** 二次验证码已过期//20210318改动 */
    YYClientRegisterErr_CerrVcodeExpire = 33,
    
    /** 短信验证码错误 */
    YYREGISTERErr_CerrSmscodeWrong = 51,
    
    /** 短信验证码过期 */
    YYREGISTERErr_CerrSmscodeExpire = 52,
    
    /** 短信验证码验证次数限制 */
    YYClientRegisterErr_CerrSmscodeVerLimit = 53,

    /** 邮箱验证码错误 */
    YYClientRegisterErr_CerrEmailcodeWrong = 54,

    /** 邮箱验证码过期 */
    YYClientRegisterErr_CerrEmailcodeExpire = 55,

    /** 邮箱验证码验证次数限制 */
    YYClientRegisterErr_CerrEmailcodeVerLimit = 56,
    
    /** 密保手机绑定达到上限 */
    YYClientRegisterErr_CerrMibaoMobileLimit = 61,

    /** 密保手机与登录手机不一致 */
    YYClientRegisterErr_CerrMobileConflict = 62,
    
    /** 根据区域拒绝用户注册 */
    YYClientRegisterErr_CerrRefusedArea = 90,

    /** 安全风险临时拒绝登录//20210318改动 */
    YYClientRegisterErr_CerrReject = 91,
    
    /** 客户端重试，db或服务错误，需要客户端自动重试 */
    YYREGISTERErr_CerrClientRetry = 99,
    
    /** 服务器限流，客户端不要重试，慎防雪崩 */
    YYREGISTERErr_CerrSysBusy = 100,
};

typedef NS_ENUM(NSInteger, YYLoginLiteErrType){
    /**
     * Service通道SDK定义的错误码
     */
    SERVICE_SDK_CODE = 0,
    
    /**
     * Service通道服务端定义的错误码
     */
    SERVICE_SRV_CODE = 1,
    /**
     * 账号SDK定义的错误码

     */
    AUTH_SDK_CODE = 3,
    
    /**
     * 账号服务端定义的错误码
     */
    AUTH_SRV_CODE = 4,
    
    /**
     * 账号服务端定义的错误码
     */
    AUTH_SRV_REGISTER_CODE = 5,
};

typedef NS_ENUM(NSInteger, YYLOGIN_THIRD_TOKEN_TYPE){
    YYLOGIN_THIRD_CODE = 0,               // OAUTH2 CODE
    YYLOGIN_THIRD_ACCESS_TOKEN = 1        // OAUTH2 ACCESSTOKEN
};

typedef NS_ENUM(NSInteger, YYLOGIN_SMS_TYPE){
    YYLOGIN_SMS_TEXT = 0,               // 文字短信
    YYLOGIN_SMS_VOICE = 1               // 语音短信
};

typedef NS_ENUM(NSInteger, YYLOGIN_REQUEST_TYPE) {
    YYLOGIN_REQUEST_SERVICE = 0,   // 使用service请求
    YYLOGIN_REQUEST_HTTP = 1       // 使用http请求
};

#endif /* YYLoginLiteConstantDefine_h */
