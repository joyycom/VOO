//
//  YYLoginLiteSDK.h
//  yybasenetsdk
//
//  Created by CHEN CHENGBIN on 2018/4/8.
//  Copyright © 2018年 yy. All rights reserved.
//
/*!
 @header YYLoginLiteSDK.h
 @abstract 登录库头文件
 */

#import <Foundation/Foundation.h>
#import "YYLoginLiteExternInfo.h"
#import "YYLoginLiteConstantDefine.h"
@class RiskControlChallengeManager;
/// subAppID的设置Key，NSString
extern NSString * _Nonnull const kYYLoginSubAppIDKey;
/// 数美的deviceid的设置Key，NSString
extern NSString * _Nonnull const kYYLoginSkyNetDeviceIdKey;
/// 三方互联接口的ext的设置key，NSString，传@"true" or @"false"
extern NSString * _Nonnull const kYYLoginCapsupport;
/// 设置绑定登录手机 或 密保手机，NSString，传@"lgn" or @"mibao"
extern NSString * _Nonnull const kYYLoginBindMobileType;

/*!
 回调接口
 */
@protocol YYLoginLiteLogHandler
- (void) onLoginLog: (int)level msg:(nonnull NSString *)msg;
@end

/*!
 登录sdk入口
 */
@interface YYLoginLiteSDK : NSObject

#if TARGET_OS_IPHONE
/// 风控sdk
@property (nonatomic, strong, readonly) RiskControlChallengeManager * _Nullable challengeManager;
#endif

/*!
 当前的请求通道
*/
@property (nonatomic, readonly) YYLOGIN_REQUEST_TYPE requestType;

+ (nonnull YYLoginLiteSDK *)sharedInstance;

/*!
 初始化sdk，推荐以该方式初始化
 @param appid 业务申请的appid
 @param version app的版本号,业务自定义
 @param langCode  zh-cn(简中)，en-us（英语(美国)）,en-id（英语）,id-id（英语(印度尼西亚)），hi-in（印度）具体语言代码，参考：https://blog.csdn.net/xiaozhu54321/article/details/48196301
 @param config 风控配置参数，详细字段请查看 YYLoginLiteRiskConfig
 @param handler 日志log回调
 @param ext 扩展字段
*/
- (void)initWithAppId:(nonnull NSString *)appid
               appver:(nonnull NSString *)version
             langCode:(nullable NSString *)langCode
           riskConfig:(nonnull YYLoginLiteRiskConfig *)config
           logHandler:(nonnull id<YYLoginLiteLogHandler>)handler
                  ext:(nullable NSDictionary *)ext;

/**
 提供初始化外的方式，设置ext
 */
- (void)setupExt:(nonnull NSDictionary *)ext;


/**
 设置请求的通道，该接口仅供引入pod yyloginlitesdk/all 使用
 @param requestType YYLOGIN_REQUEST_TYPE类型，取值YYLOGIN_REQUEST_HTTP，YYLOGIN_REQUEST_SERVICE
 */
- (void)setupRequestType:(YYLOGIN_REQUEST_TYPE)requestType;

/*!
 获取短信验证码

 @param phoneNum 手机号
 @param type  0:文字短信、1:语音短信、2:发两次文字短信
 @param len   短信长度，"4"4位短信码，"6"6位短信码；填nil，默认为6位短信码
 @param dynCode 二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 @param onCallBackBlock 回调,error Domain为SDKLoginErrorDomain值:
            1)YYLOGIN_NEXT_VER = 1, //二次验证
            2)YYLOGIN_VCODE_ERR = 13,  //二次验证码错误
            需要拿dynUrl进行二次验证
 */
- (void) getSms:(nonnull NSString *)phoneNum
       smstype:(YYLOGIN_SMS_TYPE)type
     smsLength:(nullable NSString *)len
       dynCode:(nullable NSString *)dynCode
    onCallBack:(nonnull void(^)(NSError * _Nullable error, NSString * _Nullable dynUrl))onCallBackBlock;

