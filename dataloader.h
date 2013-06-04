#ifndef DATALOADER_H
#define DATALOADER_H

#include <QRunnable>
#include <QObject>

class HistoryModel;
class ModelDataLoader;

class DataLoader : public QObject, public QRunnable
{
    Q_OBJECT
public:
    DataLoader(int type, QObject *parent = 0 );
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

#endif // DATALOADER_H
