#include <QDebug>

#include "tiiraexporter.h"
#include "tiiraservicehelper.h"

TiiraExporter::TiiraExporter(const QNetworkConfiguration &config, QObject *parent) :
    QObject(parent),
    mTiiraServiceHelper(0)
{
    mTiiraServiceHelper = new TiiraServiceHelper("iZero", "37e0a47e842a33948d804f26ece2aa8f", this);
    mTiiraServiceHelper->init(config);
    connect(mTiiraServiceHelper,SIGNAL(uploadOk(long,QString)),this,SLOT(uploadOk(long,QString)));
    connect(mTiiraServiceHelper,SIGNAL(rowUploadOk(long,int)),this,SLOT(rowUploadOk(long,int)));
}

void TiiraExporter::exportRecord(long id) {
    QMap<QString,QString> map;
    map.insert("laji", "parmaj");
    map.insert("maara", "1");
    map.insert("pvm1", "29.05.2013");
    map.insert("kunta", "Alavieska");
    map.insert("paikka", "Akanneva");
    map.insert("x", "3373527");
    map.insert("y", "7120078");
    map.insert("havainnoijat", "Tero Siironen");
    map.insert("tallentaja", "Tero Siironen");
    map.insert("havaintoriveja", "2");
    map.insert("koord_tyyppi", "YKJ");
    mTiiraServiceHelper->uploadRecord(map, id);

}

void TiiraExporter::uploadOk(long id, const QString &csvId) {
    qDebug() << Q_FUNC_INFO << id << csvId;

    QMap<QString,QString> map;
    map.insert("havaintotunnus", csvId);
    map.insert("maara", "1");
    mTiiraServiceHelper->uploadRecordRow(map, id);
}

void TiiraExporter::rowUploadOk(long id, int row) {
    qDebug() << Q_FUNC_INFO << id << row;
}
