#ifndef BaseRsp_H
#define BaseRsp_H

#include <QJsonValue>

class BaseRsp {

public:
    BaseRsp(const QString &response);

    int code() const;
    QString message() const;
    QJsonValue data() const;

    bool isSuccess();

private:
    int m_code;
    QString m_message;
    QJsonValue m_data;
};

#endif
