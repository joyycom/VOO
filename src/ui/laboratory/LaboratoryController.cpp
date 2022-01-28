#include "LaboratoryController.h"

#include <QDebug>
#include <QFileInfo>

#include "base/utils/AppConstance.h"
#include "base/utils/MemoryPool.h"
#include "base/utils/SharedPref.h"
#include "base/utils/threadpools.h"
#include "project/SEProject.h"
#include "timeline/venus/VenusManager.h"
#include <QPainter>
#include <vector>

static void VenusContext_dumpResultInfo(const VN2_Image &inputImg, const VenusContext& ctx, QString inputPath) {
    qDebug() << "dumpResultInfo!";
    
    if (ctx.faceCartoons != nullptr) {
        QImage image((uchar *)inputImg.data, inputImg.width, inputImg.height, QImage::Format_RGBA8888);
        QPainter resultPatiner(&image);
        for (int i = 0; i < ctx.faceCartoons->imgsNum; ++i) {
            QImage maskedCartoonImage(ctx.faceCartoons->imgsArr[i].width, ctx.faceCartoons->imgsArr[i].height, QImage::Format_RGBA8888);
            QImage maskImage((uchar *)(ctx.faceMasks->imgsArr[i].data),
                             ctx.faceMasks->imgsArr[i].width,
                             ctx.faceMasks->imgsArr[i].height,
                             QImage::Format_Alpha8);
            maskImage.save(inputPath + "_mask.png");
            QImage cartoonImage((uchar *)(ctx.faceCartoons->imgsArr[i].data),
                                ctx.faceCartoons->imgsArr[i].width,
                                ctx.faceCartoons->imgsArr[i].height,
                                QImage::Format_RGB888);
            cartoonImage.save(inputPath + "_cartoon.png");
            QRect rect(0, 0, ctx.faceCartoons->imgsArr[i].width, ctx.faceCartoons->imgsArr[i].height);
            maskedCartoonImage.fill(QColor(0, 0, 0, 0));
            QPainter maskedPatiner(&maskedCartoonImage);
            
            maskedPatiner.drawImage(rect, cartoonImage);
            maskedPatiner.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            maskedPatiner.drawImage(rect, maskImage);
            maskedPatiner.end();
            maskedCartoonImage.save(inputPath + "_cartoon2.png");
            
            QRect rect1(ctx.faceCartoons->imgsArr[i].rect.x0 * inputImg.width,
                        ctx.faceCartoons->imgsArr[i].rect.y0 * inputImg.height,
                        (ctx.faceCartoons->imgsArr[i].rect.x1 - ctx.faceCartoons->imgsArr[i].rect.x0) * inputImg.width,
                        (ctx.faceCartoons->imgsArr[i].rect.y1 - ctx.faceCartoons->imgsArr[i].rect.y0) * inputImg.height);
            resultPatiner.setCompositionMode(QPainter::CompositionMode_SourceOver);
            resultPatiner.drawImage(rect1, maskedCartoonImage);
            
            qDebug() << "Cartoon Pos:" << ctx.faceCartoons->imgsArr[i].rect.x0 << "," << ctx.faceCartoons->imgsArr[i].rect.y0
            << ", " << ctx.faceCartoons->imgsArr[i].rect.x1 << "," << ctx.faceCartoons->imgsArr[i].rect.y1;
            
            qDebug() << "Cartoon Pos:" << rect1;
        }
        resultPatiner.end();
        image.save(inputPath + "_cartoon.jpg");
    }
    
    for (auto &pair : ctx.splitDatas) {
        auto &key = pair.first;
        auto &imageData = pair.second;
        QImage maskImage((uchar *)(imageData.imgsArr[0].data),
                         imageData.imgsArr[0].width,
                         imageData.imgsArr[0].height,
                         imageData.imgsArr[0].channels == 1 ? QImage::Format_Alpha8 : QImage::Format_RGB888);
        maskImage.save(inputPath + QString::fromUtf8(key.c_str()) + ".png");
    }
}

LaboratoryController::LaboratoryController(QObject *parent) : QObject(parent) {
}

QString LaboratoryController::serverType() {
    return AppConstance::isTestServer() ? "测试环境" : "正式环境";
}

void LaboratoryController::changeServerEnv() {
    bool b = !AppConstance::isTestServer();
    AppConstance::setTestServer(b);
    emit serverEnvChanged(serverType());
}

QString LaboratoryController::country() {
    return AppConstance::country();
}

void LaboratoryController::changeOfPath(QUrl url) {
    auto file = url.toLocalFile();
    if (QFileInfo(file).exists()) {
        SharedPref::instance()->setString("of_exe_path", file);
        emit ofPathChanged(SharedPref::instance()->readString("of_exe_path"));
    }
}

void LaboratoryController::changeTimelineWatch(bool check) {
    emit SEProject::current()->dom()->timelineController()->watchChanged(check);
}

QString LaboratoryController::ofPath() {
    return SharedPref::instance()->readString("of_exe_path");
}

void LaboratoryController::testVenus(QUrl url) {
    if (!m_worker.isRunning()) {
        m_worker.start();
    }
    m_worker.async([url] {
        QImage image(url.toLocalFile(),"jpg,png,webp,bmp");

        std::vector<std::string> list;
//                list.push_back(FaceCartoonKey);
//                list.push_back(FaceMaskKey);
        list.push_back(ClothesSegmentKey);
        list.push_back(HeadSegmentKey);
        list.push_back(SkySegmentKey);
        list.push_back(SegmentFrameKey);
        list.push_back(AnimalSegmentKey);
        list.push_back(HairSegmentKey);

        std::vector<std::string> modes;
//                modes.push_back("venus2_cartface_data");
//                modes.push_back("venus2_faceparse_data");
        modes.push_back("venus2_clothesseg_data");
        modes.push_back("venus2_headseg_data");
        modes.push_back("venus2_skyseg_data");
        modes.push_back("venus2_segment_picture");
        modes.push_back("venus2_animalseg_data");
        modes.push_back("venus2_hairseg_data");

        if (image.format() == QImage::Format_Invalid) {
            qDebug() << "Image Format INVLAID:" << image.format();
            return;
        }

        if (image.width() * image.height() > 1600 * 800) {
            double scale = sqrt(1600.0 * 800 / (image.width() * image.height()));
            image = image.scaled(image.width() * scale, image.height() * scale);
        }

        VenusContext context(list, modes);
        qDebug() << "Image Format:" << image.format() << " Widht:" << image.width() << " Height:" << image.height();
        image = image.convertToFormat(QImage::Format_RGBA8888);
        qDebug() << "Image Format After:" << image.format();

        VN2_Image inputImage = VenusContext::fillInputImage(image.constBits(), image.width(), image.height());
        SkyVariant frames = VenusManager::sharedInstance().processFrame(inputImage, context);
        VenusContext_dumpResultInfo(inputImage, context, url.toLocalFile());

        MemoryPool::instance()->clear();
    });
}

void LaboratoryController::changeCountry(QString ct) {
    AppConstance::setCountry(ct);
    emit countryChanged(country());
}
