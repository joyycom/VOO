#pragma once

#include "SkyDefs.h"
#include <cstdint>

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyVariantType_Null = 0,
    SkyVariantType_Bool = 1,
    SkyVariantType_Int64 = 2,
    SkyVariantType_Double = 3,
    SkyVariantType_String = 4,
    SkyVariantType_Array = 5,
    SkyVariantType_Object = 6,
    SkyVariantType_Binary = 7,
} SkyVariantType;

struct SKYMEDIA_API SkyBinary {
    uint8_t *data = nullptr;
    int64_t size = 0;
};

class SKYMEDIA_API SkyVariant final {
public:
    SkyVariant();
    SkyVariant(const SkyVariant &rhs);
    SkyVariant &operator=(const SkyVariant &rhs);
    ~SkyVariant();

    SkyVariant(double val);
    SkyVariant(float val);
    SkyVariant(int val);
    SkyVariant(int64_t val);
    SkyVariant(bool val);
    SkyVariant(const char *val);
    SkyVariant(const SkyBinary &val, void *ctx, void (*onFree)(void *ctx, uint8_t *data));
    SkyVariant(const void *) = delete;

    static SkyVariant makeNull();
    static SkyVariant makeArray();
    static SkyVariant makeObject();

    // 有时需要对路径做些特殊处理，比如保存和恢复时，路径需要在相对路径和绝对路径间相互转换
    // 于是我们在 VariantType_String 上附加一个标记。当有这个标记的时候，就可知此时字符串是用来表示路径。
    // 在这里，我们并没有定义新的 VariantType_Path 类型。除了一些特殊处理，路径和字符串的逻辑是比较相似的。
    // 不新增 Path 类型，也容易跟之前的代码兼容。
    // 当调用 SkyVariant::makePath 时，得到的也是 SkyVariantType_String，只是附加一个标记。
    static SkyVariant makePath(const char *path);
    bool hasPathTag() const;

    double toDouble() const;
    int toInt() const;
    int64_t toInt64() const;
    bool toBool() const;
    const char *toString() const;
    SkyBinary toBinary() const;

    int size() const;
    bool empty() const;

    bool operator==(const SkyVariant &rhs) const;
    bool operator!=(const SkyVariant &rhs) const;

    // 数组操作
    const SkyVariant operator[](int idx) const;
    const SkyVariant at(int idx) const;
    SkyVariant &append(const SkyVariant &val);
    void replace(int idx, const SkyVariant &val);

    // 字典操作
    const SkyVariant operator[](const char *key) const;
    const SkyVariant find(const char *key) const;
    void getAllKeys(SkyString *holder, int size) const;
    SkyStringList getAllKeys() const;
    SkyVariant &insert(const char *key, const SkyVariant &val);
    void replace(const char *key, const SkyVariant &val);

    double getDouble(const char *key, double defaultValue) const {
        SkyVariant value = find(key);
        return value.isNull() ? defaultValue : value.toDouble();
    }

    int getInt(const char *key, int defaultValue) const {
        SkyVariant value = find(key);
        return value.isNull() ? defaultValue : value.toInt();
    }

    int64_t getInt64(const char *key, int64_t defaultValue) const {
        SkyVariant value = find(key);
        return value.isNull() ? defaultValue : value.toInt64();
    }

    bool getBool(const char *key, bool defaultValue) const {
        SkyVariant value = find(key);
        return value.isNull() ? defaultValue : value.toBool();
    }

    const char *getString(const char *key, const char *defaultValue) const {
        SkyVariant value = find(key);
        return value.isNull() ? defaultValue : value.toString();
    }

    template <typename Fun>
    void safeGet(const char *key, Fun &&fun) const {
        SkyVariant value = find(key);
        if (!value.isNull()) {
            fun(value);
        }
    }

    bool isNull() const;
    SkyVariantType getType() const;

    SkyVariant &merge(const SkyVariant &rhs);
    SkyVariant deepClone() const;

    SkyString transToJsonString(bool isPretty = true) const;
    SkyString transToJsonString(bool isPretty, bool withBinary) const;
    static SkyVariant parseFromJsonString(const char *json);

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
