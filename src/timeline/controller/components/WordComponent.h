#ifndef WORDCOMPONENT_H
#define WORDCOMPONENT_H

#include "BaseComponent.h"
#include <QObject>
#include <QColor>
#include <QtCore>
#include <QAbstractListModel>

class FontResourceBean {
public:
    FontResourceBean(const QJsonObject &jo);
    FontResourceBean();

    QString id() const;
    QString name() const;
    QString source() const;
    QString type() const;
    QString cover() const;
    QString preview() const;
    QString fileName() const;

    void download(std::function<void(const bool, const QString &, const QString &)> complateHandler);
    bool isDownload() const;

private:
    QString m_id;
    QString m_name;
    QString m_source;
    QString m_type;
    QString m_cover;
    QString m_preview;
    QString m_fileName;
};

class FontResourceListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum FontResourceModel {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        SourceRole = Qt::UserRole + 3,
        TypeRole = Qt::UserRole + 4,
        CoverRole = Qt::UserRole + 5,
        PreviewRole = Qt::UserRole + 6,
        FilenameRole = Qt::UserRole + 7,
        IsDownloadRole = Qt::UserRole + 8
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addData(const FontResourceBean &data);
    FontResourceBean dataWith(int index);
    void clearAll();

protected:
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<FontResourceBean> m_data;
};

class WordComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(QString wordContent READ wordContent WRITE setWordContent NOTIFY wordContentChanged(wordContent))
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(int fontFamilyIndex READ fontFamily WRITE setFontFamily NOTIFY fontFamilyIndexChanged)
    Q_PROPERTY(float textAlpha READ textAlpha WRITE setTextAlhpa NOTIFY textAlphaChanged)
    Q_PROPERTY(bool isTextBold READ isTextBold WRITE setTextBold NOTIFY textBoldChanged)
    Q_PROPERTY(bool isUnderline READ isUnderline WRITE setUnderline NOTIFY textUnderlineChanged)
    Q_PROPERTY(bool isTextItalic READ isTextItalic WRITE setTextItalic NOTIFY textItalicChanged)
    Q_PROPERTY(FontResourceListModel *fontResource READ fontResource NOTIFY fontResourceChanged)

    Q_PROPERTY(
        bool isBackgroundEnabled READ isBackgroundEnabled WRITE setBackgroundEnabled NOTIFY isBackgroundEnabledChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

    Q_PROPERTY(QColor stokeColor READ stokeColor WRITE setStokeColor NOTIFY stokeColorChanged)
    Q_PROPERTY(bool isStokeEnable READ isStokeEnable WRITE setStokeEnable NOTIFY isStokeEnableChanged)
    Q_PROPERTY(float stokeThickness READ stokeThickness WRITE setStokeThickness NOTIFY stokeThicknessChanged)

    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(bool isShadowEnable READ isShadowEnabled WRITE setShadowEnable NOTIFY isShadowEnabledChanged)
    Q_PROPERTY(float shadowBlur READ shadowBlur WRITE setShadowBlur NOTIFY shadowBlurChanged)
    Q_PROPERTY(float shadowDistance READ shadowDistance WRITE setShadowDistance NOTIFY shadowDistanceChanged)
    Q_PROPERTY(float shadowAngle READ shadowAngle WRITE setShadowAngle NOTIFY shadowAngleChanged)

    Q_PROPERTY(int textSpacing READ textSpacing WRITE setTextSpacing NOTIFY textSpacingChanged)
    Q_PROPERTY(int textLeading READ textLeading WRITE setTextLeading NOTIFY textLeadingChanged)

public:
    WordComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE FontResourceListModel *fontResource() const;

    Q_INVOKABLE QString wordContent() const;

    Q_INVOKABLE void setWordContent(QString text);

    Q_INVOKABLE QColor textColor() const;

    Q_INVOKABLE void setTextColor(QColor text);

    Q_INVOKABLE void setTextColorByUser(QColor preColor, QColor color);

    Q_INVOKABLE int fontFamily() const;

    Q_INVOKABLE QString currentName() const;

    Q_INVOKABLE bool isFontDownload() const;

    Q_INVOKABLE void setFontFamily(int index);

    Q_INVOKABLE void clickFontDownload(int family);

    Q_INVOKABLE float textAlpha() const;

    Q_INVOKABLE void setTextAlhpa(float alpha);

    Q_INVOKABLE void setTextAlhpaByUser(float preAlpha, float alpha);

    Q_INVOKABLE bool isTextBold() const;

    Q_INVOKABLE void setTextBold(bool isBold);

    Q_INVOKABLE bool isUnderline() const;

    Q_INVOKABLE void setUnderline(bool isUnderline);

    Q_INVOKABLE bool isTextItalic() const;

    Q_INVOKABLE void setTextItalic(bool isItalic);

    Q_INVOKABLE QColor backgroundColor();

    Q_INVOKABLE void setBackgroundColor(QColor color);

    Q_INVOKABLE void setBackgroundColorByUser(QColor preColor, QColor color);

    Q_INVOKABLE bool isBackgroundEnabled();

    Q_INVOKABLE void setBackgroundEnabled(bool enabled);

    Q_INVOKABLE bool isStokeEnable();

    Q_INVOKABLE void setStokeEnable(bool enabled);

    Q_INVOKABLE QColor stokeColor();

    Q_INVOKABLE void setStokeColor(QColor color);

    Q_INVOKABLE float stokeThickness();

    Q_INVOKABLE void setStokeThickness(float thickness);

    Q_INVOKABLE void setStokeThicknessByUser(float preThickness, float thickness);

    Q_INVOKABLE bool isShadowEnabled();

    Q_INVOKABLE void setShadowEnable(bool enabled);

    Q_INVOKABLE QColor shadowColor();

    Q_INVOKABLE void setShadowColor(QColor color);

    Q_INVOKABLE void setShadowColorByUser(QColor preColor, QColor curColor);

    Q_INVOKABLE float shadowBlur();

    Q_INVOKABLE void setShadowBlur(float blur);

    Q_INVOKABLE void setShadowBlurByUser(float preBlur, float blur);

    Q_INVOKABLE float shadowDistance();

    Q_INVOKABLE void setShadowDistance(float distance);

    Q_INVOKABLE void setShadowDistanceByUser(float preDistance, float distance);

    Q_INVOKABLE float shadowAngle();

    Q_INVOKABLE void setShadowAngle(float angle);

    Q_INVOKABLE void setShadowAngleByUser(float preAngle, float angle);

    Q_INVOKABLE int textSpacing();

    Q_INVOKABLE void setTextSpacing(int spacing);

    Q_INVOKABLE void setTextSpacingByUser(int preSpacing, int spacing);

    Q_INVOKABLE int textLeading();

    Q_INVOKABLE void setTextLeading(int leading);

    Q_INVOKABLE void setTextLeadingByUser(int preLeading, int leading);

    void onOfParamsChanged(QString key, SkyVariant skyVariant) override;

signals:
    void wordContentChanged(QString text);
    void textColorChanged(QColor color);
    void fontFamilyIndexChanged(int index, QString name, bool isExist);
    void textAlphaChanged(float color);
    void textBoldChanged(bool isBold);
    void textUnderlineChanged(bool isUnderline);
    void textItalicChanged(bool isItalic);

    void fontResourceChanged(FontResourceListModel *fontResource);
    void fontDownloading(bool downloading, int index);

    void isBackgroundEnabledChanged(bool enabled);
    void backgroundColorChanged(QColor color);

    void isStokeEnableChanged(bool enabled);

    void stokeThicknessChanged(float thickness);

    void stokeColorChanged(QColor color);

    void shadowColorChanged(QColor color);

    void isShadowEnabledChanged(bool enable);

    void shadowBlurChanged(float blur);

    void shadowDistanceChanged(float distance);

    void shadowAngleChanged(float angle);

    void textSpacingChanged(int spacing);

    void textLeadingChanged(int leading);

private:
    void loadFontFamiliesData(QString family);
    void onDownloadFontCateFinish(const QString &response, const QString &familyPath);
    void onSetFontFamily(const QString &fontPath, const SkyVariant &preFontPath);

    QString m_wordContent = "";
    QColor m_textColor = QColor(255, 255, 255);

    QColor m_backgroundColor = QColor(255, 255, 255, 0);
    bool m_isBackgroundColorEnabled = false;

    int mFontIndex = 0;

    FontResourceListModel *m_allFontResource = new FontResourceListModel;

    int mTextSpacing = 0;
    int mTextLeading = 0;

    bool m_isTextBold = false;
    bool m_isTextItalic = false;
    bool m_isTextUnderline = false;

    bool mStokeEnabled = false;
    QColor mStokeColor = QColor(0, 0, 0);
    float mStokeThickness = 30;

    bool mShadowEnabled = false;
    QColor mShadowColor = QColor(0, 0, 0);
    float mShadowBlur = 0.33;
    float mShadowDistance = 8;
    float mShadowAngle = -45;

    const char *KEY_TEXT = "0:Text";

    const char *KEY_TEXT_SIZE = "0:Text";
    // 3:ttf 字体文件的路径
    const char *KEY_FONT_PATH = "0:CustomFontPath";
    const char *KEY_SYSTEM_FONT_DIR = "0:SystemFontDir";
    const char *KEY_SYSTEM_FONT_NAMES = "0:SystemFontNames";
    // 4: 颜色
    const char *KEY_TEXT_COLOR = "0:Color1";
    // 5:描边
    const char *KEY_STROKE_ENABLE = "0:Outline1Enabled";
    const char *KEY_STROKE_COLOR = "0:Outline1Color1";
    const char *KEY_STROKE_THICKNESS = "0:Outline1Thickness";
    // 6:背景颜色
    const char *KEY_BACKGROUND_COLOR = "0:BackgroundColor";
    const char *KEY_BACKGROUND_ENABLED = "0:BackgroundEnabled";
    // 7:阴影
    const char *KEY_SHADOW_ENABLE = "0:Shadow";
    const char *KEY_SHADOW_COLOR = "0:ShadowColor";
    const char *KEY_SHADOW_BLUR = "0:ShadowBlur";
    const char *KEY_SHADOW_DISTANCE = "0:ShadowDistance";
    const char *KEY_SHADOW_ANGLE = "0:ShadowAngle";

    // 8:排列
    const char *KEY_ALIGNMENT = "0:Alignment";
    const char *KEY_SPACING = "0:Spacing";
    const char *KEY_LEADING = "0:Leading";
    // 9:粗斜体
    const char *KEY_BOLD = "0:Bold";
    const char *KEY_ITALIC = "0:Italic";
    const char *KEY_UNDERLINE = "0:Underline";
};

#endif // WORDCOMPONENT_H
