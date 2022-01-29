#pragma once

#include "SkyDefs.h"
#include "SkyResource.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyBackgroundStrategy_Changed = 0, // 修改背景（默认情况）
    SkyBackgroundStrategy_Keep = 1,    // 保持背景不变
    SkyBackgroundStrategy_Auto = 2,    // 假如是列表的最后特效，就修改背景。否则保持背景不变。
} SkyBackgroundStrategy;

class SkyInputList;
class SKYMEDIA_API SkyInputItem final {
public:
    SkyInputItem *operator=(const SkyInputItem &rhs) = delete;
    SkyInputItem(const SkyInputItem &rhs) = delete;

    SkyInputList *getOwner() const;

    void setAsUser();
    void setAsUser(int userIdx);
    void setAsBackground();
    void setAsBackground(SkyBackgroundStrategy ouputBgStrategy);
    void setAsResource(const SkyResource &res);
    void setAsResource(const SkyResource &res, const SkyTimeRange &trimRange);

    SkyResource getResource() const;

    SkyTimeRange getTrimRange() const;

    void setName(const char *name);
    SkyString getName() const;

    void setCropRect(const SkyRect &region);
    SkyRect getCropRect() const;

    void setCropTransform(const SkyTransform2D &transform);
    SkyTransform2D getCropTransform() const;

    // 缩放视频尺寸。cropRect 和 resize 都被设置时，会先裁剪再拉伸。
    void resize(int width, int height, SkyScaleMode scaleMode);

private:
    SkyInputItem();
    ~SkyInputItem();
};

class SkyEffect;
class SKYMEDIA_API SkyInputList final {
public:
    SkyInputList *operator=(const SkyInputList &rhs) = delete;
    SkyInputList(const SkyInputList &rhs) = delete;

    SkyEffect *getOwner() const;

    SkyInputItem *getItemAt(int index) const;
    SkyInputItem *findItemByName(const char *name) const;

    void ensureNumber(int number);
    int numberOfItems() const;

    void commit();

private:
    SkyInputList();
    ~SkyInputList();
};

SKYMEDIA_NAMESPACE_END
