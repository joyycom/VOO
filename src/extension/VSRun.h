#include <QProcess>
#include <QMap>

//扩展文件名
#define EXTENSIONS_OF_EFFECT "of-effect-1.1.0.vsix"

class VSRun {
public:
    static void run(const QStringList &arguments = QStringList(),
                    const QStringList &extensions = QStringList(),
                    std::function<void(const bool)> callback = nullptr);

private:
    static void updateExtension(QProcess &process, const QString &extension, const QString &version);
};
