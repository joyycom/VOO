#include "VenusManager.h"
#include "orangefilter/of_core.h"
#include <assert.h>

template <typename T, typename KeyT>
static bool isContains(const std::vector<T> &v, const KeyT &key) {
    auto iter = std::find(v.begin(), v.end(), key);
    return iter != v.end();
}

static bool str_ends_with(const std::string &value, const std::string &ending) {
    if (ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

enum class VenusApiType {
    FaceDetector,
    FaceParser,
    FaceCartoon,
    GeneralSplit,
    GestureDetect,
};

class VenusApiData final {
public:
    VenusApiData(const std::string &output, VenusApiType api, const std::string &mode)
        : outputKey(output),
          venusMode(mode),
          apiType(api) {
    }
    std::string outputKey;
    std::string venusMode;
    VenusApiType apiType;
};

static void onFree(void *ctx, uint8_t *data) {
    free(data);
}

static void onFreeMemoryNode(void *ctx, uint8_t *data) {
    free(ctx);
}

static SkyVariant copyToBinary(const uint8_t *data, size_t size, VenusContext &ctx) {
    SkyBinary bin;
    if (VenusContext::MemoryNode *node = ctx.moveMomory((void *)data)) {
        bin.data = (uint8_t *)node->data();
        bin.size = size;
        return SkyVariant(bin, node, onFreeMemoryNode);
    } else {
        bin.data = (uint8_t *)malloc(size);
        bin.size = size;
        memcpy(bin.data, data, size);
        return SkyVariant(bin, nullptr, onFree);
    }
}

static SkyVariant transVnRect(const VN2_Rect2D &rect) {
    SkyVariant box = SkyVariant::makeArray();
    box.append(rect.x0);
    box.append(rect.y0);
    box.append(rect.x1);
    box.append(rect.y1);
    return box;
}

#ifndef GL_RGB
#define GL_RGB 0x1907
#endif

#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

#ifndef GL_LUMINANCE
#define GL_LUMINANCE 0x1909
#endif

static SkyVariant transVNImageFormatVariant(VN2_PIX_FMT fmt) {
    switch (fmt) {
        case VN2_PIX_FMT_RGBA8888:
            return GL_RGBA;
        case VN2_PIX_FMT_GRAY8:
            return GL_LUMINANCE;
        case VN2_PIX_FMT_RGB888:
            return GL_RGB;
        default:
            return 0;
    }
}

static SkyVariant transImage2Variant(const VN2_Image &image, VenusContext &ctx) {
    SkyVariant imageVar = SkyVariant::makeObject();
    imageVar.insert("width", image.width);
    imageVar.insert("height", image.height);

    imageVar.insert("byteData", copyToBinary((uint8_t *)image.data, image.width * image.height * image.channels, ctx));
    imageVar.insert("format", transVNImageFormatVariant(image.pix_fmt));
    imageVar.insert("box", transVnRect(image.rect));
    return imageVar;
}

static SkyVariant transImageArr2Variant(const VN2_ImageArr &images, VenusContext &ctx) {
    SkyVariant list = SkyVariant::makeArray();
    for (int i = 0; i < images.imgsNum; ++i) {
        VN2_Image image = images.imgsArr[i];
        list.append(transImage2Variant(image, ctx));
    }
    return list;
}

static constexpr struct {
    VN2_GestureType vn_type;
    OF_GestureType of_type;
} s_gestureTypeTable[] = {
    {VN2_GestureType_Unknow, OF_GestureType_Retina_Unknow},
    {VN2_GestureType_V, OF_GestureType_Retina_V},
    {VN2_GestureType_ThumbUp, OF_GestureType_Retina_ThumbUp},
    {VN2_GestureType_OneHandHeart, OF_GestureType_Retina_OneHandHeart},
    {VN2_GestureType_SpiderMan, OF_GestureType_Retina_Spiderman},
    {VN2_GestureType_Lift, OF_GestureType_Retina_Lift},
    {VN2_GestureType_666, OF_GestureType_Retina_666},
    {VN2_GestureType_TwoHandHeart, OF_GestureType_Retina_TwoHandHeart},
    {VN2_GestureType_PalmTogether, OF_GestureType_Retina_PalmTogether},
    {VN2_GestureType_PalmOpen, OF_GestureType_Retina_PalmOpen},
    {VN2_GestureType_ZuoYi, OF_GestureType_Retina_ZuoYi},
    {VN2_GestureType_OK, OF_GestureType_Retina_OK},
    {VN2_GestureType_ERROR, OF_GestureType_Retina_end},
};

static OF_GestureType vnGestureType2OF(VN2_GestureType type) {
    for (auto &info : s_gestureTypeTable) {
        if (info.vn_type == type) {
            return info.of_type;
        }
    }
    return OF_GestureType_Retina_end;
}

static SkyVariant transFaceFrame2Variant(const VN2_FaceFrameDataArr &image, VenusContext &ctx) {
    SkyVariant list = SkyVariant::makeArray();
    for (int i = 0; i < image.facesNum; ++i) {
        SkyVariant frameVar = SkyVariant::makeObject();
        auto frame = image.facesArr[i];
        frameVar.insert("score", frame.faceScore);
        frameVar.insert("facePointsCount", (int)frame.faceLandmarksNum);
        frameVar.insert("box", transVnRect(frame.faceRect));
        frameVar.insert("smileScore", frame.smileScore);
        frameVar.insert("isEyeBlink", frame.ifBlinkLeftEye || frame.ifBlinkRightEye);
        frameVar.insert("isMouthOpen", frame.ifOpenMouth);

        float landmarks[278 * 2];
        for (int i = 0; i < frame.faceLandmarksNum; ++i) {
            landmarks[2 * i] = frame.faceLandmarks[i].x;
            landmarks[2 * i + 1] = frame.faceLandmarks[i].y;
        }
        frameVar.insert("facePoints", copyToBinary((uint8_t *)landmarks, sizeof(landmarks), ctx));
        list.append(frameVar);
    }
    return list;
}

static SkyVariant transGestureData2Variant(const VN2_GestureFrameDataArr &gestureArr) {
    SkyVariant gestures = SkyVariant::makeArray();
    for (int i = 0; i < gestureArr.gestureNum; ++i) {
        SkyVariant gestureData = SkyVariant::makeObject();
        gestureData.insert("type", vnGestureType2OF(gestureArr.gestureArr[i].type));
        gestureData.insert("score", gestureArr.gestureArr[i].score);
        gestureData.insert("box", transVnRect(gestureArr.gestureArr[i].rect));
        gestures.append(gestureData);
    }
    return gestures;
}

VenusContext::VenusContext(const std::vector<std::string> &apis, const std::vector<std::string> &modes)
    : m_venusApis(apis),
      m_venusModes(modes) {
    m_memoryList.next = &m_memoryList;
    m_memoryList.prev = &m_memoryList;
}

template <typename T>
struct ListUtils final {
    static void add(T *new_, T *prev, T *next) {
        next->prev = new_;
        new_->next = next;
        new_->prev = prev;
        prev->next = new_;
    }

    static void add(T *new_, T *head) {
        add(new_, head, head->next);
    }

    static void del(T *prev, T *next) {
        next->prev = prev;
        prev->next = next;
    }

    static void del(T *entry) {
        del(entry->prev, entry->next);
    }
};

VenusContext::~VenusContext() {
    MemoryNode *head = &m_memoryList;
    while (head->next != head) {
        MemoryNode *node = head->next;
        ListUtils<MemoryNode>::del(node);
        free(node);
    }
}

void *VenusContext::allocMemoryAutoFree(size_t size) {
    MemoryNode *data = (MemoryNode *)malloc(size + sizeof(MemoryNode));
    ListUtils<MemoryNode>::add(data, &m_memoryList);
    return data->data();
}

VenusContext::MemoryNode *VenusContext::moveMomory(void *data) {
    MemoryNode *head = &m_memoryList;
    MemoryNode *pos = head->next;
    while (pos != head) {
        if (pos->data() == data) {
            ListUtils<MemoryNode>::del(pos);
            return pos;
        }
        pos = pos->next;
    }
    return nullptr;
}

VN2_Image VenusContext::fillInputImage(const uint8_t *imgRgba, int width, int height) {
    VN2_Image inputImg;
    inputImg.ori_fmt = VN2_ORIENT_FMT_DEFAULT;
    inputImg.pix_fmt = VN2_PIX_FMT_RGBA8888;
    inputImg.mode_fmt = VN2_MODE_FMT_PICTURE;
    inputImg.width = width;
    inputImg.height = height;
    inputImg.channels = 4;
    inputImg.data = (void *)imgRgba;
    inputImg.texture = nullptr;
    return inputImg;
}

SkyVariant VenusContext::exportResult() {
    SkyVariant result = SkyVariant::makeObject();
    result.insert("version", "1.0.0");

    if (faceCartoons && isContains(m_venusApis, FaceCartoonKey)) {
        result.insert(FaceCartoonKey, transImageArr2Variant(*faceCartoons, *this));
    }

    if (faceMasks && isContains(m_venusApis, FaceMaskKey)) {
        result.insert(FaceMaskKey, transImageArr2Variant(*faceMasks, *this));
    }

    if (faceFrames && isContains(m_venusApis, FaceInfoKey)) {
        result.insert(FaceInfoKey, transFaceFrame2Variant(*faceFrames, *this));
    }

    if (gestureData && isContains(m_venusApis, GestureDataKey)) {
        result.insert(GestureDataKey, transGestureData2Variant(*gestureData));
    }

    for (auto &pair : splitDatas) {
        auto &key = pair.first;
        auto &image = pair.second.imgsArr[0];
        result.insert(key.c_str(), transImage2Variant(image, *this));
    }
    return result;
}

class VenusFrameProcessor final {
public:
    std::vector<std::string> venusApis;
    std::vector<std::string> modes;

    static void *createInstance(void *className, SkyVariant params);
    static void destoryInstance(void *instance);
    static SkyVariant onFrameData(void *instance, const SkyFrameDataInputContext &inputCtx);
};

static std::vector<std::string> convertStringArray(const SkyVariant venus) {
    std::vector<std::string> apis;
    for (int i = 0; i < venus.size(); ++i) {
        apis.push_back(venus[i].toString());
    }
    return apis;
}

void *VenusFrameProcessor::createInstance(void *className, SkyVariant params) {
    VenusFrameProcessor *processor = new VenusFrameProcessor();
    processor->venusApis = convertStringArray(params.find("venus"));
    processor->modes = convertStringArray(params.find("venus_modes"));
    return processor;
}

void VenusFrameProcessor::destoryInstance(void *instance) {
    VenusFrameProcessor *processor = static_cast<VenusFrameProcessor *>(instance);
    delete processor;
}

SkyVariant VenusFrameProcessor::onFrameData(void *instance, const SkyFrameDataInputContext &inputCtx) {
    VenusFrameProcessor *processor = static_cast<VenusFrameProcessor *>(instance);
    if (processor->venusApis.empty()) {
        return SkyVariant::makeNull();
    }

    bool needProcess =
        inputCtx.getTriggerMode() == SkyTriggerMode_Export ||
        (inputCtx.getTriggerMode() == SkyTriggerMode_Snap && inputCtx.getExtentPointer("AsyncVNTimeline"));

    if (!needProcess) {
        return SkyVariant::makeNull();
    }

    SkyVariant frame = inputCtx.getInputByKey(SkyInputKey_OriginVideoFrame); // 通过 SkyInputKey_VideoFrame，拿到数据
    if (frame.isNull()) {
        frame = inputCtx.getInputByKey(SkyInputKey_VideoFrame);
    }

    int width = frame["width"].toInt();
    int height = frame["height"].toInt();
    SkyBinary rgbaData = frame["rgba8Data"].toBinary();

    VenusContext context(processor->venusApis, processor->modes);
    VN2_Image inputImage = VenusContext::fillInputImage(rgbaData.data, width, height);
    return VenusManager::sharedInstance().processFrame(inputImage, context);
}

class BaseVenusApi : VenusNoncopyable {
public:
    struct VenusApiInfo final {
        VN2_Result (*createHandle)(VN2Handle *handle, const int argc, const void *argv[]);
        VN2_Result (*destroyHandle)(VN2Handle *handle);
    };

    static constexpr VN2Handle kInvalidHandle = -1;

    BaseVenusApi(const std::string &mode, const std::vector<std::string> &modelPaths, const VenusApiInfo &info)
        : m_mode(mode),
          m_info(info) {
        m_handle = kInvalidHandle;

        if (modelPaths.empty()) {
            SKY_LOG_E("[voo-vn2] makeHandle Failed! Mode Files NOT EXIST!! Mode: %s", m_mode.c_str());
            return;
        }

        int argc = static_cast<int>(modelPaths.size());
        if (str_ends_with(modelPaths.back(), ".json")) {
            argc -= 1;
        }

        std::vector<const char *> argv;
        for (auto &str : modelPaths) {
            argv.push_back(str.c_str());
        }
        VN2_Result result = m_info.createHandle(&m_handle, argc, (const void **)argv.data());
        if (result != VN2_Result_Success) {
            m_handle = kInvalidHandle;
            SKY_LOG_E("[voo-vn2] makeVenusHandle Failed!!!! %s", m_mode.c_str());
        }
    }

    virtual ~BaseVenusApi() {
        if (m_handle != kInvalidHandle) {
            m_info.destroyHandle(&m_handle);
            m_handle = kInvalidHandle;
        }
    }

    virtual bool apply(const VN2_Image &inputImage, VenusContext &context) = 0;

    VN2Handle getHandle() const {
        return m_handle;
    }

    const std::string &getMode() const {
        return m_mode;
    }

private:
    VN2Handle m_handle = kInvalidHandle;
    std::string m_mode;
    VenusApiInfo m_info;
};

class GestureDetectApi : public BaseVenusApi {
public:
    GestureDetectApi(const std::string &mode, const std::vector<std::string> &modelPaths)
        : BaseVenusApi(mode, modelPaths, {VN2_Gesture_Create, VN2_Gesture_Destory}) {
    }

    virtual bool apply(const VN2_Image &inputImage, VenusContext &context) override {
        assert(getHandle() != kInvalidHandle);
        if (context.gestureData) {
            return true;
        }

        context.gestureData.reset(new VN2_GestureFrameDataArr());
        memset(context.gestureData.get(), 0x00, sizeof(VN2_GestureFrameDataArr));

        VN2_Result result = VN2_Gesture_Apply_CPU(getHandle(), &inputImage, context.gestureData.get());
        return (result == VN2_Result_Success);
    }
};

class GeneralSplitApi : public BaseVenusApi {
public:
    GeneralSplitApi(const std::string &mode, const std::string &ouputKey, const std::vector<std::string> &modelPaths)
        : BaseVenusApi(mode, modelPaths, {VN2_Create_General, VN2_Destroy_General}),
          m_outputKey(ouputKey) {

        if (!modelPaths.empty()) {
            std::string jsonPath = modelPaths.back();
            SkyString json = SkyString::loadFromFile(jsonPath.c_str());
            SkyVariant config = SkyVariant::parseFromJsonString(json.c_str());
            SkyVariant preproc = config["preproc"];
            if (preproc.getType() == SkyVariantType_Object) {
                m_outWidth = preproc["netW"].toInt();
                m_outHeight = preproc["netH"].toInt();
            }
        }
    }

    virtual bool apply(const VN2_Image &inputImage, VenusContext &context) override {
        assert(getHandle() != kInvalidHandle);
        auto iter = context.splitDatas.find(m_outputKey);
        if (iter != context.splitDatas.end()) {
            return true;
        }

        const int kChannels = 1; // 3， 风格类， 1， +
        VN2_ImageArr imageArr;
        imageArr.imgsNum = 1;
        imageArr.imgsArr[0].data = context.allocMemoryAutoFree(m_outWidth * m_outHeight * kChannels);
        imageArr.imgsArr[0].width = m_outWidth;
        imageArr.imgsArr[0].height = m_outHeight;
        imageArr.imgsArr[0].pix_fmt = kChannels == 1 ? VN2_PIX_FMT_GRAY8 : VN2_PIX_FMT_RGB888;
        imageArr.imgsArr[0].channels = kChannels;
        imageArr.imgsArr[0].mode_fmt = str_ends_with(getMode(), "video") ? VN2_MODE_FMT_VIDEO : VN2_MODE_FMT_DEFAULT;

        VN2_Result result = VN2_Apply_General_CPU(getHandle(), &inputImage, nullptr, &imageArr);
        context.splitDatas.insert(std::make_pair(m_outputKey, imageArr));

        return (result == VN2_Result_Success);
    }

private:
    int m_outWidth = 512;
    int m_outHeight = 512;
    std::string m_outputKey;
};

class FaceParserApi : public BaseVenusApi {
public:
    FaceParserApi(const std::string &mode, const std::vector<std::string> &modelPaths)
        : BaseVenusApi(mode, modelPaths, {VN2_Create_FaceParser, VN2_Destroy_FaceParser}) {
    }

    virtual bool apply(const VN2_Image &inputImage, VenusContext &context) override {
        assert(getHandle() != kInvalidHandle);
        if (context.faceFrames == nullptr || context.faceFrames->facesNum == 0) {
            return false;
        }

        if (context.faceMasks) {
            return true;
        }

        context.faceMasks.reset(new VN2_ImageArr());
        for (unsigned int i = 0; i < context.faceFrames->facesNum; ++i) {
            context.faceMasks->imgsArr[i].width = FACEMASK_WIDTH;
            context.faceMasks->imgsArr[i].height = FACEMASK_HEIGHT;
            context.faceMasks->imgsArr[i].mode_fmt = VN2_MODE_FMT_PICTURE;
            context.faceMasks->imgsArr[i].channels = FACEMASK_CHANNEL;
            context.faceMasks->imgsArr[i].pix_fmt = VN2_PIX_FMT_GRAY8;
            context.faceMasks->imgsArr[i].data =
                context.allocMemoryAutoFree(FACEMASK_WIDTH * FACEMASK_HEIGHT * FACEMASK_CHANNEL);
        }

        VN2_Result result =
            VN2_Apply_FaceParser_CPU(getHandle(), &inputImage, context.faceFrames.get(), context.faceMasks.get());
        return (result == VN2_Result_Success);
    }

    static const int FACEMASK_WIDTH = 128;
    static const int FACEMASK_HEIGHT = 128;
    static const int FACEMASK_CHANNEL = 1;
};

class FaceDetectApi : public BaseVenusApi {
public:
    FaceDetectApi(const std::string &mode, const std::vector<std::string> &modelPaths)
        : BaseVenusApi(mode, modelPaths, {VN2_Create_Face, VN2_Destory_Face}) {
    }

    virtual bool apply(const VN2_Image &inputImage, VenusContext &context) override {
        assert(getHandle() != kInvalidHandle);
        if (context.faceFrames) {
            return true;
        }

        context.faceFrames.reset(new VN2_FaceFrameDataArr());
        memset(context.faceFrames.get(), 0x00, sizeof(VN2_FaceFrameDataArr));

        VN2_Result result = VN2_Apply_Face_CPU(getHandle(), &inputImage, context.faceFrames.get());
        return (result == VN2_Result_Success);
    }
};

class FaceCartoonMakerApi : public BaseVenusApi {
public:
    FaceCartoonMakerApi(const std::string &mode, const std::vector<std::string> &modelPaths)
        : BaseVenusApi(mode, modelPaths, {VN2_Create_CartFaceMaker, VN2_Destroy_CartFaceMaker}) {
    }

    virtual bool apply(const VN2_Image &inputImage, VenusContext &context) override {
        assert(getHandle() != kInvalidHandle);
        if (context.faceFrames == nullptr || context.faceFrames->facesNum == 0) {
            return false;
        }

        if (context.faceCartoons) {
            return true;
        }

        context.faceCartoons.reset(new VN2_ImageArr());
        for (unsigned int i = 0; i < context.faceFrames->facesNum; ++i) {
            context.faceCartoons->imgsArr[i].width = CARTFACE_WIDTH;
            context.faceCartoons->imgsArr[i].height = CARTFACE_HEIGHT;
            context.faceCartoons->imgsArr[i].mode_fmt = VN2_MODE_FMT_PICTURE;
            context.faceCartoons->imgsArr[i].channels = CARTFACE_CHANNEL;
            context.faceCartoons->imgsArr[i].pix_fmt = VN2_PIX_FMT_RGB888;
            context.faceCartoons->imgsArr[i].data =
                context.allocMemoryAutoFree(CARTFACE_WIDTH * CARTFACE_HEIGHT * CARTFACE_CHANNEL);
        }

        VN2_Result result =
            VN2_Apply_CartFaceMaker_CPU(getHandle(), &inputImage, context.faceFrames.get(), context.faceCartoons.get());
        return (result == VN2_Result_Success);
    }

    static const int CARTFACE_WIDTH = 512;
    static const int CARTFACE_HEIGHT = 512;
    static const int CARTFACE_CHANNEL = 3;
};

static std::unique_ptr<BaseVenusApi> createVenusApi(const VenusApiData &apiData,
                                                    const std::vector<std::string> &modelPaths) {
    switch (apiData.apiType) {
        case VenusApiType::FaceDetector:
            return std::unique_ptr<BaseVenusApi>(new FaceDetectApi(apiData.venusMode, modelPaths));
        case VenusApiType::FaceParser:
            return std::unique_ptr<BaseVenusApi>(new FaceParserApi(apiData.venusMode, modelPaths));
        case VenusApiType::FaceCartoon:
            return std::unique_ptr<BaseVenusApi>(new FaceCartoonMakerApi(apiData.venusMode, modelPaths));
        case VenusApiType::GeneralSplit:
            return std::unique_ptr<BaseVenusApi>(new GeneralSplitApi(apiData.venusMode, apiData.outputKey, modelPaths));
        case VenusApiType::GestureDetect:
            return std::unique_ptr<BaseVenusApi>(new GestureDetectApi(apiData.venusMode, modelPaths));
    }
    return nullptr;
}

static std::vector<VenusApiData> generateApiTypes(const std::vector<std::string> &venusApis,
                                                  const std::vector<std::string> &modes) {
    std::vector<VenusApiData> venusApiDatas;
    bool useDefaultModes = venusApis.size() != modes.size();
    for (int i = 0; i < venusApis.size(); ++i) {
        std::string api = venusApis[i];
        if (api == FaceInfoKey) {
            std::string mode = useDefaultModes ? "venus2_face278_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::FaceDetector, mode));
        } else if (api == FaceCartoonKey) {
            std::string mode = useDefaultModes ? "venus2_cartface_data" : modes[i];
            venusApiDatas.push_back(VenusApiData("", VenusApiType::FaceDetector, "venus2_face278_data"));
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::FaceCartoon, mode));
        } else if (api == FaceMaskKey) {
            std::string mode = useDefaultModes ? "venus2_faceparse_data" : modes[i];
            venusApiDatas.push_back(VenusApiData("", VenusApiType::FaceDetector, "venus2_face278_data"));
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::FaceParser, mode));
        } else if (api == ClothesSegmentKey) {
            std::string mode = useDefaultModes ? "venus2_clothesseg_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GeneralSplit, mode));
        } else if (api == SkySegmentKey) {
            std::string mode = useDefaultModes ? "venus2_skyseg_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GeneralSplit, mode));
        } else if (api == SegmentFrameKey) {
            std::string mode = useDefaultModes ? "venus2_segment_picture" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GeneralSplit, mode));
        } else if (api == HeadSegmentKey) {
            std::string mode = useDefaultModes ? "venus2_headseg_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GeneralSplit, mode));
        } else if (api == HairSegmentKey) {
            std::string mode = useDefaultModes ? "venus2_hairseg_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GeneralSplit, mode));
        } else if (api == AnimalSegmentKey) {
            std::string mode = useDefaultModes ? "venus2_animalseg_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GeneralSplit, mode));
        } else if (api == GestureDataKey) {
            std::string mode = useDefaultModes ? "venus2_gesture_data" : modes[i];
            venusApiDatas.push_back(VenusApiData(api, VenusApiType::GestureDetect, mode));
        }
    }
    return venusApiDatas;
}

