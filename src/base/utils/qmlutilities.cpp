#include "qmlutilities.h"

#include <base/widget/FramelessWindow.h>
#include <timeline/controller/components/AnimationListComponent.h>
#include <timeline/controller/components/AudioComponent.h>
#include <timeline/controller/components/ColorSelectorComponent.h>
#include <timeline/controller/components/CurvedSpeedComponent.h>
#include <timeline/controller/components/DropdownListComponent.h>
#include <timeline/controller/components/EffectGroupEditorComponent.h>
#include <timeline/controller/components/EffectInputListComponent.h>
#include <timeline/controller/components/ImageComponent.h>
#include <timeline/controller/components/LabelComponent.h>
#include <timeline/controller/components/LayoutComponent.h>
#include <timeline/controller/components/MutliInputTextComponent.h>
#include <timeline/controller/components/NameComponent.h>
#include <timeline/controller/components/ParticleColorComponent.h>
#include <timeline/controller/components/ParticleFileParserComponent.h>
#include <timeline/controller/components/ParticleValueComponent.h>
#include <timeline/controller/components/SaveOfEffectComponent.h>
#include <timeline/controller/components/SimpleComponent.h>
#include <timeline/controller/components/SimpleSpeedComponent.h>
#include <timeline/controller/components/SimpleTransformComponent.h>
#include <timeline/controller/components/SliderComponent.h>
#include <timeline/controller/components/SubEffectListComponent.h>
#include <timeline/controller/components/SwitchComponent.h>
#include <timeline/controller/components/TTSComponent.h>
#include <timeline/controller/components/TransitionComponent.h>
#include <timeline/controller/components/VideoComponent.h>
#include <timeline/controller/components/VolumeComponent.h>
#include <timeline/controller/components/VolumeFadeComponent.h>
#include <timeline/controller/components/WordComponent.h>
#include <timeline/models/asyncprocessmodel.h>
#include <timeline/qrcode/QrCodeController.h>
#include <timeline/widget/AudioWavWidget.h>
#include <timeline/widget/basicimagedatasource.h>
#include <timeline/widget/CropPreviewPlayer.h>
#include <timeline/widget/UpdateResourcePreviewPlayer.h>
#include <ui/export/ExportDialogDataSource.h>
#include <ui/feedback/feedbackdatasource.h>
#include <ui/guide/GuideDataSource.h>
#include <ui/store/StoreDataSource.h>
#include <ui/store/UploadEffectController.h>
#include <user/LoginController.h>
#include "qmlutil.h"

QmlUtilities::QmlUtilities(QObject *parent) : QObject(parent) {
}