/*!
 获取短信验证码，带场景的情况
 
 @param phoneNum 手机号
 @param type     短信类型，0：文本短信，1：语音短信,2:发两条短信。默认0
 @param len      短信长度，"4"4位短信码，"6"6位短信码；填nil，默认为6位短信码
 @param useType  场景， 1：登录，2：注册，3：改密，4：找回密码，5：绑定手机，6：修改手机
 @param otp      若是用于改密、绑定手机场景，需要传otp
 @param dynCode  二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 @param success  获取短信成功的回调
 @param failed   获取短信失败的回调
 */
- (void)getSmsWithBusiType:(nonnull NSString *)phoneNum
                    smstype:(YYLOGIN_SMS_TYPE)type
                  smsLength:(nullable NSString *)len
                    useType:(nonnull NSNumber *)useType
                        otp:(nullable NSString *)otp
                    dynCode:(nullable NSString *)dynCode
                   success:(nonnull void(^)(NSNumber * _Nullable errCode))success
                    failed:(nonnull void(^)(NSNumber * _Nullable errCode, NSString * _Nullable description, NSError * _Nullable error))failed;

  
/*!
 短信验证

 @param phoneNum 手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
 @param smsCode 短信验证码
 @param dynCode 二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 @param onCallBackBlock 回调,error Domain为SDKLoginErrorDomain值:
            1)YYLOGIN_NEXT_VER = 1, //二次验证
            2)YYLOGIN_VCODE_ERR = 13,  //二次验证码错误
            需要拿dynUrl进行二次验证
 */
- (void) smsLogin:(nonnull NSString *)phoneNum
         smsCode:(nonnull NSString *)smsCode
         dynCode:(nullable NSString *)dynCode
      onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo,
                                  NSError * _Nullable error,
                                  NSString * _Nullable dynUrl))onCallBackBlock;

/*!
短信验证

@param phoneNum 手机号(中国 0086+手机号; 美国 001+手机号; 印尼 0062+手机号)
@param smsCode 短信验证码
@param dynCode 二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
@param otp 游客登录后的otp，可以为""
@param onCallBackBlock 回调,error Domain为SDKLoginErrorDomain值:
           1)YYLOGIN_NEXT_VER = 1, //二次验证
           2)YYLOGIN_VCODE_ERR = 13,  //二次验证码错误
           需要拿dynUrl进行二次验证
*/
- (void)smsLogin:(nonnull NSString *)phoneNum
         smsCode:(nonnull NSString *)smsCode
         dynCode:(nullable NSString *)dynCode
             otp:(nullable NSString *)otp
      onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo,
                            NSError * _Nullable error,
                            NSString * _Nullable dynUrl))onCallBackBlock;

/*!
 账号密码登录
 
 @param account   账号
 @param pwd       密码
 @param dynCode   二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 @param success   成功回调,error Domain为SDKLoginErrorDomain值:
                1)YYLOGIN_NEXT_VER = 1, //二次验证
                2)YYLOGIN_VCODE_ERR = 13,  //二次验证码错误
                需要拿dynUrl进行二次验证
 @param failure   失败回调
 */
- (void) passwordLogin:(nonnull NSString *)account
             password:(nonnull NSString *)pwd
              dynCode:(nullable NSString *)dynCode
              success:(nonnull void(^)(NSNumber * _Nonnull errCode, NSString * _Nullable description,
                                       NSString * _Nullable dynUrl,
                                       YYLoginUserAuthInfo * _Nullable authinfo))success
              failure:(nonnull void(^)(NSNumber * _Nonnull sdkErrCode, NSNumber * _Nonnull srvErrCode, NSError * _Nullable error))failure;

/*!
 账号密码登录
 
 @param account   账号
 @param pwd       密码
 @param isEncryption 标记传入的pwd是否是已加密的，YES：SDK内部不加密pwd，NO：sdk内部加密pwd
 @param isBindMobile   是否需要绑手机，默认为NO，传YES，会去检测account有没绑定手机，没有绑手机会返回一个h5，该页面的显示是sdk，还是业务实现，
                    由初始化的YYLoginLiteRiskConfig的isNextVerInSDK决定
 @param dynCode   二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 @param success   成功回调,error Domain为SDKLoginErrorDomain值:
                1)YYLOGIN_NEXT_VER = 1, //二次验证/绑手机
                2)YYLOGIN_VCODE_ERR = 13,  //二次验证码错误
                需要拿dynUrl进行二次验证/绑手机，dynType取值有js和url，用于标识dynUrl的类型
 @param failure   失败回调
 */