static void registerSkyMediaFrameCallback() {
    SkyFrameDataClass cls;
    cls.createInstance = VenusFrameProcessor::createInstance;
    cls.destoryInstance = VenusFrameProcessor::destoryInstance;
    cls.onFrameData = VenusFrameProcessor::onFrameData;
    SkyApi::registerFrameDataClass("VenusFrameProcessor", cls);
}

static int venusLogCallback(const char *i_log) {
    SKY_LOG_I("[vn2] %s", i_log);
    return 0;
}

VenusManager::VenusManager() {
    VN2_Result result = VN2_SetLogLevel(VN2_LOG_LEVEL_INFO);
    result = VN2_SetLogCallback(venusLogCallback);
    result = VN2_SetLogTag("Venus1");
    registerSkyMediaFrameCallback();
}

VenusManager &VenusManager::sharedInstance() {
    static VenusManager s_instance;
    return s_instance;
}

SkyVariant VenusManager::processFrame(const VN2_Image &inputImage, VenusContext &context) {
    std::unique_lock<std::mutex> lock(m_mutex);
    const std::vector<std::string> &venusapis = context.venusApis();
    const std::vector<std::string> &modes = context.venusModes();
    std::vector<VenusApiData> venusApiDatas = generateApiTypes(venusapis, modes);

    for (auto apiData : venusApiDatas) {
        BaseVenusApi *api = getVenusApi(apiData);
        assert(api);
        if (api) {
            api->apply(inputImage, context);
        }
    }
    return context.exportResult();
}

