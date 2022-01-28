#ifndef BASEINPUTLISTCREATOR_H
#define BASEINPUTLISTCREATOR_H

#include <QString>
#include <QObject>


#include <skymedia/SkyTimeline.h>
#define FORCE_POSIX
#include <MMKV.h>


class BaseInputListCreator: public QObject{
    Q_OBJECT
public:
    BaseInputListCreator(const QString& rootDir,const QString& tempOutputDir,skymedia::SkyTimeline* timeline);

    virtual bool createInputListFile() = 0;

protected:
    QString m_rootDir;
    QString m_tempOutputDir;
    skymedia::SkyTimeline* m_newTimeline = nullptr;
    MMKV* m_mmkv = nullptr;

};

#endif // BASEINPUTLISTCREATOR_H
