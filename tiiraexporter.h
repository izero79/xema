#ifndef TIIRAEXPORTER_H
#define TIIRAEXPORTER_H

#include <QObject>

class TiiraServiceHelper;
class QNetworkConfiguration;

class TiiraExporter : public QObject
{
    Q_OBJECT
public:
    explicit TiiraExporter(const QNetworkConfiguration &config, QObject *parent = 0);
    void exportRecord(long id);
    
signals:
    
public slots:

private slots:
    void uploadOk(long id, const QString &csvId);
    void rowUploadOk(long id, int row);

private:

    TiiraServiceHelper *mTiiraServiceHelper;

};

#endif // TIIRAEXPORTER_H
