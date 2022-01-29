#pragma once

#include "hydra/constants.h"

#if defined(__ANDROID__) && !defined(DYNAMIC_VERSION)
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Android 静态库的情况下
// 需要提供填充 JVM 的方法给外部
jint JNI_OnLoad_Hydra(JavaVM *jvm, void *reserved);
void JNI_OnUnLoad_Hydra(JavaVM *jvm, void *reserved);

#ifdef __cplusplus
}
#endif

#endif

namespace hydra {

// 公共部分
const uint32_t kInvalidAppId = 0;
const uint64_t kAnonymousUserId = 0;

enum HydraLogLevel {
  LEVEL_VERBOSE = 0,
  LEVEL_DEBUG,
  LEVEL_INFO,
  LEVEL_WARN,
  LEVEL_ERROR,
};

class HydraLogger
{
 public:
  virtual ~HydraLogger() = default;
  virtual void onHydraLog(HydraLogLevel level, const char *pMsg) = 0;
};

class Delegate {
 public:
  virtual ~Delegate();

  virtual void onHydraStateChanged(State fromState, State toState) = 0;

  virtual void onHydraConnectionChanged(ConnectionState connState) = 0;

  virtual void onHydraKicked(const Code& kickCode) = 0;

  virtual void onHydraTokenEvent(RequestId requestId, const Code& tokenCode) = 0;
};

/**
 * 异步操作的回调接口，用于如login的异步返回
 */
class Completion {
 public:
  virtual ~Completion() = default;

  virtual void onComplete(RequestId requestId, const Code& code) = 0;
};
  
/**
 * 初始化 SDK
 * @param appId 接入方appid
 * @param appVersion 接入方版本号
 * @param module 登录ap时的通道标识
 * @return 返回值 < 0，操作失败；
 *         返回值 > 0，操作成功，并返回当前已累计初始化执行次数
 */
int init(uint32_t appId, const char* appVersion, const char* module = "");
  
/**
 * ！！调试接口，仅供调试！！
 * @param appId 接入方appid
 * @param appVersion 接入方版本号
 * @param lbsIp 指定调试LBS IP
 * @param lbsPort 指定调试LBS PORT
 * @param apIp 指定AP IP
 * @param apPort 指定AP PORT
 * @param apType 指定AP TYPE，默认为 3
 * @param syncIp 指定对时服务IP，一般和AP一致
 * @param syncPort 指定对时服务PORT
 * @param syncType 指定对时服务TYPE，默认为 11
 * @param module 登录ap时的通道标识
 * @return 返回值 < 0，操作失败；
 *         返回值 > 0，操作成功，并返回当前已累计初始化执行次数
 */
int initWithTest(uint32_t appId, const char* appVersion,
                 const char* lbsIp, uint16_t lbsPort,
                 const char* apIp, uint16_t apPort, uint32_t apType,
                 const char* syncIp, uint16_t syncPort, uint32_t syncType, const char* module = "");

/**
 * 反初始化 SDK
 * @return 返回值 < 0，操作失败；
 *         返回值 > 0，操作成功，并返回当前剩余初始化执行次数
 *
 * @note 对于重复调用初始化方法的场景，
 *       业务方需调用相同次数的反初始化方法，
 *       SDK内部才会真正执行反初始化流程，否则当前调用做忽略处理
 */
int deinit();

void addDelegate(Delegate* delegate);

void removeDelegate(Delegate* delegate);

State state();

uint64_t userId();
uint32_t instanceId();

// 获取服务器同步过的当前时间戳, 单位为毫秒
uint64_t getSyncedTimestamp();
uint64_t getLastServerAcceptTs();

RequestId login(uint64_t uid, const Data& token, Completion* completion);

RequestId logout();

RequestId refreshToken(const Data& token, Completion* completion);

void setHydraLogger(HydraLogger *pHydraLogger);

const char* getSDKVersion();

// 后期多实例部分（可以给不同模块使用不同的实例，各自维护）
class HydraListener {
 public:
  virtual ~HydraListener();

  virtual void onRecv(const ServiceId& sid,
                      const Message& unicastMessage) = 0;

  virtual void onRecv(const ServiceId& sid,
                      const char* strGroup,
                      const Message& broadcastMessage) = 0;

  virtual void onRecv(const ServiceId& sid,
                      const DigitGroup& numGroup,
                      const Message& broadcastMessage) = 0;
};

class OnWayMessageListener {
  public:
    virtual ~OnWayMessageListener();

    virtual void onRecvMessage(const ServiceId &sid, const Message &message, uint32_t crc) = 0;
};

class RPCCompletion {
 public:
  virtual ~RPCCompletion() = default;

  virtual void onRPCComplete(RequestId requestId,
                             const char* context,
                             const ServiceId& sid,
                             const Message& clientMessage,
                             const Code& statusCode,
                             const Message* serverMessage) = 0;
};

class Hydra {
 public:
  virtual RequestId rpc(const ServiceId& sid,
                        const Message& clientMessage,
                        RPCCompletion* completion) = 0;

  virtual RequestId rpc(const char* context,
                        const ServiceId& sid,
                        const Message& clientMessage,
                        const RPCOptions& options,
                        RPCCompletion* completion) = 0;

  virtual Dictionary& defaultHeaders() = 0;

  virtual RequestId subscribe(const StringGroups& groups) = 0;

  virtual RequestId subscribe(const DigitGroups& groups) = 0;

  virtual RequestId unsubscribe(const StringGroups& bcGroups) = 0;

  virtual RequestId unsubscribe(const DigitGroups& groups) = 0;

  virtual void addListener(HydraListener* listener) = 0;

  virtual void removeListener(HydraListener* listener) = 0;

  virtual void addOneWayMessageListener(OnWayMessageListener* listener) = 0;

  virtual void removeOneWayMessageListener(OnWayMessageListener* listener) = 0;

 protected:
  // 防止用户直接通过delete来释放实例
  virtual ~Hydra();
};

/**
 * 共享单例
 */
Hydra* hydra();

} // namespace hydra
