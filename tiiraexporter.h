#ifndef TIIRAEXPORTER_H
#define TIIRAEXPORTER_H

#include <QObject>

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
    void uploadRecord();
    void uploadRecordRow();

private slots:
    void requestFinished(QNetworkReply *reply);
    void loginRequestFinished(QNetworkReply *reply);
    void testUploadRequestFinished(QNetworkReply *reply);
    void provideAuthenication(QNetworkReply *reply, QAuthenticator *ator);

    QString envelopBodyStart() const;
    QString envelopBodyEnd() const;
    QString readName(const QString &data) const;

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
