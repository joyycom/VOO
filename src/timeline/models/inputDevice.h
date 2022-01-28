#include <QJsonObject>
#include <QVariantList>

#include "timeline/models/skyresourcebean.h"
class InputDevice {
public:
    static InputDevice makeInputDeviceFromJson(const QJsonObject &json);
    static InputDevice makeInputDevice(const SkyResourceBean &skyResourceBean, SkyClip *skyClip);
    bool isNull();
    QJsonObject toJson();
    void setId(qint64 id);
    qint64 getId();
    QString getPath();
    QString getTitle();
    void setPath(const QString &path);
    QString getType();
    void addObjName(const QString &name);
    void addUpdateParam(const QString &key, const QJsonValue &value);
    void addClipId(qint64 id);
    bool equalsClipId(qint64 id);
    void removeInfos();

private:
    void init(const QJsonObject &json);
    void init(const SkyResourceBean &skyResourceBean, SkyClip *skyClip);

private:
    QJsonObject m_data;
};