- (void) passwordLogin:(nonnull NSString *)account
             password:(nonnull NSString *)pwd
            encryption:(BOOL)isEncryption
            bindMobile:(BOOL)isBindMobile
              dynCode:(nullable NSString *)dynCode
              success:(nonnull void(^)(NSNumber * _Nonnull errCode, NSString * _Nullable description,
                                       NSString * _Nullable dynUrl, NSString * _Nullable dynType,
                                       YYLoginUserAuthInfo * _Nullable authinfo))success
              failure:(nonnull void(^)(NSNumber * _Nonnull sdkErrCode, NSNumber * _Nonnull srvErrCode, NSError * _Nullable error))failure;

/*!
 凭证登录
 注意:登录成功需要更新credit
 @param uid 用户uid
 @param onCallBackBlock 回调
 */
- (void) creditLogin:(UInt64)uid
         onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallBackBlock;

/*!
 凭证登录
 注意:登录成功需要更新credit
 @param uid 用户uid
 @param credit 凭证
 @param onCallBackBlock 回调
 */
- (void) creditLogin:(UInt64)uid
             credit:(nonnull NSString *)credit
         onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallBackBlock;

/*!
 第三方登录

 @param thirdType 登录类型,取值weibo,qq,wechat,facebook,google,twitter,instagram,vk,twitch,whatsapp,line,reddit,snapchat,apple,kakao注意要全部小写
 @param token 从第三方得到的code 或者 accesstoken
 @param type token为第三方的code时给0，为accesstoken时给1
 @param openid 第三方的UserId等，非必填
 @param url 第三方认证的url,非必填
 @param onCallBackBlock 回调
 */
- (void) thirdLogin:(nonnull NSString *)thirdType
             token:(nonnull NSString *)token
         tokenType:(YYLOGIN_THIRD_TOKEN_TYPE)type
       tokenSecret:(nullable NSString *)secret
            openid:(nullable NSString *)openid
           authUrl:(nullable NSString *)url
        onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallBackBlock;

/*!
 第三方登录

 @param thirdType 登录类型,取值weibo,qq,wechat,facebook,google,twitter,instagram,vk,twitch,whatsapp,line,reddit,snapchat,apple,kakao注意要全部小写
 @param token 从第三方得到的code 或者 accesstoken
 @param type token为第三方的code时给0，为accesstoken时给1
 @param openid 第三方的UserId等，非必填
 @param url 第三方认证的url,非必填
 @param isBindMobile 是否自动绑定手机，非必填
 @param onCallBackBlock 回调
 */
- (void) thirdLogin:(nonnull NSString *)thirdType
             token:(nonnull NSString *)token
         tokenType:(YYLOGIN_THIRD_TOKEN_TYPE)type
       tokenSecret:(nullable NSString *)secret
            openid:(nullable NSString *)openid
           authUrl:(nullable NSString *)url
         bindMobile:(BOOL)isBindMobile
        onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallBackBlock;

/*!
第三方登录

@param thirdType 登录类型,取值weibo,qq,wechat,facebook,google,twitter,instagram,vk,twitch,whatsapp,line,reddit,snapchat,apple,kakao注意要全部小写
@param token 从第三方得到的code 或者 accesstoken
@param type token为第三方的code时给0，为accesstoken时给1
@param openid 第三方的UserId等，非必填
@param url 第三方认证的url,非必填
@param isBindMobile 是否自动绑定手机，非必填
@param verifyViewEnable 绑定手机或二次验证的H5，由SDK显示页面，非必填
@param onCallBackBlock 回调
*/
- (void) thirdLogin:(nonnull NSString *)thirdType
              token:(nonnull NSString *)token
          tokenType:(YYLOGIN_THIRD_TOKEN_TYPE)type
        tokenSecret:(nullable NSString *)secret
             openid:(nullable NSString *)openid
            authUrl:(nullable NSString *)url
         bindMobile:(BOOL)isBindMobile
   verifyViewEnable: (BOOL)verifyViewEnable
         onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallBackBlock;


