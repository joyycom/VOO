#include "WordComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QColor>
#include <QDir>
#include <QUrl>
#include <QTextCodec>
#include <base/utils/AppPath.h>
#include <base/utils/SkyVariantUtil.h>
#include "src/base/http/SEHttpClient.h"
#include "src/base/http/BaseRsp.h"
#include "src/base/utils/SharedPref.h"
#include "src/base/utils/AppConstance.h"
#include "src/base/utils/OrangeFilterUtils.h"

FontResourceBean::FontResourceBean(const QJsonObject &jo) {
    m_id = jo.value("id").toString();
    m_name = jo.value("name").toString();
    m_source = jo.value("source").toString();
    m_type = jo.value("type").toString();
    m_cover = jo.value("cover").toString();
    m_preview = jo.value("preview").toString();
    QUrl modelSource(m_source);
    m_fileName = modelSource.fileName();
}

FontResourceBean::FontResourceBean() {
}

QString FontResourceBean::id() const {
    return m_id;
}

QString FontResourceBean::name() const {
    return m_name;
}

QString FontResourceBean::source() const {
    return m_source;
}

QString FontResourceBean::type() const {
    return m_type;
}

QString FontResourceBean::cover() const {
    return m_cover;
}

QString FontResourceBean::preview() const {
    return m_preview;
}

QString FontResourceBean::fileName() const {
    return m_fileName;
}

void FontResourceBean::download(std::function<void(const bool, const QString &, const QString &)> complateHandler) {
    QString fontsDir = AppPath::bundleFontPath();
    QFile *file = new QFile(fontsDir + m_fileName);
    if (file->exists()) {
        file->remove();
    }
    file->open(QIODevice::ReadWrite);
    SEHttpClient(m_source).download(
        [=](const QByteArray &data) {
            file->write(data);
            qDebug() << "WordComponent  downloading...";
        },
        [=, filename = m_fileName] {
            file->flush();
            file->close();
            qDebug() << "WordComponent  downloaded...";
            complateHandler(true, fontsDir, filename);
        });
}

bool FontResourceBean::isDownload() const {
    QDir familyDir = QDir(AppPath::bundleFontPath());
    QStringList files = familyDir.entryList(QDir::Files);
    for (const QString &file : files) {
        if (file == m_fileName) {
            return true;
        }
    }
    return false;
}

int FontResourceListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant FontResourceListModel::data(const QModelIndex &index, int role) const {
    const FontResourceBean &fontData = m_data[index.row()];
    if (role == IdRole)
        return fontData.id();
    else if (role == NameRole)
        return fontData.name();
    else if (role == SourceRole)
        return fontData.source();
    else if (role == TypeRole) {
        return fontData.type();
    } else if (role == CoverRole) {
        return fontData.cover();
    } else if (role == PreviewRole) {
        return fontData.preview();
    } else if (role == FilenameRole) {
        return fontData.fileName();
    } else if (role == IsDownloadRole) {
        return fontData.isDownload();
    }
    return "";
}

void FontResourceListModel::addData(const FontResourceBean &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << data;
    endInsertRows();
}

FontResourceBean FontResourceListModel::dataWith(int index) {
    return m_data[index];
}

void FontResourceListModel::clearAll() {
    beginResetModel();
    m_data.clear();
    endResetModel();
}

QHash<int, QByteArray> FontResourceListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[SourceRole] = "source";
    roles[TypeRole] = "type";
    roles[CoverRole] = "cover";
    roles[PreviewRole] = "preview";
    roles[FilenameRole] = "fileName";
    roles[IsDownloadRole] = "isDownload";
    return roles;
}

WordComponent::WordComponent(QObject *parent) : BaseComponent(parent) {
}