BaseVenusApi *VenusManager::getVenusApi(const VenusApiData &apiData) {
    KeyT key = std::make_pair(apiData.outputKey, apiData.venusMode);
    auto iter = m_apis.find(key);
    if (iter == m_apis.end()) {
        std::unique_ptr<BaseVenusApi> api = createVenusApi(apiData, findModelPaths(apiData.venusMode));
        if (api && api->getHandle() == BaseVenusApi::kInvalidHandle) {
            api.reset();
        }
        iter = m_apis.insert(std::make_pair(key, std::move(api))).first;
        assert(iter != m_apis.end());
    }
    return iter->second.get();
}

static const char *s_modelPathsJson = R"(
{
  "venus2_animalseg_data": [
    "venus2_animalseg_data/animal_segment.vnmod",
    "venus2_animalseg_data/animal_segment.vncfg",
    "venus2_animalseg_data/animal_segment_process_config.json"
  ],
  "venus2_cartface_data": [
    "venus2_cartface_data/cart_face_maker_PC[1.0.2].vnmod",
    "venus2_cartface_data/cart_face_maker_PC[1.0.2].vncfg"
  ],
  "venus2_clothesseg_data": [
    "venus2_clothesseg_data/clothes_segment.vnmod",
    "venus2_clothesseg_data/clothes_segment.vncfg",
    "venus2_clothesseg_data/clothes_segment_process_config.json"
  ],
  "venus2_face278_data": [
    "venus2_face278_data/dlface_detection_mobile[1.0.0].vnmod",
    "venus2_face278_data/dlface_detection_mobile[1.0.0].vncfg",
    "venus2_face278_data/dlface_104pts[1.0.0]_278pts[1.0.8].vnmods",
    "venus2_face278_data/dlface_104pts[1.0.0]_278pts[1.0.8].vncfgs"
  ],
  "venus2_faceparse_data": [
    "venus2_faceparse_data/face_parser_mobile_PC[1.0.7].vnmod",
    "venus2_faceparse_data/face_parser_mobile_PC[1.0.7].vncfg"
  ],
  "venus2_gesture_data": [
    "venus2_gesture_data/gesture[1.0.0].20210122.vnmods",
    "venus2_gesture_data/gesture[1.0.0].20210122.vncfgs"
  ],
  "venus2_hairseg_data": [
    "venus2_hairseg_data/hair_segment.vnmod",
    "venus2_hairseg_data/hair_segment.vncfg",
    "venus2_hairseg_data/hair_segment_process_config.json"
  ],
  "venus2_headseg_data": [
    "venus2_headseg_data/head_segment.vnmod",
    "venus2_headseg_data/head_segment.vncfg",
    "venus2_headseg_data/headseg_v3_process_config.json"
  ],
  "venus2_segment_picture": [
    "venus2_segment_data/segment_portrait_picture.vnmod",
    "venus2_segment_data/segment_portrait_picture.vncfg",
    "venus2_segment_data/segment_portrait_picture_process_config.json"
  ],
  "venus2_segment_video": [
    "venus2_segment_data/segment_portrait_video.vnmod",
    "venus2_segment_data/segment_portrait_video.vncfg",
    "venus2_segment_data/segment_portrait_video_process_config.json"
  ],
  "venus2_skyseg_data": [
    "venus2_skyseg_data/sky_segment_v2.vnmod",
    "venus2_skyseg_data/sky_segment_v2.vncfg",
    "venus2_skyseg_data/sky_segment_v2_process_config.json"
  ]
}
)";