/*!
第三方登录

@param thirdType 登录类型,取值weibo,qq,wechat,facebook,google,twitter,instagram,vk,twitch,whatsapp,line,reddit,snapchat,apple,kakao注意要全部小写
@param token 从第三方得到的code 或者 accesstoken
@param type token为第三方的code时给0，为accesstoken时给1
@param openid 第三方的UserId等，非必填
@param url 第三方认证的url,非必填
@param isBindMobile 是否自动绑定手机，非必填
@param verifyViewEnable 绑定手机或二次验证的H5，由SDK显示页面，非必填
@param ext 扩展dic，数美的deviceId 由kYYLoginSkyNetDeviceIdKey设置，风控开关由kYYLoginCapsupport设置
@param onCallBackBlock 回调
*/
- (void) thirdLogin:(nonnull NSString *)thirdType
              token:(nonnull NSString *)token
          tokenType:(YYLOGIN_THIRD_TOKEN_TYPE)type
        tokenSecret:(nullable NSString *)secret
             openid:(nullable NSString *)openid
            authUrl:(nullable NSString *)url
         bindMobile:(BOOL)isBindMobile
   verifyViewEnable: (BOOL)verifyViewEnable
                ext:(nullable NSDictionary *)ext
 onCallBack:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallBackBlock;

/*!
 绑定第三方token
 
 @param thirdType 登录类型,取值weibo,qq,wechat,facebook,google,twitter,instagram,vk,twitch,whatsapp,line,reddit,snapchat,apple,kakao注意要全部小写
 @param token 从第三方得到的code 或者 accesstoken
 @param type token为第三方的code时给0，为accesstoken时给1
 @param uid  用户id
 @param optToken 票据
 @param info 第三方扩展属性
 @param success  成功的回调
 @param failure  失败的回调
 */
- (void) bindThirdUserByUid:(nonnull NSString *)thirdType
                     token:(nonnull NSString *)token
                 tokenType:(YYLOGIN_THIRD_TOKEN_TYPE)type
                       uid:(nonnull NSString *)uid
                  optToken:(nonnull NSString *)optToken
                externInfo:(nullable BindThirdUser *)info
                    success:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSNumber * _Nullable errCode))success
                   failure:(nonnull void(^)(NSError * _Nullable error, NSNumber * _Nullable sdkErrCode, NSNumber * _Nullable srvErrCode))failure;


/*!
（激光）一键绑定手机

@param thirdType 第三方渠道，目前只有 china-jiguang
@param token 从第三方平台token，例如极光的token
@param otp 票据
@param thirdAppid 第三方appid（clientid）
@param uid  用户id
@param userIp 用户ip
@param region 区域编码，如中国填写zh-cn
@param ext 扩展字段，可不传
@param onCallback  结果回调，rescode 0 绑定成功；  1 当前uid已经绑定手机 ； 2 手机已经绑定另一个uid；其他返回码失败，resMsg 结果描述
*/
- (void)quickBindMobile:(nonnull NSString *)thirdType
                  token:(nonnull NSString *)token
                    otp:(nonnull NSString *)otp
             thirdAppid:(nonnull NSString *)thirdAppid
                    uid:(UInt64)uid
                 userIp:(nonnull NSString *)userIp
                 region:(nonnull NSString *)region
                    ext:(nullable NSDictionary *)ext
             onCallback:(nonnull void (^)(NSInteger rescode, NSString * _Nonnull resMsg))onCallback;


/*!
（激光）一键绑定&登录(注册)，场景：用于三方登录设置强绑，这时候要拿到三方登录接口的stoken调用该接口绑定

@param thirdType 第三方渠道，目前只有 china-jiguang
@param stoken 三方登录thirdLogin的接口获取stoken
@param token 从第三方平台token，例如极光的token
@param thirdAppid 第三方appid（例如极光appid）
@param region 区域编码，如中国，填写zh-CN
@param userIp 用户ip
@param subappid 分配了subappid的业务才需要填，可不传
@param ext 扩展字段，可不传
@param onCallback  结果回调，authinfo用户信息，error.code返回码，0表示成功，其他失败，需使用H5绑定手机
*/
- (void)qucikBindLogin:(nonnull NSString *)thirdType
                stoken:(nonnull NSString *)stoken
                 token:(nonnull NSString *)token
            thirdAppid:(nonnull NSString *)thirdAppid
                region:(nonnull NSString *)region
                userIp:(nonnull NSString *)userIp
              subappid:(nullable NSString *)subappid
                   ext:(nullable NSDictionary *)ext
            onCallback:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSError * _Nullable error))onCallback;

