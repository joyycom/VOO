#pragma once

#include <qthread.h>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QUrl>

#include "SEProject.h"
#include "base/utils/AppPath.h"
#include "base/utils/ZipUtils.h"
#include "base/utils/fileutils.h"
#include "base/utils/SharedPref.h"
#define FORCE_POSIX
#include <MMKV.h>

enum Type { Video, Audio, Image, Effects, Font, SystemFont, Animations, Input, Other };
struct PreProcessRes {
    Type type;
    SkyClip *clip;
    QString originalPath;
    QString newPath;
    QString prePath;
    bool operator==(const PreProcessRes &res) const {
        return originalPath == res.originalPath;
    }
};
class SkyPack : public QThread {
    Q_OBJECT
public:
    static SkyPack *pack(QString outputDir,
                         QString videoName,
                         QString packName,
                         QString coverPath,
                         int cover,
                         bool video,
                         bool pack,
                         int templateType,
                         QString fashionlineCfg);
    static bool shouldBeCancelled(void *userData);
    static void onProgress(void *userData, double currentTime, double totalTime);
    static void onFinish(void *userData);
    static void onError(void *userData, SkyError error);
    static int64_t getBitRate(SkyTimeline *timeline);
    SkyPack(QString outputDir,
            QString videoName,
            QString packName,
            QString coverPath,
            int cover,
            bool video,
            bool pack,
            int templateType,
            QString fashionlineCfg);
    int getFrom();
    void cancel();

    bool isPacking();
    void setSharedPref(SharedPref *sharedPref);
    bool containsResList(const QSet<PreProcessRes> &resList, const PreProcessRes &res);
    QString getResPathFromList(const QSet<PreProcessRes> &resList, const PreProcessRes &res);
    void namedSkyTimeline(SkyTimeline *timeline);
    // QThread interface
protected:
    void run();

private:
    void init();
    QString ffmpegProgram();
    QString imageMinProgram();
    void execProcess(PreProcessRes &preRes);
    void execProcessStart();
    void collectTotalProgress();
    void updateProgress(double time);
    void onVideoProgress(double time);
    void onVideoError();
    void onVideoFinish();
    void outputPack();
    void updateClipData(SkyClip *clip);
    void updateEffectData(SkyEffect *effect);
    QString copyFile(SkyVariant &params, const char *key, QString path, Type type);
    void getVideoPreRes(PreProcessRes &preRes);
    void getAudioPreRes(PreProcessRes &preRes);
    void getImagePreRes(PreProcessRes &preRes);
    void getInputPreRes(PreProcessRes &preRes);
    void getEffectsPreRes(PreProcessRes &preRes);
    void getFontPreRes(PreProcessRes &preRes, bool systemFont = false);
    void getAnimPreRes(PreProcessRes &preRes);
    void getOtherDirPreRes(PreProcessRes &preRes);
    void getOtherFilePreRes(PreProcessRes &preRes);
    void outputVideo(QString videoPath);
    void outputCover();
    void processTemplateType();
    void namedSkyObject(SkyObject *obj, int index);

    QJsonValue replaceJsonObjectPath(QJsonValue json, QMap<QString, QString> *fileUrls);
    SkyVariant updateUserData(QSet<PreProcessRes> &preResList,
                              SkyClip *clip,
                              SkyVariant &resType,
                              SkyVariant &resName,
                              SkyVariant &resPath);
    Type parseResourecType(ResourceType resourceType);
    void imageMin();
    void luaToOflua(const QString &path);
    void ofluaToLua(const QString &path);

signals:
    void coverChanged(QString &cover);
    void progressChanged(double progress);
    void outputPackFinishChanged(QString &baseDir, QSet<PreProcessRes> &resList, SkyTimeline *timeline);
    void cancelChanged();
    void errorChanged();
    void finishChanged(QString &outputDir, QString &videoPath, QString &packPath);
    void exportLogInfo(int type, QString log);

private:
    QString m_outputDir;
    QString m_videoName;
    QString m_packName;
    QString m_coverPath;
    int m_extrackCover;
    bool m_extrackVideo;
    bool m_extrackPack;
    int m_templateType;

    QString m_fashionlineCfgDir = "";
    QString m_rootDir;
    QString m_tempOutputDir;
    QString m_imagePreDir;
    QString m_resDir;
    QString m_effectsDir;
    QString m_fontsDir;
    QString m_animatiosDir;
    QProcess *m_process = nullptr;
    SkyTimeline *m_newTimeline = nullptr;
    QSet<PreProcessRes> m_resList;
    double m_totalTime = 0;
    double m_currentTime = 0;
    double m_videoTime = 0;
    int videoTypeIndex = 0;
    int audioTypeIndex = 0;
    int imageTypeIndex = 0;
    int effectsTypeIndex = 0;
    int animationsTypeIndex = 0;
    int inputTypeIndex = 0;
    int otherDirTypeIndex = 0;
    int otherFileTypeIndex = 0;
    bool m_cancel = false;
    bool m_error = false;
    SharedPref *m_sharedPref = nullptr;
    MMKV *m_mmkv = nullptr;
};