void WordComponent::onBindQml(QJsonObject &paramInfo) {
    //绑定clip，监听文本变化
    connect(this, SIGNAL(wordContentChanged(QString)), m_curClip, SLOT(onWordContentChange(QString)));

//    const QMap<QString, SkyVariant> params = getOfParams();

    SkyVariant textVariant = getOfParamValue(KEY_TEXT);
    if (textVariant.isNull()) {
        m_wordContent = "Text";
        updateOfParamValueBySilent(KEY_TEXT, m_wordContent.toUtf8().data());
    } else {
        m_wordContent = QString::fromUtf8(textVariant.toString());
    }

    SkyVariant systemFontDirVariant = getOfParamValue(KEY_SYSTEM_FONT_DIR);
    if (systemFontDirVariant.isNull()) {
        updateOfParamValueBySilent(KEY_SYSTEM_FONT_DIR, AppPath::systemFontPath().toUtf8().data());
        updateOfParamValueBySilent(KEY_SYSTEM_FONT_NAMES, OrangeFilterUtils::getLocalTTF().toStdString().c_str());
    }

    qDebug() << " wordContent " << m_wordContent;

    QColor white(255, 255, 255, 255);
    SkyVariant colorRGB = getOfParamValue(KEY_TEXT_COLOR, "[ 255, 255, 255, 255 ]");
    m_textColor = SkyVariantUtil::variant2Color(colorRGB, white);
    m_isTextBold = (getOfParamValue(KEY_BOLD, false)).toBool();
    m_isTextUnderline = (getOfParamValue(KEY_UNDERLINE, false)).toBool();
    m_isTextItalic = (getOfParamValue(KEY_ITALIC, false)).toBool();

    QString fontPath = (getOfParamValue(KEY_FONT_PATH, "")).toString();
    loadFontFamiliesData(fontPath);

    QColor transparent(0,0,0,0);
    SkyVariant bgColorVariant = getOfParamValue(KEY_BACKGROUND_COLOR, "[ 0,0,0,0 ]");
    m_backgroundColor = SkyVariantUtil::variant2Color(bgColorVariant, transparent);
    m_isBackgroundColorEnabled = (getOfParamValue(KEY_BACKGROUND_ENABLED, m_isBackgroundColorEnabled)).toBool();

    QColor black(0, 0, 0, 255);
    mShadowEnabled = (getOfParamValue(KEY_SHADOW_ENABLE, mShadowEnabled)).toBool();
    SkyVariant shadowColor = getOfParamValue(KEY_SHADOW_COLOR, "");
    mShadowColor = SkyVariantUtil::variant2Color(shadowColor, black);
    mShadowAngle = (getOfParamValue(KEY_SHADOW_ANGLE, mShadowAngle)).toDouble();
    mShadowDistance = (getOfParamValue(KEY_SHADOW_DISTANCE, mShadowDistance)).toDouble();
    mShadowBlur = (getOfParamValue(KEY_SHADOW_BLUR, mShadowBlur)).toDouble();

    mStokeEnabled = (getOfParamValue(KEY_STROKE_ENABLE, mStokeEnabled)).toDouble();
    SkyVariant strokeColor = getOfParamValue(KEY_STROKE_COLOR, "[ 255, 255, 255, 255 ]");
    mStokeColor = SkyVariantUtil::variant2Color(strokeColor, black);
    mStokeThickness = (getOfParamValue(KEY_STROKE_THICKNESS, mStokeThickness)).toDouble();

    mTextLeading = (getOfParamValue(KEY_LEADING, mTextLeading)).toDouble();
    mTextSpacing = (getOfParamValue(KEY_SPACING, mTextSpacing)).toDouble();

    emit textColorChanged(m_textColor);
    emit wordContentChanged(m_wordContent);
    emit textBoldChanged(m_isTextBold);
    emit textItalicChanged(m_isTextItalic);
    emit textUnderlineChanged(m_isTextUnderline);

    emit backgroundColorChanged(m_backgroundColor);
    emit isBackgroundEnabledChanged(m_isBackgroundColorEnabled);

    emit shadowAngleChanged(mShadowAngle);
    emit isShadowEnabledChanged(mShadowEnabled);
    emit shadowColorChanged(mShadowColor);
    emit shadowBlurChanged(mShadowBlur);
    emit shadowDistanceChanged(mShadowDistance);

    emit stokeColorChanged(mStokeColor);
    emit stokeThicknessChanged(mStokeThickness);
    emit isStokeEnableChanged(mStokeEnabled);

    emit textSpacingChanged(mTextSpacing);
    emit textLeadingChanged(mTextLeading);

    qDebug() << "backgroundColorChanged:" << mShadowColor << " m_backgroundColor:" << m_backgroundColor;
    qDebug() << "mShadowEnabled:" << mShadowEnabled << " m_isBackgroundColorEnabled:" << m_isBackgroundColorEnabled;
}

