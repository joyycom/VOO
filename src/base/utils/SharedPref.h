#ifndef SharedPref_H
#define SharedPref_H

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#define FORCE_POSIX
#include "thirds/mmkv/MMKV.h"

class SharedPref {
public:
    static SharedPref *instance();
    SharedPref();
    SharedPref(MMKV *m_mmkv);
    ~SharedPref();

    // read
    QString readString(QString k);
    int readInt(QString k, int32_t defaultValue = 0);
    uint64_t readUInt64(QString k, uint64_t defaultValue = 0);
    bool readBool(QString k, bool defaultValue = false);
    QJsonObject readJson(QString k);
    QJsonArray readJsonArray(QString k);

    // write
    void setString(QString k, QString v);
    void setInt(QString k, int v);
    void setUInt64(QString k, uint64_t v);
    void setBool(QString k, bool v);
    void setJson(QString k, QJsonObject v);
    void setJsonArray(QString k, QJsonArray v);

    void removeValueForKey(QString k);

private:
    MMKV *m_mmkv = nullptr;
};
#endif // AppPath_H
