#ifndef TIMELINEDEF_H
#define TIMELINEDEF_H

#include <QString>
#include <Qt>

enum TrackRole {
    RoleResource = Qt::UserRole + 1, //资源文件路径
    RoleType,                        //类型（视频，音频，文本）
    RoleFrameRate,                   //帧率
    RoleDuration,                    //时长，返回帧计数
    RoleStartTime,                   //开始时间
    RoleEndTime,                     //结束时间
    RoleCropStartTime,               //可以向前拖拽的时刻
    RoleCropEndTime,                 //可以向后拖拽的时刻
    RoleResourceTime,                //资源总时间
    RoleThumbnails,                  //缩略图
    RolePreEndTime,                  //上一个片段结束时间
    RoleNextStartTime,               //下一个片段开始时间
    RoleTrackType,                   //轨道类型
    RoleClipType,                    //片段类型
    RoleResouceType,                 //资源类型
    RoleIconPath,                    // ICON
    RoleName,                        //特效名
    RoleMinTransitionDuring,         //最小转场时长
    RoleMaxTransitionDuring,         //最大转场时长
    RoleTransitionDuring,            //转场时长
    RoleVolume,                      //音量
    RoleWordContent,                 //文本内容
    RoleSpeed,                       //变速
    RoleFreezeDuration,              //固定时长，不允许拖拉
    RoleCurvedSpeed,                 //曲线变速
    RoleAvgSpeed,                    //平均速度
    RoleHasInputDevice,              //配置了输入器
    RoleKeyFrameTimes,                //关键帧时间点
    RoleGroupInfos                //片段上的分组信息，主要是电商模板需要使用的

};

#define DEF_IMAGE_CLIP_START_TIME 0
#define DEF_IMAGE_CLIP_DURING 5
#define DEF_EFFECT_CLIP_DURING 3000

#define TRACK_ID "id"
#define TRACK_RESOURCE "resource"
#define TRACK_TYPE "type"
#define TRACK_FRAME_RATE "frameRate"
#define TRACK_DURING "duration"
#define TRACK_BEGIN_TIME "startTime"
#define TRACK_END_TIME "endTime"
#define TRACK_CROP_START_TIME "cropStartTime"
#define TRACK_CROP_END_TIME "cropEndTime"
#define TRACK_RESOURCE_TIME "resouceTime"
#define TRACK_THUMBNAILS "thumbnails"
#define TRACK_PRE_END_TIME "preEndTime"
#define TRACK_NEXT_START_TIME "nextStartTime"
#define TRACK_TRACK_TYPE "trackType"
#define TRACK_CLIP_TYPE "clipType"
#define TRACK_RESOURCE_TYPE "resourceType"
#define TRACK_ICON_PATH "iconPath"
#define TRACK_RESOURCE_NAME "resourceName"
#define TRACK_MIN_TRANSITION_DURING "minTransitonDuring"
#define TRACK_MAX_TRANSITION_DURING "maxTransitonDuring"
#define TRACK_TRANSITION_DURING "transitonDuring"
#define TRACK_VOLUME "volume"
#define TRACK_WORD_CONTENT "wordContent"
#define TRACK_SPEED "speed"
#define TRACK_FREEZE_DURATION "freezeDuration"
#define TRACK_CURVED_SPEED "curvedSpeed"
#define TRACK_AVG_SPEED "avgSpeed"
#define TRACK_HAS_INPUT_DEVICE "hasInputDevice"
#define TRACK_KEY_FRAME_TIMES "keyFrameTimes"
#define TRACK_KEY_GroupInfos "fashionLineGroupInfos"


#define SKY_KEY_OWNER = "sky_owner"
#define SKY_KEY_CLIP_DATA = "sky_clip_data"

enum TrackType {
    VideoTrack = 0, // 视频轨 只能添加视频（包括图片）资源，对应SDK中的SkyVideoTrack
    AudioTrack = 1, // 音频轨 只能添加图片资源 对应SDK中的SkyAudioTrack
    EffectTrack = 2 //特效轨 只能添加特效，为了统一操作 对应SDK中的SkyVideoTrack，对应Resource信息是个透明的片段
};

static constexpr struct {
    const TrackType type;
    const char* name;
} s_trackTypeTable[] = {
    {TrackType::VideoTrack, "Video"},
    {TrackType::AudioTrack, "Audio"},
    {TrackType::EffectTrack, "Effect"}
};

static const char* nameFromTrackType(TrackType type) {
    return s_trackTypeTable[(int)type].name;
}

static const TrackType trackTypeFromName(const char* name) {
    for(auto item:s_trackTypeTable){
        if(strcmp(name,item.name) == 0){
            return item.type;
        }
    }
    return VideoTrack;
}

enum ClipType { VideoClipType = 0, AudioClipType = 1, ImageClipType = 2, EffectClipType = 3, TransitionClipType = 4 };

enum ResourceType {
    VideoResource = 0,
    AudioResource = 1,
    ImageResource = 2,
    EffectResource = 3,
    StickerResource = 4,
    TextResource = 5,
    TransitionResource = 6,
    FilterResource = 7,
    MoreResource = 8,
    OverlayResource = 9
};


static constexpr struct {
    const ResourceType type;
    const char* name;
} s_resourceTypeTable[] = {
    {ResourceType::VideoResource, "Video"},
    {ResourceType::AudioResource, "Audio"},
    {ResourceType::ImageResource, "Image"},
    {ResourceType::EffectResource, "Effect"},
    {ResourceType::StickerResource, "Sticker"},
    {ResourceType::TextResource, "TextTemplate"},
    {ResourceType::TransitionResource, "Transition"},
    {ResourceType::FilterResource, "Filter"},
    {ResourceType::MoreResource, "More"},
    {ResourceType::OverlayResource, "Overlay"}
};

static const char* nameFromResourceType(ResourceType type) {
    return s_resourceTypeTable[(int)type].name;
}

static const ResourceType resourceTypeFromName(const char* name) {
    for(auto item:s_resourceTypeTable){
        if(strcmp(name,item.name) == 0){
            return item.type;
        }
    }
    return EffectResource;
}


#endif // TIMELINEDEF_H
