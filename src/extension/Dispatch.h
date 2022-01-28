#include <QObject>
#include <QWebSocket>

class Dispatch : public QObject {
    Q_OBJECT

public:
    void exec(QWebSocket *websocket, const QString &message);
signals:
    void sendSocket(const QString &message);
};
