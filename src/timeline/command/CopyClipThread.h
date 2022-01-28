#pragma once

#include <qthread.h>

#include <QUndoStack>

#include "../models/baseclip.h"

class CopyClipThread : public QThread {
    Q_OBJECT
public:
    static void copy(QUndoStack *stack, BaseClip *clip, double time);
    CopyClipThread(QUndoStack *stack, BaseClip *clip, double time);

    // QThread interface
protected:
    void run();

private:
    bool skyOutputImage(SkyResource resource, QString imagePath, double time);
    void outputImage(QString videoPath, QString imagePath, double time);
    QUndoStack *m_stack = nullptr;
    BaseClip * m_clip = nullptr;
    double m_copyFrameTime;
};
