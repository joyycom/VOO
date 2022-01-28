#include "FashionlineInputListCreator.h"

#ifdef Q_OS_WINDOWS
#include <QAxObject>
#endif
#include <QDir>
#include <QImageReader>
#include <QSemaphore>

#include <base/utils/JsonUtils.h>
#include <base/utils/fileutils.h>
#include "base/utils/UploadUtils.h"


FashionlineInputListCreator::FashionlineInputListCreator(const QString& rootDir,const QString& tempOutputDir,skymedia::SkyTimeline* timeline,const QString& cfgDir)
    :BaseInputListCreator(rootDir,tempOutputDir,timeline),m_fashionlineCfgDir(cfgDir)
{
}



bool FashionlineInputListCreator::createInputListFile() {

    QString timeline_path = QDir::fromNativeSeparators(SEProject::current()->rootPath());

    float uiinfoPTS = 999999;
    QJsonArray gltfEffect;

    //新timeline拿出轨数据，旧timeline拿文件数据
    QMap<QString,QList<SkyEffect*>*> groupEffects = extractFashionLineGroupData();

    if(groupEffects.isEmpty()){
        qInfo()<<"processTemplateType FashionLineGroup group isEmpty";
        emit exportLogInfo(QtFatalMsg,"FashionLineGroup Group Info is Empty ");
        return false;
    }
    tryPrepareExportCvsData(groupEffects);

    QString cfgFiles[] = {"/preview.csv","/output.csv","/printing.csv","/pod.xlsx","/color.csv"};
    for(QString cfg:cfgFiles){
        if(!QFileInfo::exists(QDir::cleanPath(m_tempOutputDir + cfg))){
            emit exportLogInfo(QtFatalMsg,"Config File "+cfg+" is NOT Exists!");
            return false;
        }
    }

    exportPreviewConf(QDir::cleanPath(m_tempOutputDir + "/preview.csv"),groupEffects);
    exportOutputConf(QDir::cleanPath(m_tempOutputDir + "/output.csv"),groupEffects);

    exportInputConf(QDir::cleanPath(m_tempOutputDir + "/printing.csv"),
                    QDir::cleanPath(m_tempOutputDir + "/pod.xlsx"),
                    QDir::cleanPath(m_tempOutputDir + "/color.csv"),
                    groupEffects);

    for(auto group:groupEffects.keys()){
        auto list = groupEffects[group];
        if(list){
            delete list;
        }
    }
    groupEffects.clear();
    return true;
}

void FashionlineInputListCreator::tryPrepareExportCvsData(const QMap<QString,QList<SkyEffect*>*>& groupEffects){
    auto useDefaultCfg = groupEffects.size() == 1 && groupEffects.contains("GLTF");
    if(m_fashionlineCfgDir.length() == 0 || !QFileInfo::exists(m_fashionlineCfgDir)){
        if(useDefaultCfg){
            QString effectPath = QString::fromUtf8(groupEffects["GLTF"]->at(0)->getParam("effectPath").toString());
            QString relatePath = effectPath.left(effectPath.lastIndexOf('/'));
            m_fashionlineCfgDir = QDir::cleanPath(m_rootDir + "/"+relatePath);
        }else{
            m_fashionlineCfgDir = m_rootDir;
        }
    }
    qInfo()<<"tryPrepareExportCvsData Cfg Dir:"<<m_fashionlineCfgDir;
    if (QFile::exists(QDir::cleanPath(m_fashionlineCfgDir + "/output.csv"))) {
        QFile::copy(QDir::cleanPath(m_fashionlineCfgDir + "/output.csv"),
                    QDir::cleanPath(m_tempOutputDir + "/output.csv"));
    }else{
        if(useDefaultCfg){
            QFile::copy(QCoreApplication::applicationDirPath().append("/Configs/default_GLTF_output.csv"),
                        QDir::cleanPath(m_tempOutputDir + "/output.csv"));
        }
    }

    if (QFile::exists(QDir::cleanPath(m_fashionlineCfgDir + "/preview.csv"))) {
        QFile::copy(QDir::cleanPath(m_fashionlineCfgDir + "/preview.csv"),
                    QDir::cleanPath(m_tempOutputDir + "/preview.csv"));
    }else{
        if(useDefaultCfg){
            QFile::copy(QCoreApplication::applicationDirPath().append("/Configs/default_GLTF_preview.csv"),
                        QDir::cleanPath(m_tempOutputDir + "/preview.csv"));
        }
    }

    if (QFile::exists(QDir::cleanPath(m_fashionlineCfgDir + "/pod.xlsx"))) {
        QFile::copy(QDir::cleanPath(m_fashionlineCfgDir + "/pod.xlsx"), QDir::cleanPath(m_tempOutputDir + "/pod.xlsx"));
    }

    if (QFile::exists(QDir::cleanPath(m_fashionlineCfgDir + "/color.csv"))) {
        if(useDefaultCfg){
            appendCvsData(QDir::cleanPath(m_fashionlineCfgDir + "/color.csv"),
                          QDir::cleanPath(m_tempOutputDir + "/color.csv"),
                          "group,uitype",
                          "GLTF,ColorMaterialSelector");
        }else{
            QFile::copy(QDir::cleanPath(m_fashionlineCfgDir + "/color.csv"),
                        QDir::cleanPath(m_tempOutputDir + "/color.csv"));
        }
    }

    if (QFile::exists(QDir::cleanPath(m_fashionlineCfgDir + "/printing.csv"))) {
        if(useDefaultCfg){
            appendCvsData(QDir::cleanPath(m_fashionlineCfgDir + "/printing.csv"),
                          QDir::cleanPath(m_tempOutputDir + "/printing.csv"),
                          "group",
                          "GLTF");
        }else{
            QFile::copy(QDir::cleanPath(m_fashionlineCfgDir + "/printing.csv"),
                        QDir::cleanPath(m_tempOutputDir + "/printing.csv"));
        }
    }
}

void FashionlineInputListCreator::appendCvsData(QString fromPath, QString toPath, QString title,QString data){
    QFile from(fromPath);
    QFile to(toPath);
    if(from.open(QFile::ReadOnly) && to.open(QFile::WriteOnly)){
        QString titles = from.readLine();
        if(!titles.contains(title)){
            to.write((titles.trimmed()+","+title+"\r\n").toUtf8());
            while(from.canReadLine()){
                QString line = from.readLine();
                to.write((line.trimmed()+","+data+"\r\n").toUtf8());
            }
            to.flush();
        }
        from.close();
        to.close();
    }
}

