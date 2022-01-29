/*
 *  Copyright (C) 2009 Aleksey Palazhchenko
 *  Copyright (C) 2014 Sergey Shambir
 *  Copyright (C) 2016 Alexander Makarov
 *
 * This file is a part of Breakpad-qt library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef QBREAKPAD_HANDLER_H
#define QBREAKPAD_HANDLER_H

#include "BreakpadWrapper_global.h"

#include <QString>
#include <QUrl>
#include <QObject>
#include <QMutex>

namespace google_breakpad {
    class ExceptionHandler;
    class MinidumpDescriptor;
}

class BreakpadHandlerPrivate;

class CRASH_API BreakpadHandler : public QObject {
    Q_OBJECT

private:
    BreakpadHandler();
    ~BreakpadHandler();
public:
     static BreakpadHandler *GetInstance();

     void setUploadUrl(const QUrl &url);
     QString uploadUrl() const;

     void SetupBreakPad(const QString &path);
     QString GetBreakPadDumpPath() const;

     QStringList GetDumpFileList() const;
     void UploadDumpsFiles(QStringList dumpFiles);


     bool IsHadCrashedDumpFiles();
     void DeleteAllOfTheDumpFiles();

signals:
     void onCrash(bool is_success, QString dump_dir, QString dump_file_name); // this only work on macx

public:
    void CallOnCrash(bool is_success, QString dump_dir, QString dump_file_name);

private:
    BreakpadHandlerPrivate* d;
    static BreakpadHandler* instance;
    static QMutex mutex_;
};

#endif	// QBREAKPAD_HANDLER_H
