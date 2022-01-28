#pragma once

#include "venus/vn2_common.h"
#include "venus/vn2_faceparser.h"
#include "venus/vn2_kit.h"
#include "venus/vn2_core.h"
#include "venus/vn2_gesture.h"
#include "venus/vn2_general.h"
#include "venus/vn2_face.h"

#include "skymedia/SkyApi.h"

#include <map>
#include <vector>
#include <string>
#include <mutex>
SKYMEDIA_USING_NAMESPACE

class VenusNoncopyable {
protected:
    VenusNoncopyable() = default;
    ~VenusNoncopyable() = default;

    VenusNoncopyable(const VenusNoncopyable &) = delete;
    VenusNoncopyable &operator=(const VenusNoncopyable &) = delete;
};

class VenusContext final : VenusNoncopyable {
public:
    static VN2_Image fillInputImage(const uint8_t *imgRgba, int width, int height);

    VenusContext(const std::vector<std::string> &apis, const std::vector<std::string> &modes);
    ~VenusContext();

    std::unique_ptr<VN2_FaceFrameDataArr> faceFrames;
    std::unique_ptr<VN2_GestureFrameDataArr> gestureData;
    std::unique_ptr<VN2_ImageArr> faceMasks;
    std::unique_ptr<VN2_ImageArr> faceCartoons;
    std::map<std::string, VN2_ImageArr> splitDatas;

    const std::vector<std::string> &venusApis() const {
        return m_venusApis;
    }

    const std::vector<std::string> &venusModes() const {
        return m_venusModes;
    }

    struct MemoryNode final {
        MemoryNode *next;
        MemoryNode *prev;

        void *data() const {
            return (uint8_t *)this + sizeof(MemoryNode);
        }
    };

    void *allocMemoryAutoFree(size_t size);
    MemoryNode *moveMomory(void *data);

private:
    friend class VenusManager;
    SkyVariant exportResult();
    std::vector<std::string> m_venusApis;
    std::vector<std::string> m_venusModes;
    MemoryNode m_memoryList;
};

//"venus":["faceCartoons","faceMasks"]
//"faceCartoons","faceMasks","faceItemArr","segmentFrameData","gestureFrameDataArr","bodyFrameDataArr","audioFrameData"
//其实就是frameData里面的数据
static const char *FaceInfoKey = "faceItemArr";
static const char *FaceCartoonKey = "faceCartoons";
static const char *FaceMaskKey = "faceMasks";
static const char *SegmentFrameKey = "segmentTexture";
static const char *ClothesSegmentKey = "clothesTexture";
static const char *SkySegmentKey = "skyTexture";
static const char *HeadSegmentKey = "headTexture";
static const char *AnimalSegmentKey = "animalTexture";
static const char *HairSegmentKey = "hairTexture";
static const char *GestureDataKey = "gestureArr";
static const char *StylizeCartoonKey = "stylizeCartoon";
static const char *StylizeComicKey = "stylizeComic";

class BaseVenusApi;
class VenusApiData;
class VenusManager final : VenusNoncopyable {
public:
    static VenusManager &sharedInstance();

    void setVenusBaseDir(const std::string &baseDir);

    SkyVariant processFrame(const VN2_Image &inputImage, VenusContext &context);

private:
    VenusManager();
    BaseVenusApi *getVenusApi(const VenusApiData &apiData);
    std::vector<std::string> findModelPaths(const std::string &key) const;

private:
    using KeyT = std::pair<std::string, std::string>;
    mutable std::mutex m_mutex;
    std::map<KeyT, std::unique_ptr<BaseVenusApi>> m_apis;
    std::map<std::string, std::vector<std::string>> m_modelPaths;
};
