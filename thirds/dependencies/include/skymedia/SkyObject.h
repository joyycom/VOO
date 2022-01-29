#pragma once

#include "SkyDefs.h"
#include "SkyVariant.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyObjectType_None = -1,
    SkyObjectType_Timeline,
    SkyObjectType_Track,
    SkyObjectType_Clip,
    SkyObjectType_Effect,
    SkyObjectType_Transition,
} SkyObjectType;

class SKYMEDIA_API SkyObject {
public:
    SkyObject &operator=(const SkyObject &rhs) = delete;
    SkyObject(const SkyObject &rhs) = delete;

    SkyObjectType getObjectType() const;

    void setName(const char *name);
    SkyString getName() const;

    void setUserData(const char *key, SkyVariant value);
    SkyVariant getUserData(const char *key) const;

protected:
    SkyObject() = default;
    ~SkyObject() = default;
};

class SKYMEDIA_API SkyObjectList final {
public:
    SkyObjectList();
    SkyObjectList(const SkyObjectList &rhs);
    SkyObjectList &operator=(const SkyObjectList &rhs);
    ~SkyObjectList();

    int size() const;
    SkyObject *operator[](int idx) const;
    SkyObject *at(int idx) const;

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