/*!
匿名登录

@param terminalType 终端类型
@param onCallback  结果回调，rescode取值说明：0 成功，1 获取uid失败，一般外挂才会，2 ip登录数超限，99 系统错误， info 匿名成功后的信息
*/
- (void)anonymousLogin:(NSInteger)terminalType
            onCallback:(nonnull void(^)(NSInteger rescode, YYLoginAnonymousInfo * _Nullable info))onCallback;

/*!
  扫码授权
 
 @param user     帐号,uid
 @param qrcodeId 二维码id，扫码后从二维码图片中获取qrcode_id
 @param success  成功后的回调
 @param failed   失败后的回调
 */
- (void) qrScanReq:(nonnull NSString *)user
         qrcodeId:(nonnull NSString *)qrcodeId
          success:(nonnull void(^)(int errCode, NSString * _Nullable description, NSString * _Nullable appInfo))success
           failed:(nonnull void(^)(int sdkResCode, int srvResCode, NSError * _Nullable e))failed;

/*!
 点击确认，给显码方登录
 
 @param user     帐号,uid
 @param qrcodeId 二维码id，扫码后从二维码图片中获取qrcode_id
 @param opt      一次性使用凭证
 @param success  成功后的回调
 @param failed   失败后的回调
 */
- (void) qrcodeConfirmReq:(nonnull NSString *)user
                qrcodeId:(nonnull NSString *)qrcodeId
                     opt:(nonnull NSString *)opt
                 success:(nonnull void(^)(int errCode, NSString * _Nullable description))success
                  failed:(nonnull void(^)(int sdkResCode, int srvResCode, NSError * _Nullable error))failed;

/*!
 点击取消，取消扫码操作
 
 @param user     帐号,uid
 @param qrcodeId 二维码id，扫码后从二维码图片中获取qrcode_id
 @param success  成功后的回调
 @param failed   失败后的回调
 */
- (void) qrcodeCancelReq:(nonnull NSString *)user
               qrcodeId:(nonnull NSString *)qrcodeId
                success:(nonnull void(^)(int errCode, NSString * _Nullable description))success
                 failed:(nonnull void(^)(int sdkResCode, int srvResCode, NSError * _Nullable error))failed;


/**
 * 第三方登录后绑定手机号
 * @param phoneNum
 *          手机号码
 * @param user
 *          UID
 * @param smsCode
 *          短信验证码
 * @param onCallBackBlock
 *          手机绑定回调
 */
- (void) bindMobile:(nonnull NSString *)phoneNum
              user:(nonnull NSString *)user
           smsCode:(nonnull NSString *)smsCode
        onCallBack:(nonnull void(^)(NSError * _Nullable error))onCallBackBlock;

/**
 * 获取邮箱验证码
 * @param email    邮箱
 * @param codeLen  验证码长度，默认是6
 * @param useType  验证码用途，2：注册，3：改密，4：找回密码，7：绑定邮箱，8：修改邮箱
 * @param otp      若是用于改密、绑定邮箱场景，需要传otp
 * @param dynCode  二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 * @param success  获取验证成功的回调
 * @param failed   获取验证失败的回调
 */
- (void)getEmailVerifyCode:(nonnull NSString *)email
                   codeLen:(nonnull NSString *)codeLen
                   useType:(nonnull NSNumber *)useType
                       otp:(nullable NSString *)otp
                   dynCode:(nullable NSString *)dynCode
                   success:(nonnull void(^)(NSNumber * _Nonnull errCode))success
                    failed:(nonnull void(^)(NSNumber * _Nonnull errCode, NSString * _Nonnull description, NSError * _Nonnull error))failed;

/**
 * 验证邮箱验证码
 * @param email       邮箱
 * @param verifyCode  验证码
 * @param useType     验证码用途，2：注册，3：改密，4：找回密码
 * @param otp         若是用于改密、绑定邮箱场景，需要传otp
 * @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 * @param success     验证邮箱验证码成功的回调
 * @param failed      验证邮箱验证码失败的回调
 */
