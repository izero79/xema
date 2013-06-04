#include <QDebug>

#include "dataloader.h"
#include "historymodel.h"
#include "modeldataloader.h"
#include "xemaenums.h"

DataLoader::DataLoader(int type, QObject *parent) :
    QObject(parent),
    mType(type)
{

}

void DataLoader::setHistoryModel(HistoryModel *model) {
    mHistoryModel = model;
}

void DataLoader::run()  {
    if (mType == XemaEnums::LOAD_HISTORY_DATE) {
        mModelLoader = ModelDataLoader::instance();
        mModelLoader->loadHistoryDateData(mHistoryModel);
        qDebug() << "done";
        emit historyLoaded();
    }
}