void WordComponent::onUnBind() {
    delete m_allFontResource;
}

int WordComponent::fontFamily() const {
    return mFontIndex;
}

QString WordComponent::currentName() const {
    FontResourceBean model = m_allFontResource->dataWith(mFontIndex);
    QString name = model.name();
    return name;
}

bool WordComponent::isFontDownload() const {
    FontResourceBean model = m_allFontResource->dataWith(mFontIndex);
    bool status = model.isDownload();
    return status;
}

void WordComponent::setFontFamily(int family) {
    if (mFontIndex == family) {
        return;
    }
    mFontIndex = family;
    if (family == 0) {
        SkyVariant preFontPath = getOfParamValue(KEY_FONT_PATH);
        onSetFontFamily("", preFontPath);
    } else {
        SkyVariant preFontPath = getOfParamValue(KEY_FONT_PATH, "");
        QString fontPath =
            QString("%1%2").arg(AppPath::bundleFontPath()).arg(m_allFontResource->dataWith(family).fileName());
        FontResourceBean model = m_allFontResource->dataWith(family);
        if (model.isDownload()) {
            onSetFontFamily(fontPath, preFontPath);
        } else {
            emit fontDownloading(true, mFontIndex);
            model.download([=](const bool &success, const QString &path, const QString &name) {
                qDebug() << "Font_download: " << success << " path = " << path << "name = " << name;
                emit fontDownloading(false, mFontIndex);
                onSetFontFamily(fontPath, preFontPath);
            });
        }
    }
}

void WordComponent::clickFontDownload(int family) {
    FontResourceBean model = m_allFontResource->dataWith(family);
    emit fontDownloading(true, family);
    model.download([=](const bool &success, const QString &path, const QString &name) {
        qDebug() << "Font_download: " << success << " path = " << path << "name = " << name;
        emit fontDownloading(false, family);
    });
}

void WordComponent::onSetFontFamily(const QString &fontPath, const SkyVariant &preFontPath) {
    qDebug() << "setFontFamily:" << fontPath;
    updateOfParamValueByUser(KEY_FONT_PATH, SkyVariant::makePath(fontPath.toUtf8().data()), preFontPath);
    emit fontFamilyIndexChanged(mFontIndex, currentName(), isFontDownload());
}

QColor WordComponent::textColor() const {
    return m_textColor;
}

void WordComponent::setTextColor(QColor c) {
    SkyVariant rgb = SkyVariantUtil::color2Variant(c);
    if (updateOfParamValueByUser(KEY_TEXT_COLOR, rgb)) {
        m_textColor = c;
    }
    qDebug() << "setTextColor " << m_textColor.name(QColor::HexArgb);
    emit textColorChanged(m_textColor);
}

void WordComponent::setTextColorByUser(QColor preColor, QColor color) {
    SkyVariant rgb = SkyVariantUtil::color2Variant(color);
    SkyVariant preRgb = SkyVariantUtil::color2Variant(preColor);
    if (updateOfParamValueByUser(KEY_TEXT_COLOR, rgb, preRgb)) {
        m_textColor = color;
    }
    qDebug() << "setTextColor " << m_textColor.name(QColor::HexArgb);
    emit textColorChanged(m_textColor);
}

// bool saveFile(const QString &aFileName, char *data) { //用IODevice方式保存文本文件
//    QFile aFile(aFileName);
//    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Append))
//        return false;
//    // QByteArray  strBytes=str.toLocal8Bit();
//    aFile.write(data);
//    aFile.write("\r\n");
//    aFile.close();
//    return true;
//}

void WordComponent::setWordContent(QString content) {
    if (m_wordContent == content) {
        return;
    }
    if (updateOfParamValueByUser(KEY_TEXT, content.toUtf8().data())) {
        m_wordContent = content;
    }
    emit wordContentChanged(m_wordContent);
}

QString WordComponent::wordContent() const {
    return m_wordContent;
}

bool WordComponent::isTextBold() const {
    return m_isTextBold;
}