- (void)verifyEmailCode:(nonnull NSString *)email
             verifyCode:(nonnull NSString *)verifyCode
                useType:(nonnull NSNumber *)useType
                    otp:(nullable NSString *)otp
                dynCode:(nullable NSString *)dynCode
                success:(nonnull void(^)(NSNumber * _Nullable errCode))success
                 failed:(nonnull void(^)(NSNumber * _Nullable errCode, NSString * _Nullable description, NSError * _Nullable error))failed;

/**
 * 验证短信验证码
 * @param phoneNum    手机号
 * @param verifyCode  验证码
 * @param useType     验证码用途，1：登录，2：注册，3：改密 4：找回密码
 * @param otp         若是用于改密、绑定手机场景，需要传otp
 * @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 * @param success     验证短信验证码成功的回调
 * @param failed      验证短信验证码失败的回调
 */
- (void)verifySMSCode:(nonnull NSString *)phoneNum
           verifyCode:(nonnull NSString *)verifyCode
              useType:(nonnull NSNumber *)useType
                  otp:(nullable NSString *)otp
              dynCode:(nullable NSString *)dynCode
              success:(nonnull void(^)(NSNumber * _Nullable errCode))success
               failed:(nonnull void(^)(NSNumber * _Nullable errCode, NSString * _Nullable description, NSError * _Nullable error))failed;


/**
 * 注册账号
 * @param account     账号(邮箱、手机号)
 * @param verifyCode  验证码
 * @param password    密码
 * @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 * @param success     注册账号成功的回调
 * @param failed      注册账号失败的回调
 */
- (void)registerByVerifyCode:(nonnull NSString *)account
                  verifyCode:(nonnull NSString *)verifyCode
                     password:(nullable NSString *)password
                     dynCode:(nullable NSString *)dynCode
                     success:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable userInfo, NSNumber * _Nullable errCode))success
                      failed:(nonnull void(^)(NSNumber * _Nullable errCode, NSString * _Nullable description, NSError * _Nullable error))failed;

/**
 * 通行证注册
 * @param passport    通行证，用户自定义的字段
 * @param account     账号(邮箱、手机号)，可不传，如果传了就会绑定到这个通行证
 * @param verifyCode  验证码，可不传，如果传了就会绑定到这个通行证
 * @param password    密码
 * @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 * @param success     注册账号成功的回调
 * @param failed      注册账号失败的回调
 */
- (void)registerPassport:(nonnull NSString *)passport
                 account:(nullable NSString *)account
              verifyCode:(nullable NSString *)verifyCode
                password:(nonnull NSString *)password
                 dynCode:(nullable NSString *)dynCode
                 success:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable userInfo, NSNumber * _Nullable errCode))success
                  failed:(nonnull void(^)(NSNumber * _Nullable errCode, NSString * _Nullable description, NSError * _Nullable error))failed;

/**
 * 修改密码，找回密码
 * @param account     账号(邮箱、手机号)
 * @param verifyCode  验证码
 * @param password    密码
 * @param otp         找回密码不用otp，改密需要登录态要otp
 * @param dynCode     二次验证的验证码，首次请求或者没有启用二次验证策略的则给""
 * @param success     修改密码成功的回调
 * @param failed      修改密码失败的回调
 */
- (void)modifyPwdByVerifyCode:(nonnull NSString *)account
                   verifyCode:(nonnull NSString *)verifyCode
                      password:(nonnull NSString *)password
                           otp:(nullable NSString *)otp
                      dynCode:(nullable NSString *)dynCode
                      success:(nonnull void(^)(NSNumber * _Nullable errCode))success
                       failed:(nonnull void(^)(NSNumber * _Nullable errCode, NSString * _Nullable description, NSError * _Nullable error))failed;

/**
* 请求外挂检查，需要登录后才能调用
*/
- (void)requestCheckAnti;

/// http通道是否开启测试环境，仅供测试使用
- (void)setTest:(BOOL)isTest;

/// 设置反外挂的重试策略，需要传一个NSNumber的数组，单位为秒，例如3次重试，间隔10秒，retryArray=@[@10, @10, @10]
/// 若不设置，默认是3次重试，间隔10秒
- (void)setAntiRetryStrategy:(nonnull NSArray *)retryArray;

