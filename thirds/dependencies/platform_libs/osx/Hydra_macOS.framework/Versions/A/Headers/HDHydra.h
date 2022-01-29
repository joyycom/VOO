//
//  HDHydra.h
//  HydraDemo
//
//  Created by Pluto Y on 2020/3/22.
//  Copyright © 2020 service. All rights reserved.
//

#if __OBJC__
    #import <Foundation/Foundation.h>
#endif

/**
 * Make global functions usable in C++
 */
#ifdef __cplusplus
#define HD_EXTERN extern "C" __attribute__((visibility("default")))
#else
#define HD_EXTERN extern __attribute__((visibility("default")))
#endif

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, HDState) {
    HDStateUnavailable  = 0,
    HDStateConnecting   = 1,
    HDStateAnonymous    = 2,
    HDStateLoggingIn    = 3,
    HDStateLoggedIn     = 4
};

typedef NS_ENUM(NSUInteger, HDConnectionState) {
    HDConnectionStateDisconnected   = 0,
    HDConnectionStateConnecting     = 1,
    HDConnectionStateConnected      = 2,
};

typedef NS_ENUM(NSUInteger, HDTokenType) {
    HDTokenTypeOverseas = 0,
    HDTokenTypeSunCloud = 1,
    HDTokenTypeDemostic = 2
};

typedef NS_ENUM(NSUInteger, HDLogLevel) {
    HDLogLevelVerbose   = 0,
    HDLogLevelDebug     = 1,
    HDLogLevelInfo      = 2,
    HDLogLevelWarn      = 3,
    HDLogLevelError     = 4
};

// FIXME: 需要修复
//HD_EXTERN NSErrorDomain const HDErrorDomain = @"com.hydra.error";

/**
 
 */
typedef UInt64 HDRequestId;

/**
 
 */
typedef void (^HDCommonCompletion)(HDRequestId requestId,
                                   NSError * _Nullable error);


@protocol HDLogger <NSObject>

@required
- (void)log:(NSString *)content level:(HDLogLevel)level;

@end

@protocol HDHummerDelegate <NSObject>

@optional
/**
 当 Hydra SDK 内部状态变化时会通过该回调通知到业务层
 
 @param oldState 旧的状态值，即 SDK 从那个状态变化过来的
 @param newState 新的状态值，即 SDK 现在变化成了那个状态
 */
- (void)didStateChangeFromState:(HDState)oldState toState:(HDState)newState;

/**
 Hydra SDK 内部连接状态变更通知
 
 @param connState 当前通道连接状态
 */
- (void)didConnectionChanged:(HDConnectionState)connState;

/**
 当出现通道层被踢出是会通过该回调出来
 
 @param code 被提出的原因码
 @param reason 被踢出的原因的描述字符串
 */
- (void)didKicked:(NSInteger)code reason:(NSString * _Nonnull)reason;

/**
 收到 Token 事件
 
 @param requestId 回传触发tToken事件的操作标识
 @param code Token 事件原因码
 @param reason 被Token 事件原因的描述字符串
 */
- (void)didReceiveTokenEvent:(HDRequestId)requestId
                        code:(NSInteger)code
                      reason:(NSString * _Nonnull)reason;

@end

typedef NSDictionary<NSString *, NSString *> *HDHeader;

@interface HDServiceId : NSObject

@property (nonatomic, copy, readonly) NSString *serviceName;
@property (nonatomic, copy, readonly) NSString *functionName;

+ (instancetype)serviceIdWithServiceName:(NSString *)serviceName
                            functionName:(NSString *)functionName;

@end

@interface HDDigitGroup : NSObject

@property (nonatomic, assign, readonly) UInt64 gid;
@property (nonatomic, assign, readonly) UInt64 type;

+ (instancetype)digitGroupWithId:(UInt64)gid
                            type:(UInt64)type;

@end

@interface HDData : NSObject

@property (nonatomic, copy, readonly) NSString *proto;
@property (nonatomic, copy, readonly) NSData *data;

+ (instancetype)dataWithData:(NSData *)data;

+ (instancetype)dataWithProto:(NSString *)proto
                         data:(NSData *)data;

@end

@interface HDMessage : NSObject

@property (nonatomic, copy, readonly) NSString *context;
@property (nonatomic, copy, readonly) HDHeader header;
@property (nonatomic, strong, readonly) HDData *body;

+ (instancetype)messageWithBody:(HDData *)body;

+ (instancetype)messageWithHeader:(HDHeader _Nullable)header
                             body:(HDData *)body;

+ (instancetype)messageWithContext:(NSString * _Nullable)context
                            header:(HDHeader _Nullable)header
                              body:(HDData *)body;

@end

typedef void (^HDRPCCompletion)(HDRequestId requestId,
                                HDServiceId *serviceId,
                                HDMessage *requestMessage,
                                HDMessage *responseMessage,
                                NSError *error);

@protocol HDHydraObserver <NSObject>

@optional
- (void)didReceiveUnicast:(HDServiceId *)serviceId
           unicastMessage:(HDMessage *)message;

- (void)didReceiveBroadcast:(HDServiceId *)serviceId
                   strGroup:(NSString *)group
           broadcastMessage:(HDMessage *)message;

- (void)didReceiveBroadcast:(HDServiceId *)serviceId
                 digitGroup:(HDDigitGroup *)group
           broadcastMessage:(HDMessage *)message;