void WordComponent::setTextBold(bool isBold) {
    if (m_isTextBold == isBold) {
        return;
    }
    if (updateOfParamValueByUser(KEY_BOLD, isBold, m_isTextBold)) {
        m_isTextBold = isBold;
    }
    emit textBoldChanged(m_isTextBold);
}

bool WordComponent::isTextItalic() const {
    return m_isTextItalic;
}

void WordComponent::setTextItalic(bool isItalic) {
    if (m_isTextItalic == isItalic) {
        return;
    }
    if (updateOfParamValueByUser(KEY_ITALIC, isItalic, m_isTextItalic)) {
        m_isTextItalic = isItalic;
    }
    emit textItalicChanged(m_isTextItalic);
}

bool WordComponent::isUnderline() const {
    return m_isTextUnderline;
}

void WordComponent::setUnderline(bool isUnderline) {
    if (m_isTextUnderline == isUnderline) {
        return;
    }
    if (updateOfParamValueByUser(KEY_ITALIC, isUnderline, m_isTextUnderline)) {
        m_isTextUnderline = isUnderline;
    }
    emit textUnderlineChanged(m_isTextUnderline);
}

float WordComponent::textAlpha() const {
    return m_textColor.alphaF();
}

void WordComponent::setTextAlhpa(float alpha) {
    QColor c = QColor(m_textColor);
    c.setAlphaF(alpha);
    SkyVariant rgb = SkyVariantUtil::color2Variant(c);
    if (updateOfParamValueBySilent(KEY_TEXT_COLOR, rgb)) {
        m_textColor.setAlphaF(alpha);
    }
    qDebug() << "setTextColor " << m_textColor.name(QColor::HexArgb);
    emit textAlphaChanged(m_textColor.alphaF());
}

void WordComponent::setTextAlhpaByUser(float preAlpha, float alpha) {
    QColor c = QColor(m_textColor);
    c.setAlphaF(alpha);
    SkyVariant rgb = SkyVariantUtil::color2Variant(c);
    c.setAlphaF(preAlpha);
    SkyVariant rgbPre = SkyVariantUtil::color2Variant(c);
    if (updateOfParamValueByUser(KEY_TEXT_COLOR, rgb, rgbPre)) {
        m_textColor.setAlphaF(alpha);
    }
    qDebug() << "setTextColor " << m_textColor.name(QColor::HexArgb);
    emit textAlphaChanged(m_textColor.alphaF());
}

FontResourceListModel *WordComponent::fontResource() const {
    return m_allFontResource;
}

void WordComponent::loadFontFamiliesData(QString familyPath) {
    QString fontsDir = AppPath::bundleFontPath();
    mFontIndex = 0;
    m_allFontResource->clearAll();
    m_allFontResource->addData(FontResourceBean());
    QString cacheResponse = SharedPref::instance()->readString("cache_component_word_config");
    if (!cacheResponse.isEmpty()) {
        qDebug() << "getFont local:" << cacheResponse;
        onDownloadFontCateFinish(cacheResponse, familyPath);
    }

    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/material/listDynamicConfigs?type=Font")
        .get([=](const QString &response) {
            if (response != cacheResponse) {
                SharedPref::instance()->setString("cache_component_word_config", response);
                qDebug() << "getFont remote:" << response;
                this->onDownloadFontCateFinish(response, familyPath);
            }
        });
}

void WordComponent::onDownloadFontCateFinish(const QString &response, const QString &familyPath) {
    BaseRsp *rsp = new BaseRsp(response);
    if (rsp->isSuccess()) {
        QJsonArray fontItems = rsp->data().toObject().value("list").toArray();
        for (int i = 0; i < fontItems.count(); ++i) {
            QJsonObject fontItem = fontItems.at(i).toObject();
            FontResourceBean model(fontItem);
            QFileInfo familyFileInfo(familyPath);
            m_allFontResource->addData(model);
            if (model.fileName() == familyFileInfo.fileName()) {
                mFontIndex = i + 1;
            }
            // qDebug() << "font load " << mFontIndex << model.fileName() << " "<< familyFileInfo.fileName();
        }
    } else {
        qDebug() << rsp->message();
    }
    delete rsp;
    rsp = nullptr;
    emit fontResourceChanged(m_allFontResource);
    emit fontFamilyIndexChanged(mFontIndex, currentName(), isFontDownload());
}