static std::vector<std::string> transModelPaths(const SkyPath &modeDir, const SkyVariant &fileList) {
    std::vector<std::string> result;
    for (int i = 0; i < fileList.size(); i++) {
        SkyPath filePath = modeDir.join(fileList[i].toString());
        assert(filePath.isExists());
        if (!filePath.isExists()) {
            SKY_LOG_E("[voo-vn2] Mode File is Not Exist, skip! File: %s", filePath.c_str());
            continue;
        }
        result.push_back(filePath.c_str());
    }
    return result;
}

static std::map<std::string, std::vector<std::string>> fillAllModelPaths(const std::string &venusBaseDir) {
    SkyPath modeDir(venusBaseDir.c_str());
    assert(modeDir.isExists());

    SkyVariant paths = SkyVariant::parseFromJsonString(s_modelPathsJson);
    assert(paths.getType() == SkyVariantType_Object);

    std::map<std::string, std::vector<std::string>> result;
    SkyStringList allKeys = paths.getAllKeys();
    for (int i = 0; i < allKeys.size(); i++) {
        const char *key = allKeys.c_strAt(i);
        SkyVariant fileList = paths.find(key);
        result.insert(std::make_pair(key, transModelPaths(modeDir, fileList)));
    }
    return result;
}

void VenusManager::setVenusBaseDir(const std::string &baseDir) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_modelPaths = fillAllModelPaths(baseDir);
}

std::vector<std::string> VenusManager::findModelPaths(const std::string &key) const {
    auto iter = m_modelPaths.find(key);
    if (iter != m_modelPaths.end()) {
        return iter->second;
    } else {
        return std::vector<std::string>();
    }
}
