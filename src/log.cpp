#include "log.h"
#include <QString>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDir>
#include <QThread>
#include <QDebug>
//#ifdef Q_OS_WIN
//#else
#include <iostream>
#include <QTextCodec>
#include <QDir>
#include <src/base/utils/ThreadWroker.h>
#include <base/utils/AppPath.h>
#include <base/utils/ZipUtils.h>
#include <base/utils/threadpools.h>
//#endif

#define SINGLE_LOG_FILE_MAX_SIZE 200*1024*1024
#define TOTLE_LOG_ZIP_FILE_MAX_SIZE 200*1024*1024


static struct LogWrap {
    LogWrap() {
        reset();
    }
    ~LogWrap() {
        reset();
    }
    void reset() {
        isInit = false;
        level = "";
        if (ts != nullptr) {
            //delete ts;
            ts = nullptr;
        }
        if (file != nullptr) {
            file->flush();
            file->close();

            delete file;
            file = nullptr;
        }
    }
    bool isInit;
    QMutex locker;
    QString level;
    QString filePath;
    QFile *file;
    int logSize;
    QTextStream *ts = nullptr;
    ThreadWroker *worker = nullptr;
} s_logWrap;


void tryOpenLogFile(){
    if(s_logWrap.ts){
        return;
    }
    QDir logDir(AppPath::logPath());
    QStringList filters;
    auto curDate = QDate::currentDate().toString("yyyyMMdd");
    QString logFilter = QString("log_%1*.log*").arg(curDate);
    filters << logFilter;
    QStringList list = logDir.entryList(filters,QDir::Files|QDir::Readable,QDir::Name | QDir::IgnoreCase | QDir::Reversed);
    int fileIndex = list.size();
    if(list.size() > 0 ){
        QString lastFileName = list[0].mid(13,3);
        fileIndex = lastFileName.toInt() + 1;
    }
    s_logWrap.filePath = QString("%1/log_%2_%3.log").arg(AppPath::logPath()).arg(curDate).arg(fileIndex,3,10,QChar('0'));
    s_logWrap.file = new QFile(s_logWrap.filePath);
    if (s_logWrap.file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        s_logWrap.ts = new QTextStream(s_logWrap.file);
        s_logWrap.ts->setCodec(QTextCodec::codecForName("utf-8"));
        (*s_logWrap.ts << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    }
}


void compressLogFile(QString logPath){
    auto logFileInfo = QFileInfo(logPath);
    if(!logFileInfo.exists()){
        return;
    }
    QString zipPath = logPath + ".zip";
    ZipUtils::compressFile(logPath,zipPath);
    if(QFileInfo(zipPath).exists()){
        qDebug()<<" Compress "<<logPath<<" Success:"<<QFileInfo(logPath).size()<<" -> "<<QFileInfo(zipPath).size();
        QFile::remove(logPath);
    }
}

void deleteHistoryLogFiles(){
    QStringList filters;
    QString logFilter = QString("log_*.log.zip");
    filters << logFilter;
    QDir logDir(AppPath::logPath());
    auto list = logDir.entryInfoList(filters,QDir::Files|QDir::Readable,QDir::Name | QDir::IgnoreCase | QDir::Reversed);
    int curSize = 0;
    for(auto file:list){
        if(curSize > TOTLE_LOG_ZIP_FILE_MAX_SIZE){
            QFile::remove(file.absoluteFilePath());
            qDebug()<<"delete :"<<file.fileName();
        }else{
            curSize += file.size();
        }
    }
}

void startCompressTask(){
    ThreadPools::instance()->exec([=]{
        QStringList filters;
        QString logFilter = QString("log_*.log");
        filters << logFilter;
        QDir logDir(AppPath::logPath());
        auto list = logDir.entryInfoList(filters);
        for(auto file:list){
            if(file.absoluteFilePath() != s_logWrap.filePath){
                 compressLogFile(file.absoluteFilePath());
            }
        }

        deleteHistoryLogFiles();
    });
}



bool checkFileSizeOverSize(){
    if(s_logWrap.file && s_logWrap.file->size() > SINGLE_LOG_FILE_MAX_SIZE){
        if(s_logWrap.ts){
            (*s_logWrap.ts << "----------------------------------------------------------------------\n");
            s_logWrap.ts->flush();
            delete s_logWrap.ts;
            s_logWrap.ts = nullptr;
        }
        s_logWrap.file->close();
        delete s_logWrap.file;
        s_logWrap.file = nullptr;
        return true;
    }
    return false;
}


#define _TIME_ qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
// QStringList s_outputFileNames = QStringList() << "view.cpp" << "store.cpp";
void SEMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    //    bool isFilter = true;
    //    QString file(context.file);
    //    for (auto iter = s_outputFileNames.begin(); iter != s_outputFileNames.end(); ++iter) {
    //        if (file.contains(*iter)) {
    //            isFilter = false;
    //            break;
    //        }
    //    }
    //    if (isFilter) {
    //        return;
    //    }

    QString formatMsg;
    switch (type) {
    case QtDebugMsg:
        formatMsg = "Debug:" + msg;
        break;
    case QtInfoMsg:
        formatMsg = "Info:" + msg;
        break;
    case QtWarningMsg:
        formatMsg = "Warning:" + msg;
        break;
    case QtCriticalMsg:
        formatMsg = "Critical:" + msg;
        break;
    case QtFatalMsg:
        formatMsg = "Fatal:" + msg;
        break;
    default:
        return;
    }

    if (!s_logWrap.isInit) {
        s_logWrap.isInit = true;
        s_logWrap.level = "debug";
        QString separator = "\\";
#ifdef Q_OS_MAC
        separator = "//";
#endif
        if (s_logWrap.worker == nullptr) {
            s_logWrap.worker = new ThreadWroker();
            s_logWrap.worker->start();
        }
        startCompressTask();
    }


    const char *thisFile = context.file ? context.file : "";

    QString threadText =
            QStringLiteral("[thread:0x%1]").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));
    QString text = _TIME_ + threadText + QString("[%1:%2]%3\n").arg(thisFile).arg(context.line).arg(formatMsg);

    s_logWrap.worker->async([text] {
        tryOpenLogFile();
        std::cout << text.toUtf8().data() << std::endl;
        (*s_logWrap.ts << text);
        s_logWrap.logSize++;
        if (s_logWrap.logSize % 1 == 0) {
            s_logWrap.ts->flush();
        }
        if(checkFileSizeOverSize()){
            compressLogFile(s_logWrap.filePath);
        }
    });

}