QMap<QString,QList<SkyEffect*>*> FashionlineInputListCreator::extractFashionLineGroupData(){
    QMap<QString,QList<SkyEffect*>*> groupEffects;
    for (int i = 0; i < m_newTimeline->numberOfTracks(); i++) {
        SkyTrack *track = m_newTimeline->getTrackAt(i);
        for (int j = 0; j < track->numberOfClips(); j++) {
            SkyClip *clip = track->getClipAt(j);
            int count = clip->numberOfEffects();
            std::vector<SkyEffect*> effects(count);
            clip->getEffects(effects.data(), count);
            for (int effectsIndex = 0; effectsIndex < count; effectsIndex++) {
                QString fashionLineGroup = "";
                auto fashionLineGroupVariant = effects[effectsIndex]->getUserData("fashionLineGroup");
                auto resIsGLTF = effects[effectsIndex]->getUserData("ResIsGLTF");
                if(!fashionLineGroupVariant.isNull()){
                    fashionLineGroup = QString::fromUtf8(fashionLineGroupVariant.toString());

                }else {
                    fashionLineGroup = (!resIsGLTF.isNull() && resIsGLTF.toBool() == true)?"GLTF":"";
                }
                if(fashionLineGroup.isEmpty()){
                    continue;
                }
                QList<SkyEffect*>* effects2 = groupEffects[fashionLineGroup];
                if(effects2 == nullptr){
                    effects2 = new QList<SkyEffect*>();
                    groupEffects.insert(fashionLineGroup,effects2);
                }
                effects2->append(effects[effectsIndex]);
            }
        }
    }
    return groupEffects;
}

void FashionlineInputListCreator::exportPreviewConf(QString previewCsv,const QMap<QString,QList<SkyEffect*>*>& map){
    QMap<QString,int> curIndexMap;
    QJsonArray previewArray;
    QJsonArray previewCfgArray = FileUtils::readCsvFile(previewCsv);
    int previewIndex = 0;
    for(int i = 0;i<previewCfgArray.count();++i){
        QJsonObject previewCfgItem = previewCfgArray.at(i).toObject();
        QString groupName = previewCfgItem.value("group").toString("GLTF");
        QString type = previewCfgItem.value("type").toString("");
        QString title = previewCfgItem.value("title").toString("");
        int id = previewCfgItem.value("id").toString("-1").toInt();

        int count = previewCfgItem.value("count").toString("0").toInt();
        bool snapFrame = previewCfgItem.value("snapFrame").toString("0").toInt() == 1;

        previewCfgItem.remove("snapFrame");
        previewCfgItem.remove("count");
        previewCfgItem.remove("group");

        if(!map.contains(groupName)){
            qWarning()<<"Not Found Group "<<groupName << " For Preview "<<previewCfgItem;
            emit exportLogInfo(QtFatalMsg,"preview.csv Error! GROUP "+groupName+" is NOT EXIST in VOO!");
            continue;
        }

        int targetIndex = -1;
        QJsonObject previewItem;
        for(int i=0;i<previewArray.size();++i){
            if(previewArray[i].toObject().value("type").toString("") == type &&
                    previewArray[i].toObject().value("title").toString("") == title &&
                    previewArray[i].toObject().value("id").toInt(-1) == id){
                previewItem = previewArray[i].toObject();
                targetIndex = i;
                break;
            }
        }
        if(previewItem.isEmpty()){
            if(id <= 0){
                id = previewArray.size() + 1;
            }
            previewItem.insert("id",id);
            previewItem.insert("title",title);
            previewItem.insert("type",type);
        }

        QMap<QString, QString> fileUrls;

        QList<SkyEffect*>* effectList = map[groupName];
        int beginIndex = curIndexMap.value(groupName,0);
        int endIndex = fmin(count+beginIndex,effectList->count());

        if(type == "3D"){
            auto videoParams = m_newTimeline->getVideoParams();
            previewItem.insert("width",videoParams.width);
            previewItem.insert("height",videoParams.height);
        }

        QJsonArray setting = previewItem.value("setting").toArray();
        for(int i=beginIndex;i<endIndex;++i){
            QJsonObject obj;
            SkyEffect* curEffect = effectList->at(i);
            obj.insert("objName", QString::fromUtf8(curEffect->getName().c_str()));
            SkyClip* owner = findOwerClip(m_newTimeline, curEffect);
            SkyTimeRange range = owner->getTrackRange();
            if(range.end - range.begin < 0.1){
                emit exportLogInfo(QtWarningMsg,"preview.csv Error! GROUP "+groupName+" during is TOO short! "+(range.end - range.begin)+"s");
                continue;
            }

            double seekTime = (range.begin+range.end)/2;
            obj.insert("seek",seekTime);

            if(snapFrame){
                QString path = QString("%1/res/icon/icon_%2.png").arg(m_tempOutputDir).arg(previewIndex);
                path = QDir::cleanPath(path);
                path = saveSnapFrame(seekTime, path);
                if(!path.isEmpty()){
                    fileUrls.insert(path,"");
                }
                obj.insert("icon",path);
                ++previewIndex;
            }
            setting.append(obj);
        }


        uploadImageFilePath(&fileUrls);
        setting = replaceJsonObjectPath(setting, &fileUrls).toArray();


        previewItem.insert("setting",setting);
        if(targetIndex>=0){
            previewArray.replace(targetIndex,previewItem);
        }else{
            previewArray.append(previewItem);
        }

        curIndexMap.insert(groupName,endIndex);
    }

    // preview
    QJsonDocument jsonDoc1;
    jsonDoc1.setArray(previewArray);
    QFile previewFile(QDir::cleanPath(m_tempOutputDir + "/preview.conf"));
    if (previewFile.open(QIODevice::WriteOnly)) {
        QTextStream stream(&previewFile);
        stream << jsonDoc1.toJson() << endl;
        previewFile.close();
    }
}

