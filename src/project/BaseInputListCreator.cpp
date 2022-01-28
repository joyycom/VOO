#include "BaseInputListCreator.h"
#include "SEProject.h"

BaseInputListCreator::BaseInputListCreator(const QString& rootDir,const QString& tempOutputDir,skymedia::SkyTimeline* timeline):
    m_rootDir(rootDir),m_tempOutputDir(tempOutputDir),m_newTimeline(timeline)
{
    m_mmkv = MMKV::mmkvWithID((SEProject::current()->title() + ".export").toStdString());
}