QColor WordComponent::backgroundColor() {
    return m_backgroundColor;
}

void WordComponent::setBackgroundColor(QColor color) {
    if (m_backgroundColor == color) {
        return;
    }
    SkyVariant variant = SkyVariantUtil::color2Variant(color);
    if (updateOfParamValueBySilent(KEY_BACKGROUND_COLOR, variant)) {
        m_backgroundColor = color;
    }
    emit backgroundColorChanged(m_backgroundColor);
}

void WordComponent::setBackgroundColorByUser(QColor preColor, QColor color) {
    SkyVariant variant = SkyVariantUtil::color2Variant(color);
    SkyVariant preVariant = SkyVariantUtil::color2Variant(preColor);
    if (updateOfParamValueByUser(KEY_BACKGROUND_COLOR, variant, preVariant)) {
        m_backgroundColor = color;
    }
    emit backgroundColorChanged(m_backgroundColor);
}

bool WordComponent::isBackgroundEnabled() {
    return m_isBackgroundColorEnabled;
}

void WordComponent::setBackgroundEnabled(bool enabled) {
    if(enabled && m_backgroundColor == QColor(0,0,0,0)){
       QMap<QString,SkyVariant> preParams;
       preParams.insert(KEY_BACKGROUND_COLOR,SkyVariantUtil::color2Variant(m_backgroundColor));
       preParams.insert(KEY_BACKGROUND_ENABLED,m_isBackgroundColorEnabled);

       QColor white(255,255,255);
       QMap<QString,SkyVariant> curParams;
       curParams.insert(KEY_BACKGROUND_COLOR,SkyVariantUtil::color2Variant(white));
       curParams.insert(KEY_BACKGROUND_ENABLED,enabled);
       if(updateOfParamValuesByUser(curParams,preParams)){
           m_backgroundColor = white;
           m_isBackgroundColorEnabled = enabled;
       }
       emit isBackgroundEnabledChanged(m_isBackgroundColorEnabled);
       emit backgroundColorChanged(m_backgroundColor);
    }else{
         if(updateOfParamValueByUser(KEY_BACKGROUND_ENABLED, enabled, m_isBackgroundColorEnabled)){
             m_isBackgroundColorEnabled = enabled;
         }
         emit isBackgroundEnabledChanged(m_isBackgroundColorEnabled);
    }
}

bool WordComponent::isStokeEnable() {
    return mStokeEnabled;
}

void WordComponent::setStokeEnable(bool enabled) {
    if (updateOfParamValueByUser(KEY_STROKE_ENABLE, enabled, mStokeEnabled)) {
        mStokeEnabled = enabled;
    }
    emit isStokeEnableChanged(mStokeEnabled);
}

QColor WordComponent::stokeColor() {
    return mStokeColor;
}

void WordComponent::setStokeColor(QColor color) {
    SkyVariant variant = SkyVariantUtil::color2Variant(color);
    SkyVariant preVariant = SkyVariantUtil::color2Variant(mStokeColor);
    if (updateOfParamValueByUser(KEY_STROKE_COLOR, variant, preVariant)) {
        mStokeColor = color;
    }
    emit stokeColorChanged(mStokeColor);
}

float WordComponent::stokeThickness() {
    return mStokeThickness;
}

void WordComponent::setStokeThickness(float thickness) {
    if (updateOfParamValueBySilent(KEY_STROKE_THICKNESS, thickness)) {
        mStokeThickness = thickness;
    }
    emit stokeThicknessChanged(mStokeThickness);
}

void WordComponent::setStokeThicknessByUser(float preThickness, float thickness) {
    if (updateOfParamValueByUser(KEY_STROKE_THICKNESS, thickness, preThickness)) {
        mStokeThickness = thickness;
    }
    emit stokeThicknessChanged(mStokeThickness);
}

bool WordComponent::isShadowEnabled() {
    return mShadowEnabled;
}

