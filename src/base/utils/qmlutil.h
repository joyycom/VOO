#include <QObject>

class QmlUtils : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE static int keyboardModifiers();
};
