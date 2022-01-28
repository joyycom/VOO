#include "VooSocketServer.h"

//任务类API
#define API_VooEnter "VooEnter"
#define API_PrintLog "PrintLog"
//窗口类API
#define API_ReloadEffect "ReloadEffect"
#define API_ShowMiniPlayer "ShowMiniPlayer"
#define API_HideMiniPlayer "HideMiniPlayer"

//任务类事件
#define EVENT_EnterSuccess "EnterSuccess"
#define EVENT_PrintLog "PrintLog"

//窗口类事件
#define EVENT_WindowChanged "WindowChanged"

struct RecvBody {
    QString api;
    QJsonValue args;
};
enum WindowType { Null = 0, Guide = 1, Editor = 2 };
class VooExtension {
public:
    static void init();
    static RecvBody messageToRecv(const QString &message);
    static QString wrapSend(const QString &event, const QJsonValue &data = QJsonValue());
    //业务方法
    static void printLog(int level, const QString &msg);
    static void windowChanged(WindowType type);
};
