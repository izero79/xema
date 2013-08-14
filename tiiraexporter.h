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
    
public slots:
    void init(const QNetworkConfiguration &config);
    void testRequest();
    void login();
    void uploadRecord();
    void uploadRecordRow();

private slots:
    void requestFinished(QNetworkReply *reply);
    void loginRequestFinished(QNetworkReply *reply);
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
    
};

#endif // TIIRAEXPORTER_H
