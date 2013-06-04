#include <QDebug>

#include "dataexporter.h"
#include "historymodel.h"
#include "modeldataloader.h"
#include "xemaenums.h"

DataExporter::DataExporter(int type, QObject *parent) :
    QObject(parent),
    mType(type)
{

}

void DataExporter::setHistoryModel(HistoryModel *model) {
    mHistoryModel = model;
}

void DataExporter::run()  {
    if (mType == XemaEnums::LOAD_HISTORY_DATE) {
        mModelLoader = ModelDataLoader::instance();
        mModelLoader->loadHistoryDateData(mHistoryModel);
        qDebug() << "done";
        emit historyLoaded();
    }
}

