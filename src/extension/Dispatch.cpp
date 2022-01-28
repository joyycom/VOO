#include "Dispatch.h"
#include "VooExtension.h"
#include "api/Dispatch_Tasks.h"
#include "api/Dispatch_Window.h"

void Dispatch::exec(QWebSocket *websocket, const QString &message) {
    RecvBody recv = VooExtension::messageToRecv(message);
    if (recv.api != API_VooEnter) {
        QVariant successFlag = websocket->property(EVENT_EnterSuccess);
        if (!successFlag.toBool()) {
            websocket->close();
            return;
        }
    }
    //任务类API
    if (recv.api == API_VooEnter) {
        websocket->setProperty(EVENT_EnterSuccess, true);
        emit sendSocket(VooExtension::wrapSend(EVENT_EnterSuccess));
    } else if (recv.api == API_PrintLog) {
        PrintLog::exec(recv.args);
    }
    //窗口类API
    else if (recv.api == API_ReloadEffect) {
        ReloadEffect::exec(recv.args);
    } else if (recv.api == API_ShowMiniPlayer) {
        ShowMiniPlayer::exec(recv.args);
    } else if (recv.api == API_HideMiniPlayer) {
        HideMiniPlayer::exec(recv.args);
    }
}