@end

@interface HDHydra : NSObject

+ (instancetype)sharedInstance;

/**
 Hydra SDK 当前所处的状态
 */
@property (class, nonatomic, assign, readonly) HDState state;
/**
 Hydra SDK 当前所登录的 uid
 */
@property (class, nonatomic, assign, readonly) UInt64 userId;
/**
 Hydra SDK 当前所登录的 长连接实例标识
 */
@property (class, nonatomic, assign, readonly) UInt32 instanceId;

/**
 Hydra SDK 与服务器交互过的最后的服务器端时间（目前只会在心跳包中携带回来），单位为秒
 */
@property (class, nonatomic, assign, readonly) NSTimeInterval lastServerAcceptTs;

/**
 用于获取与服务器同步过的时间戳，单位为秒
 如果与服务器同步失败的话，则返回当前本地的时间
 */
@property (class, nonatomic, assign, readonly) NSTimeInterval syncedCurrentTimestamp;

/**
 初始化 SDK
 调用该方法后，SDK 内部会快速去寻找最优的 AP 节点进行连接
 在连接成功后，业务可以通过调用的对应的接口进行 RPC 请求、 订阅广播等操作
 @param appId 用于表示 App 的表示
 @param appVersion App 的版本号
  
 @return 返回值 < 0，操作失败；
         返回值 > 0，操作成功，并返回当前已累计初始化执行次数
 */
+ (int)initSDKWithAppId:(UInt32)appId
             appVersion:(NSString *)appVersion;

/**
 初始化 SDK
 调用该方法后，SDK 内部会快速去寻找最优的 AP 节点进行连接
 在连接成功后，业务可以通过调用的对应的接口进行 RPC 请求、 订阅广播等操作
 @param appId 用于表示 App 的表示
 @param appVersion App 的版本号
 @param module ap登录标识
  
 @return 返回值 < 0，操作失败；
         返回值 > 0，操作成功，并返回当前已累计初始化执行次数
 */
+ (int)initSDKWithAppId:(UInt32)appId
             appVersion:(NSString *)appVersion
                 module:(NSString *)module;

+ (int)initSDKTestWithAppId:(UInt32)appId
                 appVersion:(NSString *)appVersion
                      lbsIp:(NSString *)lbsIp
                    lbsPort:(uint16_t)lbsPort
                       apIp:(NSString *)apIp
                     apPort:(uint16_t)apPort
                     apType:(uint16_t)apType
                     syncIp:(NSString *)syncIp
                   syncPort:(uint16_t)syncPort
                   syncType:(uint16_t)syncType;

+ (int)initSDKTestWithAppId:(UInt32)appId
                 appVersion:(NSString *)appVersion
                     module:(NSString *)module
                      lbsIp:(NSString *)lbsIp
                    lbsPort:(uint16_t)lbsPort
                       apIp:(NSString *)apIp
                     apPort:(uint16_t)apPort
                     apType:(uint16_t)apType
                     syncIp:(NSString *)syncIp
                   syncPort:(uint16_t)syncPort
                   syncType:(uint16_t)syncType;

/**
 反初始化 SDK
 
 
 @return 返回值 < 0，操作失败；
         返回值 > 0，操作成功，并返回当前剩余初始化执行次数
 @note 对于重复调用初始化方法的场景，
       业务方需调用相同次数的反初始化方法，
       SDK内部才会真正执行反初始化流程，否则当前调用做忽略处理
 */
+ (int)deinit;

+ (void)addDelegate:(id<HDHummerDelegate>)delegate;

+ (void)removeDelegate:(id<HDHummerDelegate>)delegate;

/**
 登录 Hydra SDK
 在登录之后会在对应的 RPC 外层包内填入对应的 uid 信息
 同时SDK内部可以监听到对应改登录 uid 的单播消息，业务放可以通过后台的接口发送单播
 
 @param uid 所需登录的 uid
 @param tokenType token 的类型，详情看 HDTokenType
 @param token token
 @param completion 登录完成后的回调
 */
+ (HDRequestId)login:(UInt64)uid
           tokenType:(HDTokenType)tokenType
               token:(NSData *)token
          completion:(HDCommonCompletion)completion;

/**
 退出登录 Hydra SDK
 退出登录后 SDK 内部的登录态都会清空，并且退出登录后 SDK 也再也不会收到对应的单播消息
 */
+ (void)logout;

+ (HDRequestId)refreshToken:(NSData *)token
                  tokenType:(HDTokenType)tokenType
                 completion:(HDCommonCompletion)completion;

+ (void)setLogger:(id<HDLogger>)logger;

- (HDRequestId)rpcWithServiceId:(HDServiceId *)serviceId
                        message:(HDMessage *)message
                     completion:(HDRPCCompletion)completion;

- (HDRequestId)subscribeStringGroups:(NSSet<NSString *> *)groups;
- (HDRequestId)unsubscribeStringGroups:(NSSet<NSString *> *)groups;

- (HDRequestId)subscribeDigitGroups:(NSSet<HDDigitGroup *> *)groups;
- (HDRequestId)unsubscribeDigitGroups:(NSSet<HDDigitGroup *> *)groups;

- (void)addHydraObserver:(id<HDHydraObserver>)observer;
- (void)removeHydraObserver:(id<HDHydraObserver>)observer;

@end

NS_ASSUME_NONNULL_END
