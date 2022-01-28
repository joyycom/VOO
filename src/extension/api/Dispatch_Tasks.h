#include <QJsonValue>

/*
 *打印日志
 */
class PrintLog {

public:
    static void exec(const QJsonValue &data);

private:
    enum LogLevel { Debug, Info, Warn, Error };
};
