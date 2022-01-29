//
//  YYLoginLiteSDK+RiskControl.h
//  yyloginlitesdk
//
//  Created by 梁志锋 on 2020/10/30.
//  Copyright © 2020 Maple. All rights reserved.
//

#import "YYLoginLiteSDK.h"
#import <riskControl/RiskControlSDKDefine.h>

NS_ASSUME_NONNULL_BEGIN

@interface YYLoginLiteSDK (RiskControl)
/// 挑战接口
/// @param chanllengeExtDict 挑战信息
/// @param callBack 回调
/// callback 参数解析：
/// result：挑战结果，定义如下：
///     RiskControlChallengeResultSuccess = 0,///<挑战成功
///     RiskControlChallengeResultFailed = -1,///<挑战失败
///     RiskControlChallengeResultCancel = -2,///<取消挑战
///     RiskControlChallengeResultParamsError = -3,///<参数错误
///     RiskControlChallengeResultAllreadyChallenged = -4,///<挑战手段未解锁
/// 用途：在业务方接入风控之后（业务后台接入风控服务器），发送请求如果触发风控，则通过以下方法，弹出挑战手段；

- (void)yy_showVerifyViewWithInfo:(NSDictionary *)chanllengeExtDict
                         callBack:(void (^)(RiskControlChallengeResult result))callBack;

/// 获取默认扩展字段
/// 用途：由于风控sdk不与风控服务器交互，所以业务方在发送请求的时候，带上一些扩展字段给风控服务器，扩展字段由风控sdk内部组装，业务方通过yy_getDefaultExt获取；
- (NSDictionary *)yy_getDefaultExt;


/// 调用安全SDK的runCode并返回结果用于上传
/// @param code 服务器下发的检测代码
- (NSData *)yy_runCode:(NSData *)code;

/// 获取缓存的PCID
- (NSString *)yy_getPCID;

/// 采集设备信息并连接服务器同步更新PCID
/// @param bizName 向风控后台申请，根据不同场景传不同的值
- (void)yy_syncPCID:(NSString *)bizName;

@end

NS_ASSUME_NONNULL_END