/**
 获取用于登录service/信令的token

 @return 返回获取用于登录service/信令的token
 */
- (nullable NSString *)getServiceToken;


/**
 获取业务自身的otp凭证，如果要用于登录service/信令，请用getServiceToken接口
 
 注意:票据有效时间较短,每次使用都必须重新调用这个接口获取
 @return 获取业务自身的otp凭证
*/
- (nullable NSString *)getBusinessOtpToken;

/**
 获取业务自身的otp凭证，如果要用于登录service/信令，请用getServiceToken接口，该接口与getBusinessOtpToken是等价的
 
 注意:票据有效时间较短,每次使用都必须重新调用这个接口获取
 @param uid 业务方管理的uid，根据该uid查找对应的credit去生成票据
 @return 获取业务自身的otp凭证
*/
- (nullable NSString *)getBusinessOtpToken:(UInt64)uid;


/**
 获取跨业务访问的otp凭证，
 
 @param desAppid 跨业务的appid，例如IM是yyim，信令/service传signap
 @return 获取跨业务访问的otp凭证
*/
- (nullable NSString *)getAcrossBusinessAcessOtpToken:(nonnull NSString *)desAppid;

/*!
 设置地区码
 @param langeCode 地区码
 
 */
- (void) setlangCode:(nonnull NSString *)langeCode;

/**
 根据uid获取本地保存的credit
 */
- (nullable NSString *)getCredit: (UInt64)uid;

/*!
 退出登录
 */
- (void) logout;

/**
 * 退出当前登录的UID，退出后所有OTP失效，如有需要则重新登录
 * 注意调用此接口后也需要调用ServiceSDK的unBind，保证登录相关的逻辑正确
 * @param enableAutoLoginNext
 *          是否允许下次可以进行creditLogin 自动登录，
 *          此参数给false，则下次登录必须要用手机号或第三方或者密码重登了，给true只是退出本次登录
 */
- (void) logout:(BOOL)enableAutoLoginNext;

/**
 * 获取uid
 */
- (UInt64) getUid;

/**
 * 获取bizName;
 */
- (nullable NSString *) getBizName;


/**
 * 是否是国内服务器
 */
- (BOOL) isChineseService;

#pragma mark - 废弃的接口
/*!
 短信注册，已废弃，请使用modifyPwdByVerifyCode
 
 @param account   账号
 @param smsCode   短信验证码
 @param pwd  密码
 @param success   成功回调; resCode的枚举值参考YYREGISTER_ERROR_CODE
 @param failure   失败回调
 */
- (void) smsRegister:(nonnull NSString *)account
            smsCode:(nonnull NSString *)smsCode
           password:(nullable NSString *)pwd
            success:(nonnull void(^)(YYLoginUserAuthInfo * _Nullable authinfo, NSNumber * _Nonnull resCode, NSString * _Nullable description))success
            failure:(nonnull void(^)(NSNumber * _Nonnull sdkErrCode, NSNumber * _Nonnull srvErrCode, NSError * _Nullable error))failure __attribute__((deprecated("use `registerByVerifyCode:` instead.")));

/*!
 短信改密，已废弃，请使用modifyPwdByVerifyCode
 
 @param phoneNum   账号
 @param smsCode   短信验证码
 @param pwd  密码
 @param success   成功回调; errCode枚举值参考YYREGISTER_ERROR_CODE
 @param failure   失败回调
 */
- (void) smsModifyPwd:(nonnull NSString *)phoneNum
             smsCode:(nonnull NSString *)smsCode
            password:(nonnull NSString *)pwd
             success:(nonnull void(^)(NSNumber * _Nonnull errCode, NSString * _Nullable description))success
             failure:(nonnull void(^)(NSNumber * _Nonnull sdkErrCode, NSNumber * _Nonnull srvErrCode, NSError * _Nullable error))failure __attribute__((deprecated("use `modifyPwdByVerifyCode:` instead.")));

/*!
 初始化sdk，不推荐使用，推荐使用initWithAppId

 @param appid 业务申请的appid
 @param version app的版本号,业务自定义
 @param langCode  zh-cn(简中)，en-us（英语(美国)）,en-id（英语）,id-id（英语(印度尼西亚)），hi-in（印度）具体语言代码，参考：https://blog.csdn.net/xiaozhu54321/article/details/48196301
 */
