#ifndef TIIRAEXPORTER_H
#define TIIRAEXPORTER_H

#include <QObject>
#include <QMap>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkConfiguration;
class QAuthenticator;

class TiiraExporter : public QObject
{
    Q_OBJECT
public:
    explicit TiiraExporter(const QString &username, const QString &pwdHash, QObject *parent = 0);
    
signals:
    void wrongCredientals();
    void noUploadRights();
    void loginOk(const QString &name);
    
public slots:
    void init(const QNetworkConfiguration &config);
    void testRequest();
    void login();
    void testUpload();
    void uploadRecord(const QMap<QString,QString> &map, long recordId);
    void uploadRecordRow(const QMap<QString,QString> &map, long recordId, int row);

private slots:
    void requestFinished(QNetworkReply *reply);
    void loginRequestFinished(QNetworkReply *reply);
    void testUploadRequestFinished(QNetworkReply *reply);
    void provideAuthenication(QNetworkReply *reply, QAuthenticator *ator);
    void uploadRequestFinished(QNetworkReply *reply);
    void uploadRowRequestFinished(QNetworkReply *reply);

    QString envelopBodyStart() const;
    QString envelopBodyEnd() const;
    QString readName(const QString &data) const;
    QString recordRowNodes(const QMap<QString,QString> &map) const;
    QString readCsvId(const QString &data) const;
    QString readRowCount(const QString &data) const;

private:

    QNetworkAccessManager *mNetAccessMgr;
    bool mReady;
    QString mServerUsername;
    QString mServerPassword;
    QByteArray mServerAddress;
    QString mUsername;
    QString mPwdhash;
    QString mUsersName;
    
};

#endif // TIIRAEXPORTER_H
