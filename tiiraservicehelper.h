#ifndef TIIRASERVICEHELPER_H
#define TIIRASERVICEHELPER_H

#include <QObject>
#include <QMap>
#include <QNetworkRequest>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkConfiguration;
class QAuthenticator;

class TiiraServiceHelper : public QObject
{
    Q_OBJECT
public:
    explicit TiiraServiceHelper(const QString &username, const QString &pwdHash, QObject *parent = 0);
    void resetServer(const QString &username, const QString &pwdHash);
    
signals:
    void wrongCredientals();
    void noUploadRights();
    void loginOk(const QString &name);
    void uploadOk(long id, const QString &csvId);
    void uploadFailed(const QString &error);
    void rowUploadOk(long id, int row);
    void serverLoginFailed();
    void loginFailUnknown();
    void adLoaded(const QString &iconUrl, const QString &url);
    void tiiraClosed();

public slots:
    void init(const QNetworkConfiguration &config);
    void login();
    void getAd();
    void uploadRecord(const QMap<QString,QString> &map, long recordId);
    void uploadRecordRow(const QMap<QString,QString> &map, long recordId);
    void removeId(long id);

private slots:
    void requestFinished(QNetworkReply *reply);
    void loginRequestFinished(QNetworkReply *reply);
    void adRequestFinished(QNetworkReply *reply);
    void provideAuthenication(QNetworkReply *reply, QAuthenticator *ator);
    void uploadRequestFinished(QNetworkReply *reply);
    void uploadRowRequestFinished(QNetworkReply *reply);

    QString envelopBodyStart() const;
    QString envelopAuth() const;
    QString envelopBodyEnd() const;
    QString readName(const QString &data) const;
    QString recordRowNodes(const QMap<QString,QString> &map) const;
    QString readCsvId(const QString &data) const;
    QString readRowCount(const QString &data) const;
    QString readError(const QString &data) const;

    void setServer();

private:

    QNetworkAccessManager *mNetAccessMgr;
    bool mReady;
    QString mServerUsername;
    QString mServerPassword;
    QByteArray mSoapServerAddress;
    QByteArray mServerAddress;
    QString mUsername;
    QString mPwdhash;
    QString mUsersName;
    QMap<long, QNetworkRequest> mRequests;
    QMap<long, QNetworkRequest> mRowRequests;
    bool mAuthSent;
    QString mTiiraAppKey;
};

#endif // TIIRASERVICEHELPER_H