- (void) initSDK:(nonnull NSString *)appid
         appver:(nonnull NSString *)version
       langCode:(nullable NSString *)langCode
     logHandler:(nonnull id<YYLoginLiteLogHandler>)handler __attribute__((deprecated("use `initWithAppId:` instead.")));


/*!
 初始化sdk，不推荐使用，推荐使用initWithAppId
 @param appid 业务申请的appid
 @param version app的版本号,业务自定义
@param langCode  zh-cn(简中)，en-us（英语(美国)）,en-id（英语）,id-id（英语(印度尼西亚)），hi-in（印度）具体语言代码，参考：https://blog.csdn.net/xiaozhu54321/article/details/48196301
 @param bizName 外挂检测业务申请的名称
 @param isChineseService 是否是国内版
*/
- (void) initSDK:(nonnull NSString *)appid
          appver:(nonnull NSString *)version
        langCode:(nullable NSString *)langCode
         bizName:(nonnull NSString *)bizName
isChineseService:(BOOL)isChineseService
      logHandler:(nonnull id<YYLoginLiteLogHandler>)handler __attribute__((deprecated("use `initWithAppId:` instead.")));

/*!
 初始化sdk，不推荐使用，推荐使用initWithAppId
 @param appid 业务申请的appid
 @param version app的版本号,业务自定义
 @param langCode  zh-cn(简中)，en-us（英语(美国)）,en-id（英语）,id-id（英语(印度尼西亚)），hi-in（印度）具体语言代码，参考：https://blog.csdn.net/xiaozhu54321/article/details/48196301
 @param bizName 外挂检测业务申请的名称
 @param isChineseService 是否是国内版
 @param isCapSupport 风控开关，是否支持二次认证的交互式
 @param isNextVerInSDK 是否由sdk实现二次验证的显示和操作逻辑
*/
- (void) initSDK:(nonnull NSString *)appid
          appver:(nonnull NSString *)version
        langCode:(nullable NSString *)langCode
         bizName:(nonnull NSString *)bizName
isChineseService:(BOOL)isChineseService
isCapSupport:(BOOL)isCapSupport
isNextVerInSDK:(BOOL)isNextVerInSDK
      logHandler:(nonnull id<YYLoginLiteLogHandler>)handler __attribute__((deprecated("use `initWithAppId:` instead.")));

/*!
 获取业务自身的otp凭证，如果要用于登录service/信令，请用getServiceToken接口
 
 注意:票据有效时间较短,每次使用都必须重新调用这个接口获取
 @return 获取业务自身的otp凭证
 */
- (nullable NSString *)getOpt  __attribute__((deprecated("use `getBusinessOtpToken` instead.")));

/*!
 获取业务自身的otp凭证，如果要用于登录service/信令，请用getServiceToken接口
 
 注意:票据有效时间较短,每次使用都必须重新调用这个接口获取
 @param uid 业务方管理的uid，根据该uid查找对应的credit去生成票据
 @return 票据
*/
- (nullable NSString *)getOtpWithUid:(UInt64)uid  __attribute__((deprecated("use `getBusinessOtpToken:` instead.")));;

/*!
 获取跨业务访问的otp票据
 
 @param desAppid 要跨业务的那个业务的appID
 @return 跨业务访问的otp票据
 */
- (nullable NSString *)getOpt:(nonnull NSString *)desAppid __attribute__((deprecated("use `getAcrossBusinessAcessOtpToken:` instead.")));;


/*!
 获取跨业务访问的otp票据，暂无业务使用，若用到该接口，请联系叶宏业
 注意:票据有效时间较短,每次使用都必须重新调用这个接口获取
 @param uid 业务方管理的uid，根据该uid查找对应的credit去生成票据
 @param desAppid 要跨业务的那个业务的appID
 @return 票据
*/
- (nullable NSString *)getOtpWithUid:(UInt64)uid appId:(nonnull NSString *)desAppid __attribute__((deprecated("该接口用于获取跨业务的otp票据，目前应该没有业务使用，若要使用，请联系叶宏业")));;



@end
