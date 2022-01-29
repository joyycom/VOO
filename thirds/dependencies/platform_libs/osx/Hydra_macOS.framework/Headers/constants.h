#pragma once

#include <cctype>
#include <cstdint>
#include <string>

namespace hydra {

typedef uint64_t RequestId;

const int kInvalidRequestId = 0;

/**
 * SDK状态定义
 */
enum State {
  HYDRA_UNAVAILABLE = 0,

  /**
   * 表示传输通道正在尝试建立与AP服务器的连接，在该状态下发送的请求会被hydra缓存下来。直到请求超时，或连接
   * 建立后按FIFO的顺序进行发送
   */
  HYDRA_CONNECTING = 1,

  /**
   * 通道已连接上，可以提供到服务器的消息路由功能，但并不具备'具名uid'的能力，因此协议数据中携带的uid为0，且无法
   * 收到服务器针对某个uid下发的单播消息
   */
  HYDRA_ANONYMOUS = 2,

  HYDRA_LOGGING_IN = 3,

  /**
   * 通道已连接，且成功执行了登录操作。后续所有rpc均会带上该身份（uid）信息，且可以接收到服务器对该uid下发
   * 的单播消息
   */
  HYDRA_LOGGED_IN = 4,
};

/**
 * 通道连接状态定义
 */
enum ConnectionState {
  /**
   * 通道已断开
   */
  HYDRA_CONN_DISCONNECTED = 0,

  /**
   * 通道正在连接中
   */
  HYDRA_CONN_CONNECTING = 1,

  /**
   * 通道已连接
   */
  HYDRA_CONN_CONNECTED = 2,
};

struct TokenType {
  static const char* TYPE_DOMESTIC;
  static const char* TYPE_OVERSEAS;
  static const char* TYPE_SUNCLOUD;
  static const char* TYPE_YY_COOKIE;
};

class CodeImpl;
class Code {
 public:
  Code();
  Code(uint32_t value, const char* reason, const char* extra = nullptr);
  Code(const Code& code);
  Code& operator= (const Code& code);
  ~Code();

  uint32_t value() const;

  const char* desc() const;

  bool operator == (const Code& rhs) const;
  bool operator != (const Code& rhs) const;

 private:
  CodeImpl* impl;
};

#define DEFINE_CODE(value, name, msg) static inline Code name(const char* extra = nullptr) \
  { return Code(value, msg, extra); }

struct KickedCodes {
    DEFINE_CODE(100,  KC_REJECTED,       "Account rejected by logged in another device")
    DEFINE_CODE(1001, KC_PWD_CHANGED,    "Password changed")
    DEFINE_CODE(1002, KC_ACCOUNT_LOCKED, "Account locked")
    DEFINE_CODE(1003, KC_ACCOUNT_SEALED, "Account sealed")
    DEFINE_CODE(1004, KC_TOKEN_EXPIRED,  "Token expired")
    DEFINE_CODE(1005, KC_DEVICE_SEALED,  "Device sealed")
    DEFINE_CODE(1006, KC_RECOVER_FAILED, "Recover failed")
};

struct TokenCodes {
  DEFINE_CODE(2001, TC_PARAMETER_INVALID, "Request parameters invalid")
  DEFINE_CODE(2002, TC_VERIFY_FAILED,     "OTP ticket verify failed")
  DEFINE_CODE(2003, TC_EXPIRED,           "OTP ticket expired")
  DEFINE_CODE(2018, TC_APPID_INVALID,     "Appid invalid")
  DEFINE_CODE(2999, TC_WILL_EXPIRE,       "OTP ticket will expire")
};

struct Codes {
  DEFINE_CODE(200, OK,                 "OK")
  DEFINE_CODE(208, ONEWAY_SUCCESS,     "One-way message delivered")
  DEFINE_CODE(400, BAD_REQUEST,        "Bad Request")
  DEFINE_CODE(401, UNAUTHORIZED,       "Unauthorized")
  DEFINE_CODE(402, PAYMENT_REQUIRED,   "Payment required")
  DEFINE_CODE(403, FORBIDDEN,          "Forbidden")
  DEFINE_CODE(404, NOT_FOUND,          "Not found")
  DEFINE_CODE(408, REQUEST_TIMEOUT,    "Request timeout, try again later")
  DEFINE_CODE(453, EXCEEDED_LIMIT,     "Requests exceeded the limit")
  DEFINE_CODE(500, INTERNAL_SVR_ERROR, "Internal server error")
  DEFINE_CODE(502, BAD_GATEWAY,        "Bad gateway")
  DEFINE_CODE(503, SVC_UNAVAILABLE,    "Service unavailable")
  DEFINE_CODE(504, GATEWAY_TIMEOUT,    "Gateway timeout")
};

struct LoginCodes : public Codes {
  DEFINE_CODE(300, LGC_RETRY_LATER,      "Not ready, try again later")
  DEFINE_CODE(403, LGC_OTP_INVALID,      "OTP ticket verify failed")
  DEFINE_CODE(406, LGC_OVERLOADED,       "AP overloaded, try another ap")
  DEFINE_CODE(409, LGC_RESOURCE_CONCLIT, "Resource conflict, try another ap")
};

class DataImpl;
class Data {
 public:
  Data(const void* content, int size, const char* proto = nullptr);
  Data(const Data& data);
  Data& operator = (const Data& rhs);
  ~Data();

