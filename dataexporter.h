#ifndef DATAEXPORTER_H
#define DATAEXPORTER_H

#include <QRunnable>
#include <QObject>

class HistoryModel;
class ModelDataLoader;

class DataExporter : public QObject, public QRunnable
{
    Q_OBJECT
public:
    DataExporter(int type, QObject *parent = 0 );
    void setHistoryModel(HistoryModel *model);

signals:
    void historyLoaded();

protected:
    void run();

private:
    int mType;
    HistoryModel *mHistoryModel;
    ModelDataLoader *mModelLoader;


};

#endif // DATAEXPORTER_H
