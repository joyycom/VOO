#pragma once
#include <windows.h>
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

namespace protocol
{
	namespace udbsdk
	{
		extern "C"
		{
			enum Errcode
			{
				SUCCESS = 0,				//登录成功
				NEXT_VER = 1,			//二次验证
				PWD_ERR = 2,			//密码错误
				USER_NOT = 3,				//帐号不存在
				USER_LOCK = 4,				//帐号锁定（用户自己锁定）
				REJECT = 5,				//安全风险临时拒绝登录
				BAND = 6,				//封禁
				PARAM_ERR = 7,			  //参数错误

				SMS_LIMIT = 10,			//下发短信验证码超过最大次数
				SMSCODE_ERR = 11,			//短信验证码错误
				SMSCODE_EXPIRE = 12,			//短信验证码过期
				VCODE_ERR = 13,			//二次验证码错误
				VCODE_EXPIRE = 14,			//二次验证码已过期
				SMS_RATE = 15,			//下发短信验证码频率限制

				CREDIT_ERR = 20,		  //凭证错误或无效
				CREDIT_EXPIRE = 21,			//凭证过期
				DEVICE_ERR = 22,			//凭证设备不匹配
				APPID_ERR = 23,			//凭证appid不匹配
				GEN_CREDIT_ERR = 24,			//产凭证错误

				SESS_EXPIRE = 31,			//sessoindata已失效
				SESS_DECODE = 32,			//sessoindata解码失败
				SESS_VER_ERR = 33,			//sessoindata校验失败
				SESS_NULL = 34,			//sessoindata为空

				QRCODE_NO_CONFIRM = 60,   //QRCODE已扫描，未确认，客户端提示扫码端确认，稍后重试登录
				QRCODE_NO_SCAN = 61,   //QRCODE未扫描，客户端继续展示二维码，稍后重试登录
				QRCODE_EXPIRE = 62,          //QRCODE过期，客户端终止重试
				QRCODE_NOT_EXIST = 63,   //QRCODE不存在，客户端终止重试
				QRCODE_CANCLE = 64,   //QRCODE已被用户取消，客户端终止重试
				QRCODE_USER = 65,   //QRCODE用户取消，用户错误

				CLIENT_RETRY = 99,			//客户端重试，db或服务错误，需要客户端自动重试
				SYS_BUSY = 100,			//服务器限流，客户端不要重试，慎防雪崩

				BIND_MOBILE = 101,
			};

			enum ClientRegisterErr {

				CSUCCESS = 0,
			    CERR_PARAM = 1,
			    CERR_SYS = 2,
			    CERR_MOB_EXITS = 3,              //手机号已存在
			    CERR_NO_USER = 4,               //用户不存在，例如改密操作
				CERR_EMAIL_EXITS = 5,           //邮箱已存在
				CERR_BUSY_LIMIT = 6,            //频率限制
				CERR_OTP_VER = 7,               //otp校验错误
			    CERR_SMSCODE_WRONG = 51,        //短信验证码错误
			    CERR_SMSCODE_EXPIRE = 52,       //短信验证码过期
				CERR_SMSCODE_VER_LIMIT = 53,
				CERR_EMAILCODE_WRONG = 54,
				CERR_EMAILCODE_EXPIRE = 55,
				CERR_EMAILCODE_VER_LIMIT = 56,

			    CERR_CLIENT_RETRY = 99,		    //客户端重试，db或服务错误，需要客户端自动重试
			    CERR_SYS_BUSY = 100,			//服务器限流，客户端不要重试，慎防雪崩
			};
			enum UserInfoErr
			{
				SUCCESS1 = 0,	
				Not_Exist = 1,
				Param_ERR = 2,
				SmsBroker_ERR = 3,
				DB_ERR = 96,
				Freq_Limit = 97,
				AUTH_ERR = 98,
				SYS_ERR = 99,
			};

			enum Status {
				EInit = 0,
				EOnConnecting = 1,
				EOnConnecttimeout = 2,
				EOnConnected = 3,
				EOnClosed = 4,
				EClose = 5,
				EBinded = 6,
				EBindTokenInvalid = 254 //该错误代表已登录的token失效 需要通知UI重新登录
			};


			struct ProtoUserData
			{
				uint64_t uid;	                //用户uid，登录成功后返回
				uint64_t uno;                   //用户号码，登录成功后返回
				const char* credit;				//凭证，登录成功后返回
				const char* signapotp;
				const char* accesstoken;
				const char* ext;
			};
			struct ProtoAntiCode
			{
				const char* biz_name;
				const char* anti_code;
			};
			struct SendSmsRsp
			{
				const char* context;			     //预留字段，上下文用于异步化处理
				Errcode     errcode;				//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				//预留字段，动态验证的H5 url
				const char* sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* ext;                    //预留字段,扩展
			};
			struct SmsLoginRsp
			{
				const char* context;			        //预留字段，上下文用于异步化处理
				Errcode errcode;				        //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		        //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				    //预留字段，动态验证的H5 url
				const char* sessiondata;			    //预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* url;                        //预留字段，登录成功后打开一个页面。
				ProtoUserData userData;
				uint32_t server_time;		            //服务器时间。unix时间戳
				uint32_t isnewuser;                     //1:新注册用户，0:存量用户
				const char* ext;                        //预留字段,扩展
			};
			struct CreditLoginRsp
			{
				const char* context;			        //预留字段，上下文用于异步化处理
				Errcode errcode;				        //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		        //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				    //预留字段，动态验证的H5 url
				const char* sessiondata;			    //预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* url;                        //预留字段，登录成功后打开一个页面。
				ProtoUserData userData;
				uint32_t server_time;		            //服务器时间。unix时间戳
				const char* ext;                        //预留字段,扩展
			};
			struct PwdLoginRsp
			{
				const char* context;			        //预留字段，上下文用于异步化处理
				Errcode errcode;				        //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		        //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				    //预留字段，动态验证的H5 url
				const char* sessiondata;			    //预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* url;                        //预留字段，登录成功后打开一个页面。
				ProtoUserData userData; 
				uint32_t    server_time;		  //服务器时间。unix时间戳
				uint32_t strategy;              //策略,0x08:短信，跳转到短信登录流程
				const char* ext;                 //预留字段,扩展,{"SignApOtp":"","AccessToken":""}
			};
			struct SmsRegisterRsp
			{
				const char*         context;			//预留字段，上下文用于异步化处理
			    ClientRegisterErr   errcode;			//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
			    const char*         description;		//错误提示文案，用于直接在ui显示。可能包含超链接内容。
			    const char*         dyn_ver;			//预留字段，动态验证的H5 url
			    const char*         sessiondata;	    //预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
			    const char*         url;                //预留字段，登录成功后打开一个页面。
			    ProtoUserData       userData;
			    uint32_t            server_time;		//服务器时间。unix时间戳
			    const char*         ext;                //预留字段,扩展
			};

			struct SmsModifyPwdRsp
			{
				const char*        context;			    //预留字段，上下文用于异步化处理
			    ClientRegisterErr   errcode;		    //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
			    const char*        description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
			    const char*        dyn_ver;				//预留字段，动态验证的H5 url
			    const char*        sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
			    uint32_t        server_time;		    //服务器时间。unix时间戳
		        const char*        ext;                 //预留字段,扩展
			};
			struct ThirdloginRsp
			{
				uint32_t    version;
				uint64_t     uid;
			    const char* context;
			    const char* rescode;                   //返回码，0 成功，其他失败
			    const char* message;                   //返回结果UI提示信息
			    const char* data;                      //返回登录信息，json格式
				const char* newUser;                   //是否第一次登陆、true/false
				const char* nickname;                  //第三方昵称
				const char* sex;                       //第三方性别
				const char* channel;                   //登陆渠道
				const char* openid;                    //第三方openid
				const char* unionid;                   //第三方unionid，可空  一般QQ、微信才有
				const char* avatar;                     //第三方头像url,可空
				const char* accessToken;                //第三方accesstoken，可空
				const char* tokenSecret;                 //第三方tokensecret，可空，一般是oauth1.0的第三方才有，例如twitter

			};
			struct QrCodeRsp
			{
				const char* context;			        //预留字段，上下文用于异步化处理
				Errcode errcode;				        //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		        //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* qrcode_id;					//二维码id，用于生成二维码图片
			};

			struct QrLoginRsp
			{
				const char* context;			        //预留字段，上下文用于异步化处理
				Errcode errcode;				        //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				ProtoUserData userData;
				uint32_t    server_time;		  //服务器时间。unix时间戳
			};

			struct SendEmailRsp
			{
				const char* context;			     //预留字段，上下文用于异步化处理
				ClientRegisterErr  errcode;				//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				//预留字段，动态验证的H5 url
				const char* sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* ext;                    //预留字段,扩展
			};

			struct RegisterByCodeRsp
			{
				const char*         context;			//预留字段，上下文用于异步化处理
				ClientRegisterErr   errcode;			//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char*         description;		//错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char*         dyn_ver;			//预留字段，动态验证的H5 url
				const char*         sessiondata;	    //预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char*         url;                //预留字段，登录成功后打开一个页面。
				ProtoUserData       userData;
				uint32_t            server_time;		//服务器时间。unix时间戳
				const char*         ext;                //预留字段,扩展
			};

			struct ModifyPwdByCodeRsp
			{
				const char*        context;			    //预留字段，上下文用于异步化处理
				ClientRegisterErr   errcode;		    //错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char*        description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char*        dyn_ver;				//预留字段，动态验证的H5 url
				const char*        sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				uint32_t        server_time;		    //服务器时间。unix时间戳
				const char*        ext;                 //预留字段,扩展
			};

			struct VerifyEmailCodeRsp
			{
				const char* context;			     //预留字段，上下文用于异步化处理
				ClientRegisterErr  errcode;				//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				//预留字段，动态验证的H5 url
				const char* sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* ext;                    //预留字段,扩展
			};

			struct SendSmsWithBusicRsp
			{
				const char* context;			     //预留字段，上下文用于异步化处理
				ClientRegisterErr  errcode;			//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				//预留字段，动态验证的H5 url
				const char* sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* ext;                    //预留字段,扩展
			};

			struct VerifySmsCodeRsp
			{
				const char* context;			     //预留字段，上下文用于异步化处理
				ClientRegisterErr  errcode;			//错误码，0为成功，1为需要动态验证，其他取值含义请看“错误码取值含义”
				const char* description;		    //错误提示文案，用于直接在ui显示。可能包含超链接内容。
				const char* dyn_ver;				//预留字段，动态验证的H5 url
				const char* sessiondata;			//预留字段，若需要动态验证，则需要把sessiondata再下次请求中带回
				const char* ext;                    //预留字段,扩展
			};

			struct MobileRsp
			{
				const char* context;
				UserInfoErr rescode ;
			    const char* errmsg ;
			    const char* ext ;
			};
			struct resCodeResponse
			{
				int resCode;
				const char* msg; 
				int msgSize;

			};

			struct OTPInfo
			{
				char *otp;
				int otpSize;
                OTPInfo() {
                    otp = NULL;
                    otpSize = 0;
				}
			};

			struct IloginSdkNotify
			{
				/*
				resCodeResponse->rescode == 200 成功
				*/
				virtual void OnChannelStatusNotify(int status) = 0;//登录时需要监听
				virtual void OnForceOut(uint64_t uid, uint32_t code, const char* desc, const int descSize) = 0;//同一账户登录、同时多次登录、会收到这个响应、表示被踢

				virtual void smsResponse(const SendSmsRsp* rsp,resCodeResponse resCode) = 0;
				virtual void smsLoginResponse(const SmsLoginRsp* rsp, resCodeResponse resCode) = 0;
				virtual void creditLoginResponse(const CreditLoginRsp* rsp, resCodeResponse resCode) = 0;
				virtual void passwordLoginResponse(const PwdLoginRsp* rsp, resCodeResponse resCode) = 0;
				virtual void SmsRegisterResponse(const SmsRegisterRsp* rsp, resCodeResponse resCode) = 0;
				virtual void sModifyPwdResponse(const SmsModifyPwdRsp* rsp,resCodeResponse resCode) = 0;
				virtual void ThirdloginRspResponse(const ThirdloginRsp* rsp, resCodeResponse resCode) = 0;
				virtual void MobileRspResponse(const MobileRsp* rsp, resCodeResponse resCode) = 0;
				virtual void outSigninResponse(const int resCode)=0;
				virtual void QrCodeRspResponse(const QrCodeRsp* rsp, resCodeResponse resCode) = 0;
				virtual void QrLoginRspResponse(const QrLoginRsp *rsp, resCodeResponse resCode) = 0;
				virtual void SendEmailRspResponse(const SendEmailRsp *rsp, resCodeResponse resCode) = 0;
				virtual void RegisterByCodeResponse(const RegisterByCodeRsp *rsp, resCodeResponse resCode) = 0;
				virtual void ModifyPwdByVerifyCodeRspResponse(const ModifyPwdByCodeRsp *rsp, resCodeResponse resCode) = 0;
				virtual void VerifyEmailCodeRspResponse(const VerifyEmailCodeRsp *rsp, resCodeResponse resCode) = 0;
				virtual void SmsRequestWithBusiRspResponse(const SendSmsWithBusicRsp *rsp, resCodeResponse resCode) = 0;
				virtual void VerifySMSCodeRspResponse(const VerifySmsCodeRsp *rsp, resCodeResponse resCode) = 0;
			};
			struct IloginSdk
			{
				/*!
				  初始化sdk

				  @param  appid     业务申请的appid(需要业务自己去申请)
				  @param  pNotify   回调
				  @param  app_ver   业务app版本号
				  @param  region    国家码，两个字母长度，如“US”，参考：https://zh.wikipedia.org/wiki/ISO_3166-1 该参数只给servicesdk使用
				*/
				virtual int init(uint32_t appid,IloginSdkNotify*  pNotify, const char* app_ver, uint32_t app_verSize, const char* region, uint32_t regionSize) = 0;
				
				/*!
				初始化udbregion

				@param  region    国家码，两个字母长度，如“US”，参考：https://zh.wikipedia.org/wiki/ISO_3166-1
				*/
				virtual void setUdbRegion(const char* udbRegion, uint32_t udbRegionSize) = 0;

				/*!
				开启自动登录(是否保存credit的开关)

				@param  loginSwitch   true开启自动登录、false关闭、默认是开启
				*/
				virtual void isAutomaticLogin(bool loginSwitch) = 0;
				
				/*!
				发送短信

				@param  user      手机号带区号 0086XXX
				@param  sms_type  验证短信类型
				@param  region    国家码，两个字母长度，如“US”，参考：https://zh.wikipedia.org/wiki/ISO_3166-1 //收到的语音短信的语言、由region决定
				*/
				
				virtual bool smsRequest(const char* user, uint32_t userSize,const char* sms_type, uint32_t sms_typeSize,const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				发送短信

				@param  user      手机号带区号 0086XXX
				@param  sms_type  验证短信类型, 0：文本短信，1：语音短信,2:发两条短信。默认0
				@param  len       短信长度，"4"4位短信码，"6"6位短信码；填nil，默认为6位短信码
				@param  busiType  场景， 1：登录，2：注册，3：改密，4：找回密码，5：绑定手机，6：修改手机
				@param  otp       若是用于改密、绑定手机场景，需要传otp
				*/

				virtual bool smsRequestWithBusi(const char* user, uint32_t userSize, int smsType, int len, int busiType,
					const char* otp, uint32_t otpSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				退出登录

				@param  uid     用户uid、注册后服务器返回所得
				@param  context   上下文
				*/
				virtual int outSignin(uint64_t uid, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				手机号+验证码登录

				@param  user      手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
				@param  smscode   手机验证码
				*/
				virtual bool smsLoginRequest(const char* user, uint32_t userSize, const char* smscode, uint32_t smscodeSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				凭证登录
				@param  uid       用户uid、注册后服务器返回所得
				*/
				virtual bool creditLoginRequest(uint64_t uid, const char* context = NULL, uint32_t contextSize = 0) = 0;


				/*!
				凭证登录
				@param  uid       用户uid、注册后服务器返回所得
				@param  credit    凭证
				*/
				virtual bool creditLoginRequest2(uint64_t uid, const char* credit, uint32_t creditSize, const char* context = nullptr, uint32_t contextSize = 0) = 0;

				/*!
				密码登录

				@param  user      手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
				@param  sha1pwd   用户密码
				*/
				virtual bool passwordLoginRequest(const char* user, uint32_t userSize, const char* sha1pwd, uint32_t sha1pwdSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				密码注册
				@param  user      手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
				@param  smscode   手机验证码
				@param  sha1pwd   用户密码
				*/
				virtual bool SmsRegisterRequest(const char* user, uint32_t userSize,const char* smscode, uint32_t smscodeSize, const char* sha1pwd, uint32_t sha1pwdSize, const char* context = NULL, uint32_t contextSize = 0)=0;

				/*!
				修改密码
				@param  user      手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
				@param  smscode   手机验证码
				@param  sha1pwd   用户密码(新密码)
				*/
				virtual bool sModifyPwdRequest(const char* user, uint32_t userSize, const char* smscode, uint32_t smscodeSize, const char* sha1pwd, uint32_t sha1pwdSize, const char* context = NULL, uint32_t contextSize = 0) = 0;
              
				/*!
				第三方登录
				@param channel     登录类型,取值facebook,google,twitter,instagram,weibo,qq,wechat 注意要全部小写
				@param token       从第三方得到的code 或者 accesstoken
				@param thirdType   token为第三方的code时给0，为accesstoken时给1
				@param openid      第三方的UserId等，非必填
				@param authUrl     第三方认证的url,非必填
				@param extInfo     (预留参数)扩展信息参数，json格式
			   */
				virtual bool ThirdloginRspRequest(const char* channel, uint32_t channelSize,const char* token, uint32_t tokenSize,uint32_t tokentype, const char* openid, uint32_t openidSize,const char* authUrl, uint32_t authUrlSize, const char* extInfo, uint32_t extInfoSize,const char* context = NULL, uint32_t contextSize = 0) = 0;
				/*!
				确认手机号是否已注册
				
				@param  user      手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
				*/
				virtual bool MobileReqRequest(const char* user, uint32_t userSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/**
				* 获取邮箱验证码
				* @param user     邮箱
				* @param codeLen  验证码长度，默认是6
				* @param useType  验证码用途，2：注册，3：改密，4：找回密码，7：绑定邮箱，8：修改邮箱
				* @param dynCode  二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
				* @param context  上下文
				*/
				virtual bool GetEmailVerifyCodeRequest(const char* user, uint32_t userSize, int codeLen, int useType, const char* otp, uint32_t otpSize, const char* dynCode, uint32_t dynCodeSize,
					const char* context = NULL, uint32_t contextSize = 0) = 0;

				/**
				* 注册账号
				* @param user        账号(邮箱、手机号)
				* @param verifyCode  验证码
				* @param password    密码
				* @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
				* @param context     上下文
				*/
				virtual bool RegisterByVerifyCode(const char* user, uint32_t userSize, const char* verifyCode, uint32_t verifyCodeSize,
					const char* sha1pwd, uint32_t sha1pwdSize, const char* dynCode, uint32_t dynCodeSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/**
				* 修改密码
				* @param user        账号(邮箱、手机号)
				* @param verifyCode  验证码
				* @param password    密码
				* @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
				* @param context     上下文
				*/
				virtual bool ModifyPwdByVerifyCode(const char* user, uint32_t userSize, const char* verifyCode, uint32_t verifyCodeSize,
					const char* sha1pwd, uint32_t sha1pwdSize, const char* otp, uint32_t otpSize, const char* dynCode, uint32_t dynCodeSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/**
				* 验证邮箱验证码
				* @param user        邮箱
				* @param verifyCode  验证码
				* @param useType     验证码用途，1：登录，2：注册，3：改密,4:找回密码
				* @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
				* @param context     上下文
				*/
				virtual bool VerifyEmailCode(const char* user, uint32_t userSize, const char* verifyCode, uint32_t verifyCodeSize,
					int useType, const char* otp, uint32_t otpSize, const char* dynCode, uint32_t dynCodeSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/**
				* 验证短信验证码
				* @param user        手机号
				* @param verifyCode  验证码
				* @param useType     验证码用途，1：登录，2：注册，3：改密 4：找回密码
				* @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
				* @param otp         若是用于改密、绑定手机场景，需要传otp
				*/
				virtual bool VerifySMSCode(const char* user, uint32_t userSize, const char* verifyCode, uint32_t verifyCodeSize, int useType,
					const char* otp, uint32_t otpSize, const char* dynCode, uint32_t dynCodeSize, const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				获取OTP
                @param  otp 外面传过来用于填回的otp, 不能为NULL
                @return 返回值0 获取成功，其他值失败
				*/
                virtual int GetOTP(OTPInfo* otp) = 0;
				/*!
				获取OTP
				@param  appid  
                @param  otp 外面传过来用于填回的otp, 不能为NULL
                @return 返回值0 获取成功，其他值失败
				*/
                virtual int GetOTP(const char* appid, uint32_t appidSize, OTPInfo* otp) = 0;

				/*!
				获取二维码的id
				@param  context   上下文
				*/
				virtual bool QrCodeRequest(const char* context = NULL, uint32_t contextSize = 0) = 0;

				/*!
				尝试登陆，客户端控制频率，建议尝试间隔不能小于6秒
				@param  qrCodeId   二维码id
				@param  context    上下文
				*/
				virtual bool QrLoginRequest(const char* qrCodeId, uint32_t qrCodeIdSize, const char *context = NULL, uint32_t contextSize = 0) = 0;
			};
			__declspec(dllexport) IloginSdk * Init();
			__declspec(dllexport) void UnInit();
		}
	}
}