void FashionlineInputListCreator::exportInputConf(const QString printingcvs,
                              QString podxls,
                              QString colorcvs,
                              const QMap<QString, QList<SkyEffect *> *> &map) {
    //    QJsonObject uiinfoObj = JsonUtils::jsonObjectFromFile(m_tempOutputDir + "/" + relatePath +
    //    "/uiinfo.conf").object();
//    auto isGltfConfig = map.size() == 1 && map.contains("GLTF");

    QMap<QString, QString> colorList;
    QJsonArray inputList;
    QJsonArray arrSize;
#ifdef Q_OS_WINDOWS

    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    excel.setProperty("DisplayAlerts", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open(const QString&)",QDir::cleanPath(podxls));
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");

    int sheet_count = 0;
    if(work_book != nullptr){
        QAxObject *work_sheets = work_book->querySubObject("Sheets");
        sheet_count = work_sheets->property("Count").toInt();
    }

    QJsonObject itSize;
    if (sheet_count == 1) {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
        QAxObject *used_range = work_sheet->querySubObject("UsedRange");

        QAxObject *columns = used_range->querySubObject("Columns");
        int column_count = columns->property("Count").toInt();
        int column_start = used_range->property("Column").toInt();

        QAxObject *rows = used_range->querySubObject("Rows");
        int row_count = rows->property("Count").toInt();
        int row_start = used_range->property("Row").toInt();

        if (row_count > 10) {
            int sizeRow = row_start + 6; // row = 7是尺码所在位置(cm)
            for (int col = 2; col < column_count; col++) {
                QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", sizeRow, col);
                QString size = cell->dynamicCall("Value2()").toString();
                if (size.isEmpty()) {
                    break;
                }

                if (itSize.isEmpty()) {
                    itSize["id"] = inputList.size() + 1;
                    itSize["title"] = "尺码";
                    itSize["type"] = "sizeChoose";
                    itSize["defValue"] = size;
                }
                QJsonObject size0;
                size0["name"] = size;
                arrSize.push_back(size0);
            }
            itSize["paramSettingInfo"] = arrSize;
            inputList.append(itSize);

            int colorRow = row_start + 1;       // row = 2是颜色所在位置
            int colorColumn = column_start + 2; // column = 3是颜色所在位置
            QAxObject *colorCell = work_sheet->querySubObject("Cells(int,int)", colorRow, colorColumn);
            QString colorString = colorCell->dynamicCall("Value2()").toString();
            QStringList lst = colorString.trimmed().split("\n");
            for (QString c : lst) {
                c = c.trimmed();
                if(c.contains("#")){
                    colorList.insert(c.split("#")[0].trimmed(), c.split("#")[1].trimmed());
                }
            }
        }
    }else if(sheet_count > 2){
         QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 2);
         QAxObject *used_range = work_sheet->querySubObject("UsedRange");

         QAxObject *columns = used_range->querySubObject("Columns");
         int column_count = columns->property("Count").toInt();
         int column_start = used_range->property("Column").toInt();

         QAxObject *rows = used_range->querySubObject("Rows");
         int row_count = rows->property("Count").toInt();
         int row_start = used_range->property("Row").toInt();
         for(int row = row_start;row<row_count;++row){
             for(int col = column_start;col<column_count;++col){
                 QAxObject *colorCell = work_sheet->querySubObject("Cells(int,int)", row, col);
                 QString colorString = colorCell->dynamicCall("Value2()").toString();
                 qInfo()<<"pod.xls "<<col<<","<<row<<" => "<<colorString;
             }
         }
         int sizeRow = 1;
         for (int col = 3; col <= column_count; col++) {
             QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", sizeRow, col);
             QString size = cell->dynamicCall("Value2()").toString();
             if (size.isEmpty()) {
                 break;
             }

             if (itSize.isEmpty()) {
                 itSize["id"] = inputList.size() + 1;
                 itSize["title"] = "尺码";
                 itSize["type"] = "sizeChoose";
                 itSize["defValue"] = size;
             }
             QJsonObject size0;
             size0["name"] = size;
             arrSize.push_back(size0);
         }
         itSize["paramSettingInfo"] = arrSize;
         inputList.append(itSize);

         int colorCol = 1;
         for (int row = 1; row <= row_count; row++) {
             QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", row, colorCol);
             QString colorName = cell->dynamicCall("Value2()").toString().trimmed();
             QAxObject *cell2 = work_sheet->querySubObject("Cells(int,int)", row, colorCol+1);
             QString color = cell2->dynamicCall("Value2()").toString();
             if ((colorName.isEmpty()||color.isEmpty()) && !color.contains("#[0-9 a-f]{6}{6}")) {
                 continue;
             }

             int leftIndex = color.indexOf('#');
             color = color.right(color.length() - leftIndex - 1);
             colorList.insert(colorName, color);
             qInfo()<<"parse Color:"<<colorName<<" : "<<color;
         }
    }

    work_books->dynamicCall("Close(Boolen)", false); // clock workbook
    excel.dynamicCall("Quit(void)");                 // close excel

#endif

    if(colorList.isEmpty() || arrSize.isEmpty()){
        emit exportLogInfo(QtFatalMsg,"pod.xls Error! ");

        return;
    }

    qInfo()<<"Read pod.xls Finish!";
    QMap<QString, QString> *fileUrls = new QMap<QString, QString>();
    QMap<QString, QString> hadProcessUvs;
    auto printingArray = FileUtils::readCsvFile(printingcvs);

    QList<QString> subPrintingItems;

    QMap<QString,QString> nameUvs;

    QList<QString> checkedImagePaths;

    bool isAllBinaryPrinting = true;

    for (int i = 0; i < printingArray.size(); i++) {
        auto item = printingArray.at(i).toObject();
        QString uv = item.value("uv").toString();
        if(uv != "binary_printing.png"){
            isAllBinaryPrinting = false;
            break;
        }
    }

    for (int i = 0; i < printingArray.size(); i++) {
        auto item = printingArray.at(i).toObject();
        QString name = item.value("name").toString();
        QString rot = item.value("rot").toString();
        QString size = item.value("size").toString();
        QString w = item.value("w").toString();
        QString h = item.value("h").toString();
        QString uv = item.value("uv").toString();
        QString alpha = item.value("alpha").toString();
        QString mask = item.value("mask").toString();
        QString icon = item.value("icon").toString();
        QString group = item.value("group").toString();
        QString ptsArr = item.value("pts").toString();
        QString groupsStr = item.value("groups").toString();

        if(group.isEmpty() && groupsStr.isEmpty() && uv.isEmpty() && name.isEmpty()){
            continue;
        }

        QStringList groups;
        if(groupsStr.isEmpty()){
            groups.append(group);
        }else{
            auto list = groupsStr.split(",");
            for(auto item:list){
                if(item.trimmed().size() > 0){
                    groups.append(item.trimmed());
                }
            }
        }


        QString icon_selected = item.value("icon_select").toString();
        QString background = item.value("background").toString();
        int backgroundScale = 1;
        if (background.indexOf("@") > -1 && background.toLower().indexOf("x") > -1) {
            QStringList tmp = background.split("@");
            backgroundScale = tmp.at(1).toLower().split("x").at(0).toUInt();
        }

        QString backgroundMask = item.value("background_mask").toString();

        //裁片
        QJsonObject it;
        for (int j = 0; j < inputList.size(); j++) {
            if (inputList.at(j).toObject()["title"].toString() == name) {
                it = inputList.at(j).toObject();
                inputList.removeAt(j);
                break;
            }
        }

        QJsonArray paramSettingInfo;
        QJsonArray paramAssembleList;
        QString relatePath = "";
        QList<SkyEffect *> effectList;

        QString uvPath = uv;
        int groupWidth = -1;
        int groupHeight = -1;
        if(group == "AssembleList" || groups.contains("AssembleList")){
            if(!QFileInfo::exists(m_fashionlineCfgDir+"/"+uv)){
                emit exportLogInfo(QtFatalMsg,"printing.csv Error: File "+uv+" is NOT Exist! "+(m_fashionlineCfgDir+"/"+uv));
                return;
            }

            FileUtils::copyFile(m_fashionlineCfgDir+"/"+uv,m_tempOutputDir+"/"+uv,true);

            auto assembleConfigStr = FileUtils::readJson(m_tempOutputDir+"/"+uv);
            auto assembleConfigs = QJsonDocument::fromJson(assembleConfigStr.toUtf8()).array();

            for(auto cfg:assembleConfigs){
                auto cfgObj = cfg.toObject();
                if(cfgObj.value("title").toString() == name){
                    paramAssembleList = cfgObj.value("groups").toArray();
                    for(auto item:paramAssembleList){
                        subPrintingItems.append(item.toObject().value("title").toString(""));
                    }
                    groupWidth = cfgObj.value("width").toInt(-1);
                    groupHeight = cfgObj.value("height").toInt(-1);
                }
            }
        }else{
            for(auto group:groups){
                auto list = map[group];
                if(list != nullptr && list->size()>0){
                    for(auto effect:*list){
                        effectList.append(effect);
                    }
                }

            }

            if(effectList.isEmpty()){
                qWarning()<<"Canot found group :"<<group;
                emit exportLogInfo(QtFatalMsg,"printing.csv Error! GROUP "+group+" is NOT EXIST in VOO!");
                continue;
            }

            qInfo()<<"process printing.cvs "<<group;

            QString effectPath = QString::fromUtf8(effectList.at(0)->getParam("effectPath").toString());
            relatePath = effectPath.left(effectPath.lastIndexOf('/'));

            if(!it.isEmpty() && it.contains("paramSettingInfo")){
                paramSettingInfo = it["paramSettingInfo"].toArray();
            }

            QString uvKey = QString("%1_%2").arg(effectList.at(0)->getName().c_str()).arg(uv);
            if(!hadProcessUvs.contains(uvKey)){
                if(!nameUvs.contains(name)){
                    FileUtils::copyFile(m_fashionlineCfgDir+"/"+uv,m_tempOutputDir+"/"+uv,true);
                    if(QFileInfo::exists(m_tempOutputDir + "/"+relatePath + "/"+uvPath)){
                        uvPath = relatePath + "/"+uvPath;
                        nameUvs[name] = uvPath;
                    } else if(QFileInfo::exists(m_tempOutputDir + "/"+uvPath) && !isAllBinaryPrinting){
                        nameUvs[name] = uvPath;
                    }
                }else{
                    uvPath = nameUvs[name];
                }


                //裁片参数
                QJsonObject uiinfoObj =
                    JsonUtils::jsonObjectFromFile(m_tempOutputDir + "/" + relatePath + "/uiinfo.conf").object();
                if (!uiinfoObj.isEmpty()) {
                    QFileInfo uvDir(uv);
                    QString uvFileName = uvDir.fileName();
                    QJsonArray paramList = uiinfoObj["paramList"].toArray();
                    for (int k = 0; k < paramList.size(); k++) {
                        if ((paramList[k].toObject()["uiType"].toString() == "Image" ||
                             paramList[k].toObject()["uiType"].toString() == "BinaryImage") &&
                                (paramList[k].toObject()["defValue"].toString() == uvFileName ||
                                  (paramList[k].toObject()["defValue"].toString() == "binary_printing.png" &&
                                   group != "GLTF"))) {
                            QJsonArray ar = paramList[k].toObject()["paramSettingInfo"].toArray();

                            for(int index=0;index<effectList.count();++index){
                                auto effect = effectList.at(index);
                                QString effectName = QString::fromUtf8(effect->getName().c_str());
                                for (int l = 0; l < ar.size(); l++) {
                                    int filterIndex = ar[l].toObject()["filterIndex"].toInt();
                                    QString paramName = ar[l].toObject()["paramName"].toString();
                                    QString paramType = ar[l].toObject()["paramType"].toString();
                                    if (paramType.compare("binary", Qt::CaseInsensitive) == 0 ||
                                        paramType.compare("randomNum", Qt::CaseInsensitive) == 0) {
                                        QJsonObject s0;
                                        s0["objName"] = effectName;
                                        s0["filterIndex"] = filterIndex;
                                        s0["paramName"] = paramName;
                                        s0["paramType"] = paramType;
                                        paramSettingInfo.push_back(s0);
                                    } else if(paramType == "" || paramType.compare("string",Qt::CaseInsensitive) == 0){
                                        QString paramKey = QString("%1:%2").arg(filterIndex).arg(paramName);
                                        auto paramValue = effect->getParam("ofParam").find(paramKey.toUtf8().data());
                                        auto paramValueStr = QString::fromUtf8(paramValue.toString());
                                        if(nameUvs.contains(name)){
                                            if(paramValueStr != uvPath){
                                                SkyVariant ofParam = effect->getParam("ofParam");
                                                SkyVariant path = SkyVariant::makePath(uvPath.toUtf8().data());
                                                ofParam.insert(paramKey.toUtf8().data(),path);
                                                SkyVariant effectParams = SkyVariant::makeObject();
                                                effectParams.insert("ofParam",ofParam);
                                                effect->updateParams(effectParams);
                                                qWarning()<<" UpdateParamValue:"<<paramKey<<" Value:"<<paramValueStr<<" -> "<<uvPath;
                                            }
                                        }else{
                                            if(!paramValue.isNull()){
                                                if(paramValueStr != uv){
                                                    qWarning()<<" UpdateParamValue:"<<paramKey<<" UV: "<<uv<<" -> "<<paramValueStr;
                                                    uv = paramValueStr;
                                                    uvPath = paramValueStr;
                                                    nameUvs[name] = paramValueStr;
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
                hadProcessUvs[uvKey] = uv;
            }else{
                uv = hadProcessUvs[uvKey];
            }
        }

        QString cfgResRelatePath = "";

        if(groups.contains(GLTF_GROUP) && !relatePath.isEmpty() && m_fashionlineCfgDir.contains(relatePath)){
            cfgResRelatePath = relatePath + "/";
        }else{
            FileUtils::copyFile(m_fashionlineCfgDir+"/"+alpha,m_tempOutputDir+"/"+alpha,true);
            FileUtils::copyFile(m_fashionlineCfgDir+"/"+mask,m_tempOutputDir+"/"+mask,true);
            FileUtils::copyFile(m_fashionlineCfgDir+"/"+background,m_tempOutputDir+"/"+background,true);
            FileUtils::copyFile(
                m_fashionlineCfgDir + "/" + backgroundMask, m_tempOutputDir + "/" + backgroundMask, true);
            FileUtils::copyFile(m_fashionlineCfgDir+"/"+icon,m_tempOutputDir+"/"+icon,true);
            FileUtils::copyFile(m_fashionlineCfgDir+"/"+icon_selected,m_tempOutputDir+"/"+icon_selected,true);
        }

        if (it.isEmpty()) {
            it["id"] = inputList.size() + 1;
            it["path"] =  uvPath;

            checkedImagePaths.append(uvPath);

            if(alpha.isEmpty()){
                it["alpha"] = "";
            }else{
                it["alpha"] =  cfgResRelatePath+alpha;
                checkedImagePaths.append(cfgResRelatePath+alpha);
            }
            if(mask.isEmpty()){
                it["mask"] = "";
            }else{
                it["mask"] = cfgResRelatePath+mask;
                checkedImagePaths.append(cfgResRelatePath+mask);

            }
            if (background.length() > 0) {
                it["background"] = cfgResRelatePath+background;
                it["backgroundScale"] = backgroundScale;
                checkedImagePaths.append(cfgResRelatePath+background);
            } else {
                it["background"] = "";
                it["backgroundScale"] = 1;
            }
            if (backgroundMask.length() > 0) {
                it["backgroundMask"] = cfgResRelatePath+backgroundMask;
                checkedImagePaths.append(cfgResRelatePath+backgroundMask);
            } else {
                it["backgroundMask"] = "";
            }
            it["icon"] = cfgResRelatePath+icon;
            it["iconHighlight"] = cfgResRelatePath+icon_selected;
//            fileUrls->insert(it["path"].toString(), "");
            fileUrls->insert(it["alpha"].toString(), "");
            fileUrls->insert(it["mask"].toString(), "");
            fileUrls->insert(it["icon"].toString(), "");
            fileUrls->insert(it["iconHighlight"].toString(), "");
            fileUrls->insert(it["background"].toString(), "");
            fileUrls->insert(it["backgroundMask"].toString(), "");
            it["title"] = name;

            if(!subPrintingItems.contains(name)){
               checkedImagePaths.append(cfgResRelatePath+icon);
               checkedImagePaths.append(cfgResRelatePath+icon_selected);
            }

            if(paramAssembleList.isEmpty()){
                QImageReader render(m_tempOutputDir + "/" +uvPath);
                it["w"] = render.size().width();
                it["h"] = render.size().height();
            }else{
                it["w"] = groupWidth;
                it["h"] = groupHeight;
            }

            QJsonArray ja;
            it["sizeSetting"] = ja;
            QJsonObject v1;
            v1["filterIndex"] = 0;
            v1["paramKey"] = "ModelRotY";
            v1["value"] = rot;
            it["updateParam"] = v1;
        }

        QJsonArray pts;
        if(ptsArr.isEmpty()){
            for(int index=0;index<effectList.count();++index){
                SkyClip* owner = findOwerClip(m_newTimeline, effectList.at(index));
                SkyTimeRange range = owner->getTrackRange();
                if(range.end - range.begin < 0.1){
                    continue;
                }
                double seekTime = (range.begin+range.end)/2;
                pts.append(seekTime);
            }
        }else{
            auto ptsStrList = ptsArr.trimmed().split(",");
            for(int index=0;index<ptsStrList.count();++index){
                auto seekTime = ptsStrList.at(index).toDouble();
                pts.append(seekTime);
            }
        }

        it["pts"] = pts;

        auto defaultIsHidden = subPrintingItems.contains(name);

        QJsonObject s0;
        s0["name"] = size;
        s0["alpha"] = alpha;
        s0["mask"] = mask;
        s0["width"] = w;
        s0["height"] = h;
        QJsonArray ja = it["sizeSetting"].toArray();
        ja.push_back(s0);
        it["isHidden"] = item.contains("isHidden")?item.value("isHidden").toString().compare("true",Qt::CaseInsensitive) == 0:defaultIsHidden;
        it["type"] = paramAssembleList.isEmpty()?"image":"Group";
        it["groupInfo"] = paramAssembleList;
        it["sizeSetting"] = ja;
        it["paramSettingInfo"] = paramSettingInfo;

        inputList.append(it);
    }

    for(int i=0;i<inputList.size();++i){
        auto inputObj = inputList.at(i).toObject();
        if(inputObj["type"] == "Group"){
            auto paramAssembleList = inputObj["groupInfo"].toArray();
            QJsonArray assembleList;
            int targetW = -1;
            int targetH = -1;

            for(int k=0;k<paramAssembleList.size();++k){
                auto title = paramAssembleList.at(k)["title"].toString();
                auto rect = paramAssembleList.at(k)["rect"].toArray();

                QJsonObject targetObj;
                for(int j = 0;j<inputList.size();++j){
                    if(inputList.at(j)["title"].toString("") == title){
                        targetObj = paramAssembleList.at(k).toObject();
                        targetObj.insert("id",inputList.at(j)["id"]);
                        int w = inputList.at(j)["w"].toInt();
                        int h = inputList.at(j)["h"].toInt();
                        if(w > 0 && h > 0 && rect.size() == 4){
                            targetW = w / rect[2].toDouble();
                            targetH = h / rect[3].toDouble();
                        }
                        break;
                    }
                }
                if(targetObj.isEmpty()){
                    emit exportLogInfo(QtWarningMsg,"AssembleList Config Error: "+inputObj["title"].toString()+" has NOT FOUND sub title:"+title);
                    break;
                }
                assembleList.append(targetObj);

            }
            if(inputObj["w"].toInt(-1) == -1 && inputObj["h"].toInt(-1) == -1){
                inputObj["w"] = targetW;
                inputObj["h"] = targetH;
            }
            inputObj.insert("groupInfo",assembleList);
            inputList.replace(i,inputObj);
        }
    }

    for(QString path:checkedImagePaths){
        if(!QFileInfo::exists(m_tempOutputDir + "/" + path)){
            emit exportLogInfo(QtWarningMsg,"printing.csv Error: NOT Found image "+path);
        }
    }


    uploadImageFilePath(fileUrls);
    inputList = replaceJsonObjectPath(inputList, fileUrls).toArray();

    auto colorInputCfg = generateInputColorConf(colorcvs,map,&colorList);
    colorInputCfg["id"] = inputList.size() + 1;
    inputList.append(colorInputCfg);

    QJsonDocument jsonDoc;
    jsonDoc.setArray(inputList);
    QFile outputFile(QDir::cleanPath(m_tempOutputDir + "/inputList.conf"));
    if (outputFile.open(QIODevice::WriteOnly)) {
        QTextStream stream(&outputFile);
        stream << jsonDoc.toJson() << endl;
        outputFile.close();
    }
}

QJsonObject FashionlineInputListCreator::generateInputColorConf(QString colorcvs,const QMap<QString,QList<SkyEffect*>*>& map, QMap<QString,QString>* colorList){
    QJsonArray colorConfigs = FileUtils::readCsvFile(colorcvs);

    QJsonObject colorInputCfg;
    QJsonArray colorJsonList;
    colorInputCfg["title"] = "颜色";
    colorInputCfg["type"] = "colorMaterial";

    for (int i = 0; i < colorConfigs.size(); i++) {
        QJsonObject colorCfg = colorConfigs.at(i).toObject();
        QString name = colorCfg.value("name").toString();
        QString path = colorCfg.value("path").toString().trimmed();
        QString group = colorCfg.value("group").toString();
        QString uitype = colorCfg.value("uitype").toString();
        bool isSkyResource = colorCfg.value("isSkyResource").toString(0).toInt() == 1;
        auto isGltfConfig =  group == GLTF_GROUP;

        if(name.isEmpty() && path.isEmpty() && group.isEmpty()){
            continue;
        }

        QString hex = "";
        bool finded = false;
        QMap<QString, QString>::iterator iter = colorList->begin();
        while (iter != colorList->end()) {
            if (name.compare(iter.key(),Qt::CaseInsensitive) == 0) {
                finded = true;
                hex = iter.value();
                break;
            }
            iter++;
        }
        if (!finded) {
            qInfo() << "cannot find color -> " << name;
            emit exportLogInfo(QtFatalMsg,"color.csv Error: NOT Found color:"+name + " in pod.xlsx");
            continue;
        }

        qInfo()<<"process color.cvs "<<name;

        if(!path.isEmpty() && QFileInfo::exists(m_fashionlineCfgDir+"/"+path)){
            if(!isGltfConfig){
                FileUtils::copyFile(m_fashionlineCfgDir+"/"+path, m_tempOutputDir + "/" + path,true);
            }
            if(!isSkyResource && !isGltfConfig){
                path = "../../"+path;
            }else{
                path = "/"+path;
            }
        }

        QJsonObject jojo;
        for(int j=0;j<colorJsonList.size();++j){
            if(colorJsonList.at(j).toObject()["name"] == name){
                jojo = colorJsonList.at(j).toObject();
                colorJsonList.removeAt(j);
                break;
            }
        }
        if(jojo["name"] != name){
            jojo["name"] = name;
            jojo["hex"] = hex;
            jojo["paramValue"] = path;
        }

        QJsonArray ja = jojo["paramValues"].toArray();
        ja.push_back(path);
        jojo["paramValues"] = ja;

        //                jojo["path"] = relatePath + "/" + path;
        //                fileUrls->insert(jojo["path"].toString(), "");
        //                QFileInfo smallIconFile = QFileInfo(m_rootDir + relatePath + "/" + path);
        //                QImage image(smallIconFile.filePath());
        //                if (image.width() > 100) {
        //                    image = image.scaledToWidth(100);
        //                } else if (image.height() > 100) {
        //                    image = image.scaledToHeight(100);
        //                }
        //                QString smallIconImage =
        //                    QFileInfo(m_tempOutputDir + relatePath + "/small_" + smallIconFile.fileName()).filePath();
        //                image.save(smallIconImage, smallIconFile.suffix().toUpper() == "JPG" ? "JPG" : "PNG");
        //                jojo["smallIcon"] = relatePath + "/" + QFileInfo(smallIconImage).fileName();
        //                fileUrls->insert(jojo["smallIcon"].toString(), "");
        colorJsonList.append(jojo);
    }

    if(colorJsonList.isEmpty()){
        emit exportLogInfo(QtFatalMsg,"color.csv Error: NOT Found invalid color:");

        return colorInputCfg;
    }

    colorInputCfg["materialList"] = colorJsonList;

    QJsonArray updateParams;
    QString processColor = "";
    for (int i = 0; i < colorConfigs.size(); i++) {
        QJsonObject colorCfg = colorConfigs.at(i).toObject();
        QString name = colorCfg.value("name").toString();
        QString path = colorCfg.value("path").toString();
        QString group = colorCfg.value("group").toString();
        QString uitype = colorCfg.value("uitype").toString();
        if(processColor != "" && processColor != name){
            continue;
        }
        processColor = name;

        QJsonArray updateParam;

        auto effectList = map[group];

        if(effectList == nullptr || effectList->isEmpty()){
            qWarning()<<"Canot found group :"<<group;
            emit exportLogInfo(QtFatalMsg,"color.csv Error! GROUP "+group+" is NOT EXIST in VOO!");
            continue;
        }
        QString effectPath = QString::fromUtf8(effectList->at(0)->getParam("effectPath").toString());

        QString relatePath = effectPath.left(effectPath.lastIndexOf('/'));

        QJsonObject uiinfoObj =
                JsonUtils::jsonObjectFromFile(m_tempOutputDir + "/" + relatePath + "/uiinfo.conf").object();
        QJsonArray paramList = uiinfoObj["paramList"].toArray();
        for(int eIndex=0;eIndex<effectList->size();++eIndex){
            QString effectName = QString::fromUtf8(effectList->at(eIndex)->getName().c_str());
            QJsonObject groupParams;
            groupParams["objName"] = effectName;
            for (int j = 0; j < paramList.size(); j++) {
                if (paramList[j].toObject()["uiType"].toString() == uitype) {
                    if(!groupParams["params"].isNull()){
                        qWarning()<<"Duplicate params UITYPE:"+uitype;
                        continue;
                    }
                    QJsonArray subParam;
                    auto paramObj = paramList[j].toObject();
                    QJsonArray colors = paramObj["paramSettingInfo"].toArray();
                    for (int k = 0; k < colors.size(); k++) {
                        QJsonObject s0;
                        QJsonObject curEffectItem = colors.at(k).toObject();
                        s0["filterIndex"] = curEffectItem["filterIndex"].toInt();
                        s0["paramKey"] = curEffectItem["paramName"];
                        if(curEffectItem.contains("paramType")){
                            s0["paramType"] = curEffectItem["paramType"];

                            if(curEffectItem["paramType"] == "SkyResource"){
                                groupParams["objName"] = QString::fromUtf8(
                                            findOwerClip(m_newTimeline, effectList->at(eIndex))->getName().c_str());
                            }
                        }else{
                        }

                        if(curEffectItem.contains("valueType")){
                            s0["valueType"] = curEffectItem["valueType"];
                        }else{
                            if(paramObj.contains("valueType")){
                                s0["valueType"] = paramObj["valueType"];
                            }else{
                                s0["valueType"] = "string";
                            }
                        }
                        subParam.push_back(s0);
                    }
                    groupParams["params"] = subParam;
                }
            }
            updateParam.append(groupParams);
        }

        updateParams.append(updateParam);
    }
    colorInputCfg["updateParams"] = updateParams;

    if(colorJsonList.size() > 0){
        QJsonObject defColorObj = colorJsonList.at(0).toObject();
        QJsonArray defColorParamValues = defColorObj["paramValues"].toArray();

        //    Q_ASSERT(updateParams.size() == defColorParamValues.size());

        for(int i=0;i<updateParams.size();++i){
            QJsonArray objParams = updateParams.at(i).toArray();
            for(int k=0;k<objParams.size();++k){
                QJsonObject objParam = objParams.at(k).toObject();
                QString targetObjName = objParam["objName"].toString();
                SkyObject* targetObj = m_newTimeline->findObjectByName(targetObjName.toUtf8().data());
                QJsonArray params = objParam["params"].toArray();
                QString paramValue = defColorParamValues.at(i).toString();
                for(int j=0;j<params.size();++j){
                    QJsonObject param = params.at(j).toObject();
                    if(param["filterIndex"].toInt(0) == -1 && param["paramType"] == "SkyResource"
                            && param["paramKey"] == "updateResource" && targetObj->getObjectType() == SkyObjectType_Clip){
                        auto targetResPath = QString::fromUtf8(((SkyClip*)targetObj)->getResource().getPath().c_str());
                        if(!targetResPath.endsWith(paramValue)){
                            FileUtils::copyFile(m_tempOutputDir + paramValue, targetResPath,true);
                            QString updateRes = targetResPath.remove(m_tempOutputDir);
                            SkyResource resource((m_tempOutputDir + paramValue).toUtf8());
                            ((SkyClip*)targetObj)->updateResource(resource);
                            qInfo()<<"Update Color Resource :"+paramValue<<" ->"<<updateRes;
                        }

                    }else if(param["filterIndex"].toInt(-1) >= 0 && targetObj->getObjectType() == SkyObjectType_Effect){
                        auto effect = ((SkyEffect*)targetObj);
                        SkyVariant ofParam = effect->getParam("ofParam");
                        QString paramKey = QString("%1:%2").arg(param["filterIndex"].toInt(-1)).arg(param["paramKey"].toString());
                        if(param["valueType"] == "float,float,float,float"){
                            auto colors = paramValue.split(",");
                            QColor c;
                            SkyVariant colorArr = SkyVariant::makeArray();
                            if (colors.count() == 4) {
                                colorArr.append(colors.at(0).toDouble());
                                colorArr.append(colors.at(1).toDouble());
                                colorArr.append(colors.at(2).toDouble());
                                colorArr.append(colors.at(3).toDouble());
                            }
                            ofParam.insert(paramKey.toUtf8().data(),colorArr);
                        }else{
                            ofParam.insert(paramKey.toUtf8().data(),paramValue.toUtf8().data());
                        }
                        SkyVariant effectParams = SkyVariant::makeObject();
                        effectParams.insert("ofParam",ofParam);
                        effect->updateParams(effectParams);
                        qInfo()<<"Update Param :"+paramKey<<" ->"<<paramValue;
                    }
                }
            }
        }
        defColorObj["paramValues"] = defColorParamValues;
        colorJsonList.replace(0,defColorObj);
        colorInputCfg["materialList"] = colorJsonList;

    }
    return colorInputCfg;
}

void FashionlineInputListCreator::exportOutputConf(QString outputCsv,const QMap<QString,QList<SkyEffect*>*>& map){
    QMap<QString,int> curIndexMap;
    QJsonArray outputArray;
    QJsonArray outputConfig = FileUtils::readCsvFile(outputCsv);
    for(int i = 0;i<outputConfig.count();++i){
        QJsonObject item ;

        QJsonObject outputItem = outputConfig.at(i).toObject();
        QString groupName = outputItem.value("group").toString("GLTF");
        int count = outputItem.value("count").toString("0").toInt();

        if(!map.contains(groupName)){
            qWarning()<<"Not Found Group "<<groupName << " For Preview "<<outputItem;
            emit exportLogInfo(QtFatalMsg,"output.csv Error! GROUP "+groupName+" is NOT EXIST in VOO!");
            continue;
        }

        QList<SkyEffect*>* effectList = map[groupName];
        int beginIndex = curIndexMap.value(groupName,0);
        int endIndex = fmin(count+beginIndex,effectList->count());

        QString exportParam = outputItem.value("exportParam").toString("");
        QJsonArray exportParamArrays;

        if(!exportParam.trimmed().isEmpty()){
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(exportParam.toUtf8(), &error);
            if(error.error == QJsonParseError::NoError && doc.isArray()){
                exportParamArrays = doc.array();
            }else{
                qWarning()<<"exportParam Error??:"<<exportParam;
                emit exportLogInfo(QtWarningMsg,"output.csv Error! exportParam : "+exportParam);
            }
        }

        QJsonArray setting;
        for(int i=beginIndex;i<endIndex;++i){
            QJsonObject obj;
            SkyEffect* curEffect = effectList->at(i);

            SkyClip* owner = findOwerClip(m_newTimeline, curEffect);
            SkyTimeRange range = owner->getTrackRange();

            if(range.end - range.begin < 0.1){
                emit exportLogInfo(QtWarningMsg,"Output.cvs Error! "+groupName+" during is too short! "+(range.end - range.begin)+"s");
                continue;
            }
            double seekTime = (range.begin+range.end)/2;
            obj.insert("pts",seekTime);
            obj.insert("type","image");

            QJsonArray paramSettingInfo;
            for(int j=0;j<exportParamArrays.size();++j){
                auto paramObj = exportParamArrays.at(j).toObject();
                paramObj.insert("objName",QString::fromUtf8(curEffect->getName().c_str()));
                paramSettingInfo.append(paramObj);
            }
            if(paramSettingInfo.count() > 0){
                obj.insert("paramSettingInfo",paramSettingInfo);
            }
            outputArray.append(obj);
        }

        curIndexMap.insert(groupName,endIndex);
    }

    QJsonDocument jsonDoc;
    jsonDoc.setArray(outputArray);
    QFile outputFile(QDir::cleanPath(m_tempOutputDir + "/output.conf"));
    if (outputFile.open(QIODevice::WriteOnly)) {
        QTextStream stream(&outputFile);
        stream << jsonDoc.toJson() << endl;
        outputFile.close();
    }
}

QString FashionlineInputListCreator::saveSnapFrame(double time,QString path){
    auto controller = SEProject::current()->dom()->timelineController();
    controller->setOutputMode(true);
    auto previewController = SEProject::current()->dom()->previewController();
    QImage image = previewController->getSkyTimelineView()->snapFrame(3, time);
    controller->setOutputMode(false);
    QFileInfo fileInfo(path);
    auto dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }
    if (!image.isNull()) {
        if (QFile::exists(path)) {
            QFile::remove(path);
        }
        if (image.width() > 400) {
            image = image.scaledToWidth(400);
        } else if (image.height() > 400) {
            image = image.scaledToHeight(400);
        }
        image.save(path, "PNG",80);

        path = path.replace(m_tempOutputDir + "/", "");
        return path;
    }
    return "";
}

void FashionlineInputListCreator::uploadImageFilePath(QMap<QString, QString> *fileUrls) {
    int uploadSize = fileUrls->size();
    QSemaphore *sem = new QSemaphore(uploadSize);
    for (QString key : fileUrls->keys()) {
        sem->acquire(1);
        QString filePath = m_tempOutputDir;
        if (key.startsWith("/")) {
            filePath = m_tempOutputDir + key;
        } else {
            filePath = m_tempOutputDir + "/" + key;
        }

        if(!QFileInfo::exists(filePath)){
            qWarning() << "Upload Failed! File:" << filePath << " NOT EXIST!";
            sem->release(1);
            continue;
        }
        QString fileMd5 = FileUtils::getFileMd5(filePath);
        std::string uploadedUrl;
        std::string uploadedKey = QString("Upload_%1").arg(fileMd5).toStdString();
        m_mmkv->getString(uploadedKey,uploadedUrl);
        if(uploadedUrl.length() > 0){
            auto url = QString::fromStdString(uploadedUrl);
            qWarning() << "Hit Cache Skip Uploaded File:" << filePath << " URL:"<<url;
            fileUrls->insert(key, url);
            sem->release(1);
            continue;
        }

        UploadUtils::upload(
                    filePath,
                    [=](qint64 bytesSent, qint64 bytesTotal) {
        },
        [fileUrls, key, sem, filePath,this,uploadedKey](const bool &success, const QString &str) {
            if (success) {
                fileUrls->insert(key, str);
                m_mmkv->set(str.toStdString(), uploadedKey);
                auto result = true; // QFile::remove(filePath);
                qWarning() << "Upload Success: Key:" << key << " LocalFile:" << filePath << " URL:" << str
                         << " result:" << result;
            } else {
                qWarning() << "Upload Failed! Key:" << key << " URL:" << str;
            }
            sem->release(1);
        },1);
    }
    sem->acquire(uploadSize);
    delete sem;
    qInfo() << "Upload ALl File Success!" << uploadSize;
}

QJsonValue FashionlineInputListCreator::replaceJsonObjectPath(QJsonValue json, QMap<QString, QString> *fileUrls) {
    if (json.isArray()) {
        auto jsonArray = json.toArray();
        QJsonArray newArray;
        for (int i = 0; i < jsonArray.size(); ++i) {
            newArray.append(replaceJsonObjectPath(jsonArray.at(i), fileUrls));
        }
        return newArray;
    } else if (json.isObject()) {
        auto jsonObj = json.toObject();
        QJsonObject newJsonObj;
        auto keys = jsonObj.keys();
        for (auto key : keys) {
            newJsonObj.insert(key, jsonObj[key]);
            if (jsonObj[key].isArray() || jsonObj[key].isObject()) {
                newJsonObj.insert(key, replaceJsonObjectPath(jsonObj[key], fileUrls));
            } else if (jsonObj[key].isString() && fileUrls->value(jsonObj[key].toString(), "").length() > 0) {
                newJsonObj.insert(key + "Url", fileUrls->value(jsonObj[key].toString(), ""));
            }
        }
        return newJsonObj;
    } else {
        return json;
    }
}


SkyClip* FashionlineInputListCreator::findOwerClip(SkyTimeline* timeline,SkyEffect * effect){
    return (SkyClip*)effect->getOwner();
}