void WordComponent::setShadowEnable(bool enabled) {
    //    QMap<QString, SkyVariant> paramMap;
    //    if(enabled){
    //        paramMap[KEY_SHADOW_ENABLE] = enabled;
    //        paramMap[KEY_SHADOW_COLOR] = color2Variant(mShadowColor);
    //        paramMap[KEY_SHADOW_BLUR] = mShadowBlur;
    //        paramMap[KEY_SHADOW_ANGLE] = mShadowAngle;
    //        paramMap[KEY_SHADOW_DISTANCE] = mShadowDistance;
    //    }else{
    //        paramMap[KEY_SHADOW_ENABLE] = enabled;
    //    }
    if (updateOfParamValueByUser(KEY_SHADOW_ENABLE, enabled, mShadowEnabled)) {
        mShadowEnabled = enabled;
    }
    emit isShadowEnabledChanged(mShadowEnabled);
}

QColor WordComponent::shadowColor() {
    return mShadowColor;
}

void WordComponent::setShadowColor(QColor color) {
    if (mShadowColor == color) {
        return;
    }
    SkyVariant variant = SkyVariantUtil::color2Variant(color);
    if (updateOfParamValueByUser(KEY_SHADOW_COLOR, variant)) {
        mShadowColor = color;
    }
    emit shadowColorChanged(mShadowColor);
}

void WordComponent::setShadowColorByUser(QColor preColor, QColor curColor) {
    SkyVariant variant = SkyVariantUtil::color2Variant(curColor);
    SkyVariant preVariant = SkyVariantUtil::color2Variant(preColor);
    if (updateOfParamValueByUser(KEY_SHADOW_COLOR, variant, preVariant)) {
        mShadowColor = curColor;
    }
    emit shadowColorChanged(mShadowColor);
}

float WordComponent::shadowBlur() {
    return mShadowBlur;
}

void WordComponent::setShadowBlur(float blur) {
    if (updateOfParamValueBySilent(KEY_SHADOW_BLUR, blur)) {
        mShadowBlur = blur;
    }
    emit shadowBlurChanged(blur);
}

void WordComponent::setShadowBlurByUser(float preBlur, float blur) {
    if (updateOfParamValueByUser(KEY_SHADOW_BLUR, blur, preBlur)) {
        mShadowBlur = blur;
    }
    emit shadowBlurChanged(blur);
}

float WordComponent::shadowDistance() {
    return mShadowDistance;
}

void WordComponent::setShadowDistance(float distance) {
    if (updateOfParamValueBySilent(KEY_SHADOW_DISTANCE, distance)) {
        mShadowDistance = distance;
    }
    emit shadowDistanceChanged(distance);
}

void WordComponent::setShadowDistanceByUser(float preDistance, float distance) {
    if (updateOfParamValueByUser(KEY_SHADOW_DISTANCE, distance, preDistance)) {
        mShadowDistance = distance;
    }
    emit shadowDistanceChanged(distance);
}

int WordComponent::textSpacing() {
    return mTextSpacing;
}

void WordComponent::setTextSpacing(int spacing) {
    if (updateOfParamValueBySilent(KEY_SPACING, spacing)) {
        mTextSpacing = spacing;
    }
    emit textSpacingChanged(mTextSpacing);
}

void WordComponent::setTextSpacingByUser(int preSpacing, int spacing) {
    if (updateOfParamValueByUser(KEY_SPACING, spacing, preSpacing)) {
        mTextSpacing = spacing;
    }
    emit textSpacingChanged(mTextSpacing);
}

int WordComponent::textLeading() {
    return mTextLeading;
}

void WordComponent::setTextLeading(int leading) {
    if (updateOfParamValueBySilent(KEY_LEADING, leading)) {
        mTextLeading = leading;
    }
    emit textLeadingChanged(mTextLeading);
}

void WordComponent::setTextLeadingByUser(int preLeading, int leading) {
    if (updateOfParamValueByUser(KEY_LEADING, leading, preLeading)) {
        mTextLeading = leading;
    }
    emit textLeadingChanged(mTextLeading);
}

float WordComponent::shadowAngle() {
    return mShadowAngle;
}

void WordComponent::setShadowAngle(float angle) {
    if (updateOfParamValueBySilent(KEY_SHADOW_ANGLE, angle)) {
        mShadowAngle = angle;
    }
    emit shadowAngleChanged(angle);
}