  const void* content() const;

  int size() const;

  const char* proto() const;

  bool operator == (const Data& rhs) const;

 private:
  DataImpl* impl;
};

class ServiceIdImpl;
class ServiceId {
 public:
  ServiceId(const char* serviceName, const char* functionName);
  ServiceId(const ServiceId& serviceId);
  ServiceId& operator = (const ServiceId& rhs);
  ~ServiceId();

  const char* serviceName() const;
  const char* functionName() const;

  bool operator == (const ServiceId& rhs) const;

 private:
  ServiceIdImpl* impl;
};

/**
 * BCGroups 表示广播组标识集合，一般用于进行广播消息订阅时指定
 * 对应的广播组。该集合可同时处理数字广播组标识和字符串广播组标识
 */
struct GroupsImpl;

class StringGroups {
 public:
  StringGroups();
  StringGroups(const char* group); // NOLINT
  explicit StringGroups(int count, ...);
  StringGroups(const StringGroups& cpy);
  ~StringGroups();
  StringGroups& operator = (const StringGroups& rhs);

  void add(const char* group);

  int count() const;

  const char* groupAt(int pos) const;

 private:
  GroupsImpl* impl;
};

struct DigitGroup {
  uint64_t type;
  uint64_t id;

  DigitGroup(uint64_t gtype, uint64_t gid) {
    this->type = gtype;
    this->id   = gid;
  }

  bool isValid() const {
    return type != 0 && id != 0;
  }

  bool operator<(const DigitGroup& rhs) const {
    return (id < rhs.id) || ((id == rhs.id) && (type < rhs.type));
  }

  bool operator==(const DigitGroup& rhs) const {
    return (type == rhs.type) && (id == rhs.id);
  }
};

class DigitGroups {
 public:
  DigitGroups();
  DigitGroups(const DigitGroup& group); // NOLINT
  DigitGroups(const DigitGroups& cpy);
  ~DigitGroups();
  DigitGroups& operator= (const DigitGroups& rhs);

  void add(const DigitGroup& group);

  int count() const;

  DigitGroup groupAt(int pos) const;

 private:
  GroupsImpl* impl;
};

/**
 * Key-Value 字符串二元组数据项
 *
 * @discuss
 * 对应于c++的std::pair<std::string, std::string> 数据结构，为了避免STL版本
 * 冲突问题，这里提供了专用封装
 */
struct KVItem {
  const char* key;
  const char* value;
};

/**
 * 字符串字典数据结构
 *
 * @discuss
 * 对应于c++的std::map<std::string, std::string> 数据结构，为了避免STL版本
 * 冲突问题而提供了专用封装
 */
class DictionaryImpl;
class Dictionary {
 public:
  Dictionary();
  Dictionary(const Dictionary& cpy);
  Dictionary& operator = (const Dictionary& rhs);
  ~Dictionary();

  /**
   * 添加KV数据项
   *
   * @param key 头数据的索引名
   * @param value 头数据的值
   */
  void add(const char* key, const char* value);

  /**
   * 根据key值移除KV数据项
   *
   * @param key 头数据索引名
   */
  void remove(const char* key);

  /**
   * 获取数据项总共数量
   */
  int count() const;

  /**
   * 根据位置数据项，一般用于在不知道key的情况下进行数据枚举
   */
  const KVItem* itemAt(int position) const;

  /**
   * 根据数据项key直接查询头数据值
   *
   * @param key 头数据的索引名
   */
  const char* valueFor(const char* key) const;

  bool operator == (const Dictionary& rhs) const;

 private:
  DictionaryImpl* impl;
};

struct MessageImpl;
class Message {
 public:
  static Message empty();

  explicit Message(const Data& body);
  Message(const Dictionary& headers, const Dictionary& routes, const Data& body);
  Message(const Dictionary& headers, const Data& body);
  Message(const Message& message);
  Message& operator= (const Message& rhs);
  ~Message();

  Dictionary& headers();

  Dictionary& routes();

  const Data& body() const;

  bool operator == (const Message& rhs) const;

 private:
  MessageImpl* impl;
};

class RPCOptionsImpl;
class RPCOptions {
 public:
  static const int kDefaultTimeoutInterval = 10;

  RPCOptions();
  RPCOptions(const RPCOptions& options);
  RPCOptions& operator= (const RPCOptions& rhs);
  ~RPCOptions();

  int numberOfTimeoutIntervals() const;
  int timeoutIntervalAt(int idx) const;
  void setTimeoutInterval(int interval);
  void setTimeoutIntervals(int count, int* intervals);

 private:
  RPCOptionsImpl* impl;
};

} // namespace hydra
