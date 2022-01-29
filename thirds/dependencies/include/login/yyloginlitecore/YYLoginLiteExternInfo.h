//
//  YYLoginLiteExternInfo.h
//  yyloginlitesdk
//
//  Created by 陈开才 on 2019/9/5.
//  Copyright © 2019年 yy. All rights reserved.
//

#ifndef YYLoginLiteExternInfo_h
#define YYLoginLiteExternInfo_h

@class ProtoUserData;
@protocol YYLoginLiteLogHandler;

NS_ASSUME_NONNULL_BEGIN
@interface YYLoginLiteRiskConfig : NSObject

/// 外挂检测业务申请的名称，找唐志鹏申请，必填
@property (nonatomic, copy) NSString *bizName;
/// 是否是国内版，安全sdk的上报区分国内外的路由，默认NO，必填
@property (nonatomic, assign) BOOL isChineseService;
/// 风控开关，是否支持二次认证的交互式，默认YES，选填
@property (nonatomic, assign) BOOL isCapSupport;
/// 是否由sdk实现二次验证的显示和操作逻辑，默认YES，选填
@property (nonatomic, assign) BOOL isNextVerInSDK;



/// 根据bizName初始化风控配置，isCapSupport=YES，isNextVerInSDK=YES
/// @param bizName 外挂检测业务申请的名称
/// @param isChineseService 是否是国内版，安全sdk的上报区分国内外的路由
+ (instancetype)defaultConfigWithBizName:(NSString *)bizName isChineseService:(BOOL)isChineseService;

@end

@interface YYLoginAnonymousInfo: NSObject

@property(nonatomic) NSInteger uid;
/** 用户号码，对应yy体系里面的yy号 */
@property(nonatomic) NSInteger uno;
/** 通行证，对应yy体系passport */
@property(nonatomic, copy) NSString *username;
/** */
@property(nonatomic, copy) NSString *password;
/** 返回的cookie */
@property(nonatomic, copy) NSData *cookie;
@property(nonatomic, copy) NSDictionary<NSString*, NSString*> *extmap;

@end

@interface BindThirdUser : NSObject

@property (nonatomic, strong) NSString *secret;
@property (nonatomic, strong) NSString *openid;
@property (nonatomic, strong) NSString *url;

@end

@interface YYLoginUserAuthInfo : NSObject
/// 用户id，注册/登录成功后返回
@property(nonatomic, assign) UInt64 uid;
/// 是否为新用户
@property(nonatomic, assign) BOOL isNewUser;
/// 用户号码，注册/登录成功后返回（类似YY
@property(nonatomic, assign) UInt64 userNum;
/// 预留字段，登录成功后打开一个页面。
@property(nonatomic, copy) NSString *url;
/// 登录时的时间戳
@property(nonatomic, assign) NSTimeInterval timestamp;
/// 扩展json串，如{"region":"br"}
@property(nonatomic, strong) NSString* _Nullable ext;
/// 凭证
@property(nonatomic, copy) NSString *credit;
/// h5登录service的凭证
@property(nonatomic, copy) NSString *signapotp;
/// web的凭证
@property(nonatomic, copy) NSString *accesstoken;
/// 用户手机掩码
@property(nonatomic, copy) NSString *mobilemask;
/// 第三方渠道{facebook,google,twitter,instagram,weibo,qq,wechat}
@property(nonatomic, copy) NSString *channel;
/**
* 第三方返回的帐号信息，json串，有如下固定的字段名:
* {
* "openid": "XXXXXXXXXXXXXXXX",
* "unionid": "XXXXXXXX",
* "nickname": "Roven",
* "sex": "男",
* "avatar": "http://thirdqq.qlogo.cn/qqapp/XXXXX",
* "accessToken": "XXXXXXXXXXXXXXX"
* }
*/
@property(nonatomic, copy) NSString *thirdInfo;
@property(nonatomic, copy) NSDictionary<NSString*, NSString*> *extmap;
@property(nonatomic, copy) NSString *extjsstr;
@property(nonatomic, copy) NSString *regRegion;

/// 解析ProtoUserData
+ (void)unpackUserdata: (ProtoUserData *)userdata info: (YYLoginUserAuthInfo *)info;
/// 解析字典
+ (void)unpackDic: (NSDictionary *)dic info: (YYLoginUserAuthInfo *)info;
@end

#endif /* YYLoginLiteExternInfo_h */
NS_ASSUME_NONNULL_END