void WordComponent::setShadowAngleByUser(float preAngle, float angle) {
    if (updateOfParamValueByUser(KEY_SHADOW_ANGLE, angle, preAngle)) {
        mShadowAngle = angle;
    }
    emit shadowAngleChanged(angle);
}

void WordComponent::onOfParamsChanged(QString key, SkyVariant skyVariant) {
    if (key == KEY_TEXT) {
        m_wordContent = QString::fromUtf8(skyVariant.toString());
        emit wordContentChanged(m_wordContent);
    } else if (key == KEY_ALIGNMENT) {

    } else if (key == KEY_BACKGROUND_COLOR) {
        QColor color = SkyVariantUtil::variant2Color(skyVariant, m_backgroundColor);
        m_backgroundColor = color;
        emit backgroundColorChanged(m_backgroundColor);
        qDebug() << "onOfParamsChanged BackgroundColorChanged " << color;
    } if(key == KEY_BACKGROUND_ENABLED){
        m_isBackgroundColorEnabled = skyVariant.toBool();
        emit isBackgroundEnabledChanged(m_isBackgroundColorEnabled);
    }else if (key == KEY_BOLD) {
        m_isTextBold = skyVariant.toBool();
        emit textBoldChanged(m_isTextBold);
    } else if (key == KEY_FONT_PATH) {
        int fontIndex = 0;
        QString familyPath = QString::fromUtf8(skyVariant.toString());
        for (int i = 1; i < m_allFontResource->rowCount(); ++i) {
            QString path =
                QString("%1%2").arg(AppPath::bundleFontPath()).arg(m_allFontResource->dataWith(i).fileName());
            if (familyPath == path) {
                fontIndex = i;
                break;
            }
        }
        qDebug() << " onOfParamsChanged1111 FONT PATH:" << familyPath << " Index:" << fontIndex
                 << " CurIndex:" << mFontIndex;
        emit fontFamilyIndexChanged(fontIndex, currentName(), isFontDownload());
    } else if (key == KEY_LEADING) {
        mTextLeading = skyVariant.toInt();
        emit textLeadingChanged(mTextLeading);
    } else if (key == KEY_SHADOW_ANGLE) {
        mShadowAngle = skyVariant.toInt();
        emit shadowAngleChanged(mShadowAngle);
    } else if (key == KEY_SHADOW_BLUR) {
        mShadowBlur = skyVariant.toInt();
        emit shadowBlurChanged(mShadowBlur);
    } else if (key == KEY_SHADOW_COLOR) {
        mShadowColor = SkyVariantUtil::variant2Color(skyVariant, mShadowColor);
        emit shadowColorChanged(mShadowColor);
    } else if (key == KEY_SHADOW_DISTANCE) {
        mShadowDistance = skyVariant.toDouble();
        emit shadowDistanceChanged(mShadowDistance);
    } else if (key == KEY_SHADOW_ENABLE) {
        mShadowEnabled = skyVariant.toBool();
        emit isShadowEnabledChanged(mShadowEnabled);
    } else if (key == KEY_SPACING) {
        mTextSpacing = skyVariant.toInt();
        emit textSpacingChanged(mTextSpacing);
    } else if (key == KEY_STROKE_COLOR) {
        mStokeColor = SkyVariantUtil::variant2Color(skyVariant, mStokeColor);
        emit stokeColorChanged(mStokeColor);
    } else if (key == KEY_STROKE_ENABLE) {
        mStokeEnabled = skyVariant.toBool();
        qDebug() << " onOfParamsChanged KEY_STROKE_ENABLE:" << mStokeEnabled;
    } else if (key == KEY_STROKE_THICKNESS) {
        mStokeThickness = skyVariant.toDouble();
        emit stokeThicknessChanged(mStokeThickness);
    } else if (key == KEY_TEXT_COLOR) {
        m_textColor = SkyVariantUtil::variant2Color(skyVariant, m_textColor);
        qDebug() << " onOfParamsChanged TextColor:" << m_textColor;
        emit textColorChanged(m_textColor);
    } else if (key == KEY_UNDERLINE) {
        m_isTextUnderline = skyVariant.toBool();
        emit textUnderlineChanged(m_isTextUnderline);
    }
}