void QmlUtilities::registerCommonTypes() {
    qmlRegisterType<StoreDataSource>("StoreDataSource", 1, 0, "StoreDataSource");
    qmlRegisterType<CategoryListModel>("CategoryListModel", 1, 0, "CategoryListModel");
    qmlRegisterType<StoreResourceListModel>("StoreResourceListModel", 1, 0, "StoreResourceListModel");
    qmlRegisterType<GuideDataSource>("GuideDataSource", 1, 0, "GuideDataSource");
    qmlRegisterType<ProjectHistoryListModel>("ProjectHistoryListModel", 1, 0, "ProjectHistoryListModel");
    qmlRegisterType<SkyTimelineView>("SkyMedia", 1, 0, "SkyTimelineView");
    qmlRegisterType<SkyTimelineView2>("SkyMedia", 1, 0, "SkyTimelineView2");
    qmlRegisterType<MiniPlayerView>("SkyMedia", 1, 0, "MiniPlayerView");
    qmlRegisterType<EditorPageDataSource>("EditorPageDataSource", 1, 0, "EditorPageDataSource");
    qmlRegisterType<PreviewController>("PreviewController", 1, 0, "PreviewController");
    qmlRegisterType<TimelineController>("TimelineController", 1, 0, "TimelineController");
    qmlRegisterType<TimelineModel>("TimelineModel", 1, 0, "TimelineModel");
    qmlRegisterType<AsyncProcessModel>("AsyncProcessModel", 1, 0, "AsyncProcessModel");
    qmlRegisterType<PropertyEditController>("PropertyEditController", 1, 0, "PropertyEditController");
    qmlRegisterType<LocalResourceListModel>("LocalResourceListModel", 1, 0, "LocalResourceListModel");
    qmlRegisterType<EffectGridPropertyModel>("EffectGridPropertyModel", 1, 0, "EffectGridPropertyModel");
    qmlRegisterType<AudioWavWidget>("AudioWavWidget", 1, 0, "AudioWavWidget");
    qmlRegisterType<CropPreviewPlayer>("CropPreviewPlayer", 1, 0, "CropPreviewPlayer");
    qmlRegisterType<UpdateResourcePreviewPlayer>("UpdateResourcePreviewPlayer", 1, 0, "UpdateResourcePreviewPlayer");
    qmlRegisterType<TransformComponent>("TransformComponent", 1, 0, "TransformComponent");
    qmlRegisterType<SimpleTransformComponent>("SimpleTransformComponent", 1, 0, "SimpleTransformComponent");

    //    qmlRegisterType<PropertyStickerController>("PropertyStickerController", 1, 0, "PropertyStickerController");
    // qmlRegisterType<AnimationsListModel>("AnimationsListModel", 1, 0, "AnimationsListModel"); // SliderComponent
    qmlRegisterType<SliderComponent>("SliderComponent", 1, 0, "SliderComponent"); //
    qmlRegisterType<AnimationListComponent>("AnimationListComponent", 1, 0, "AnimationListComponent");
    qmlRegisterType<LabelComponent>("LabelComponent", 1, 0, "LabelComponent");
    qmlRegisterType<VideoComponent>("VideoComponent", 1, 0, "VideoComponent"); //
    qmlRegisterType<AudioComponent>("AudioComponent", 1, 0, "AudioComponent");
    qmlRegisterType<WordComponent>("WordComponent", 1, 0, "WordComponent");
    qmlRegisterType<VolumeComponent>("VolumeComponent", 1, 0, "VolumeComponent");
    qmlRegisterType<NameComponent>("NameComponent", 1, 0, "NameComponent");
    qmlRegisterType<TransitionComponent>("TransitionComponent", 1, 0, "TransitionComponent");
    qmlRegisterType<StoreResourcePreviewController>(
        "StoreResourcePreviewController", 1, 0, "StoreResourcePreviewController");
    qmlRegisterType<SkyMediaPlayer>("SkyMediaPlayer", 1, 0, "SkyMediaPlayer");
    qmlRegisterType<ExportDialogDataSource>("ExportDialogDataSource", 1, 0, "ExportDialogDataSource");
    qmlRegisterType<QmlFileUtils>("QmlUtils", 1, 0, "FileUtils");
    qmlRegisterType<QmlUtils>("QmlUtils", 1, 0, "QmlUtils");
    qmlRegisterType<FeedBackDataSource>("FeedBackDataSource", 1, 0, "FeedBackDataSource");
    qmlRegisterType<CurvedSpeedComponent>("CurvedSpeedComponent", 1, 0, "CurvedSpeedComponent");
    qmlRegisterType<SimpleSpeedComponent>("SimpleSpeedComponent", 1, 0, "SimpleSpeedComponent");
    qmlRegisterType<ImageComponent>("ImageComponent", 1, 0, "ImageComponent");
    qmlRegisterType<QrCodeController>("QrCodeController", 1, 0, "QrCodeController");
    qmlRegisterType<TimelineController>("TimelineController", 1, 0, "TimelineController");
    qmlRegisterType<LoginController>("LoginController", 1, 0, "LoginController");
    qmlRegisterType<BasicImageDataSource>("BasicImageDataSource", 1, 0, "BasicImageDataSource");
    //    qmlRegisterType<BaseComponent>("BaseComponent", 1, 0, "BaseComponent");//SimpleComponent
    qmlRegisterType<SimpleComponent>("SimpleComponent", 1, 0, "SimpleComponent"); // SimpleComponent
    qmlRegisterType<ColorSelectorComponent>("ColorSelectorComponent", 1, 0, "ColorSelectorComponent"); // SimpleComponent
    qmlRegisterType<FramelessWindow>("FramelessWindow", 1, 0, "FramelessWindow");
    qmlRegisterRevision<QQuickWindow, QT_VERSION_MINOR>("FramelessWindow", 1, 0);
    qmlRegisterRevision<QWindow, QT_VERSION_MINOR>("FramelessWindow", 1, 0);
    qmlRegisterType<BaseLayoutComponent>("BaseLayoutComponent", 1, 0, "BaseLayoutComponent"); // SimpleComponent
    qmlRegisterType<ColumnLayoutComponent>("ColumnLayoutComponent", 1, 0, "ColumnLayoutComponent"); // SimpleComponent
    qmlRegisterType<StackLayoutComponent>("StackLayoutComponent", 1, 0, "StackLayoutComponent"); // SimpleComponent
    qmlRegisterType<SubEffectListComponent>("SubEffectListComponent", 1, 0, "SubEffectListComponent"); // SimpleComponent
    qmlRegisterType<MutliInputTextComponent>("MutliInputTextComponent", 1, 0, "MutliInputTextComponent");
    qmlRegisterType<TTSComponent>("TTSComponent", 1, 0, "TTSComponent");
    qmlRegisterType<SwitchComponent>("SwitchComponent", 1, 0, "SwitchComponent");
    qmlRegisterType<DropdownListComponent>("DropdownListComponent", 1, 0, "DropdownListComponent");
    qmlRegisterType<VolumeFadeComponent>("VolumeFadeComponent", 1, 0, "VolumeFadeComponent");
    qmlRegisterType<EffectInputListComponent>("EffectInputListComponent", 1, 0, "EffectInputListComponent");
    qmlRegisterType<EffectGroupEditorComponent>("EffectGroupEditorComponent", 1, 0, "EffectGroupEditorComponent");
    qmlRegisterType<UploadEffectController>("UploadEffectController", 1, 0, "UploadEffectController");
    qmlRegisterType<ParticleValueComponent>("ParticleValueComponent", 1, 0, "ParticleValueComponent");
    qmlRegisterType<ParticleColorComponent>("ParticleColorComponent", 1, 0, "ParticleColorComponent");
    qmlRegisterType<SaveOfEffectComponent>("SaveOfEffectComponent", 1, 0, "SaveOfEffectComponent");
    qmlRegisterType<ParticleFileParserComponent>("ParticleFileParserComponent", 1, 0, "ParticleFileParserComponent");

}

QQmlApplicationEngine *QmlUtilities::sharedEngine() {
    static QQmlApplicationEngine *s_engine = new QQmlApplicationEngine();
    return s_engine;
}
