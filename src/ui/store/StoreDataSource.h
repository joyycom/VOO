#ifndef StoreDataSource_H
#define StoreDataSource_H

#include <QObject>
#include "StoreCategoryModel.h"
#include "StoreResourceModel.h"
#include "LocalResourceModel.h"
#include <project/SEProject.h>

class StoreDataSource : public QObject {
    Q_OBJECT
public:
    explicit StoreDataSource(QObject *parent = nullptr);
    ~StoreDataSource();

    void showToast(QString msg, int during = 3000);

signals:
    void categoryListChanged(CategoryListModel *data);
    void showResourceList();
    void resourceListChanged(StoreResourceListModel *data);
    void resourceDownloadComplate(int index);
    void showVideoLocalList();
    void videoListChanged(LocalResourceListModel *data);
    void showAudioLocalList();
    void audioListChanged(LocalResourceListModel *data);
    void showMoreLocalList();
    void moreListChanged(LocalResourceListModel *data);

    void toastContentChanged(QString msg, int during);

public slots:
    void loadData();

    //分类
    void selectTopCategory(QString cate);
    QString selectTopCategoryID();
    void selectThirdCategory(int subIndex, int thirdIndex);
    void selectSubCategory(int index);

    //特效资源
    CategoryListModel *categoryListModel();
    StoreResourceListModel *resources();
    QString getResourcePath(int index);
    void selectResourceIndex(int index);
    void selectLocalVideoResourceIndex(int index);
    void selectLocalAudioResourceIndex(int index);
    void selectLocalMoreResourceIndex(int index);
    void addLocalVideoResource(QVariantList list, int trackIndex = -1, double time = -1);
    void addLocalAudioResource(QVariantList list, int trackIndex = -1, double time = -1);
    void addLocalMoreResource(QVariantList list, int trackIndex = -1, double time = -1);
    void addSubEffectToVideoClip(QVariantList list, int trackIndex, int clipIndex);
    void deleteLocalVideoResource(QVariantList list);
    void deleteLocalAudioResource(QVariantList list);
    void deleteLocalMoreResource(QVariantList list);

    void addNewEffect(QUrl dirPath);

    void editEffect(int index);
    void uploadEffect(int index);

    void openEffectDir(int index);
    void openVideoDir(int index);
    void openAudioDir(int index);
    void openResourceDir(LocalResourceListModel *resource, int index);

    void addResourceToTimeline(int index, int trackIndex = -1, double time = -1);

    //本地资源
    LocalResourceListModel *localVideoResource();
    int localVideoDataCount();
    LocalResourceListModel *localAudioResource();
    int localAudioDataCount();
    LocalResourceListModel *localMoreResource();
    int localMoreDataCount();
    void openNewVideo(QString filePath);
    void openNewAudio(QString filePath);
    bool openNewMore(QString filePath);

private:
    SkyResourceBean createResBean(QString id, QString name, QString path, QString modelThumb);

    void selectLocalResourceIndex(LocalResourceListModel *localResource, int index);

    void combindAudioAndMusicCategory();

    void loadTopCategoryData(QString category);

    QString pascalCase(QString input);

    void newEffect(const QString &path, const QString &targetpath, const QMap<QString, QString> &map);

    void writeTemplate(const QString &src, const QString &dst, const QMap<QString, QString> &map);

    //分类列表
    QList<CategoryModel> m_allCategorys;

    CategoryListModel *m_curCategoryListModel = new CategoryListModel;
    QString m_topCategoryId = "VIDEO";
    void updateCurrentCategoryView();
    void onLoadCategoryFinish(const QString &response);

    //资源列表
    QList<StoreResourceModel> m_allResources;
    StoreResourceListModel *m_currentResources = new StoreResourceListModel;
    void onLoadCategoryDataFinish(QString cate, const QString &response);

    LocalResourceListModel *m_localVideoResource = new LocalResourceListModel;
    LocalResourceListModel *m_localAudioResource = new LocalResourceListModel;
    LocalResourceListModel *m_localMoreResource = new LocalResourceListModel;
};

#endif // StoreDataSource_H
