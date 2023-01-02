#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QStringList>


#include "tiiraservicehelper.h"
#include "tiirachecksumcalculator.h"
#include "settings.h"

TiiraServiceHelper::TiiraServiceHelper(const QString &username, const QString &pwdHash, QObject *parent) :
    QObject(parent),
    mNetAccessMgr(0),
    mReady(false),
    mUsername(username),
    mPwdhash(pwdHash),
    mAuthSent(false)
{
}

void TiiraServiceHelper::resetServer(const QString &username, const QString &pwdHash) {
    mAuthSent = false;
    mUsername = username;
    mPwdhash = pwdHash;
    setServer();
}

void TiiraServiceHelper::init(const QNetworkConfiguration &config) {
    qDebug() << Q_FUNC_INFO;
    if (mReady) {
        return;
    }
    mNetAccessMgr = new QNetworkAccessManager(this);

    mNetAccessMgr->setConfiguration(config);
    connect(mNetAccessMgr, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this,
                SLOT(provideAuthenication(QNetworkReply*,QAuthenticator*)));
    //connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRowRequestFinished(QNetworkReply*)));
    setServer();
    mReady = true;

}

void TiiraServiceHelper::login() {
    qDebug() << Q_FUNC_INFO;
    disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRequestFinished(QNetworkReply*)));
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loginRequestFinished(QNetworkReply*)));
    QMap<QString,QString> map;

    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mSoapServerAddress, QUrl::StrictMode );
    request.setUrl( urli );
    QString xml = envelopBodyStart();
    xml.append("<v2:kukaOlen>");
    xml.append(envelopAuth());
    xml.append("<v2:varmenne>");
    xml.append(TiiraChecksumCalculator::getChecksum(mPwdhash, map));
    xml.append("</v2:varmenne>");
    xml.append("</v2:kukaOlen>");
    xml.append(envelopBodyEnd());
    QByteArray postdata;
    postdata.append(xml);
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                           QVariant( QString("text/xml;charset=utf-8")));
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                          QVariant( qulonglong(postdata.size()) ));
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QVariant( int(QNetworkRequest::AlwaysNetwork) ));

    mNetAccessMgr->post(request, postdata);
}

void TiiraServiceHelper::loginRequestFinished(QNetworkReply *reply) {
    disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loginRequestFinished(QNetworkReply*)));
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRequestFinished(QNetworkReply*)));
    //qDebug() << Q_FUNC_INFO << reply;
    QString data = reply->readAll();
    //qDebug() << Q_FUNC_INFO << data;
    if (data.contains("autentikointivirhe")) {
        //qDebug() << Q_FUNC_INFO << "wrong credientals";
        emit wrongCredientals();
    } else if (data.contains("<ns1:kukaOlenResponse>")) {
        mUsersName = readName(data);
        //qDebug() << Q_FUNC_INFO << "login ok" << mUsersName;
        emit loginOk(mUsersName);
    } else {
        qDebug() << Q_FUNC_INFO << "unknown error";
        emit loginFailUnknown();
    }
}

void TiiraServiceHelper::getAd() {
    qDebug() << Q_FUNC_INFO;
    disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRequestFinished(QNetworkReply*)));
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(adRequestFinished(QNetworkReply*)));
    QMap<QString,QString> map;

    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mServerAddress, QUrl::StrictMode );
    request.setUrl( urli );

    mNetAccessMgr->get(request);
}


void TiiraServiceHelper::adRequestFinished(QNetworkReply *reply) {
    qDebug() << Q_FUNC_INFO << reply;
    disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(adRequestFinished(QNetworkReply*)));
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRequestFinished(QNetworkReply*)));
    QString data = reply->readAll();
    qDebug() << Q_FUNC_INFO << data;
    data = data.replace("{", "");
    data = data.replace("}", "");
    QStringList fields = data.split(",");
    QString iconUrl = "";
    QString url = "";
    bool closed = false;
    foreach(QString field, fields) {
        qDebug() << "field: " << field;
        if (field.startsWith("\"icon\"")) {
            iconUrl = field.section("\":\"", 1, 1).remove("\"");
            qDebug() << "iconUrl: " << iconUrl;
            if (Settings::tiiraServer() != 0) {
                iconUrl.replace("http://", "http://" + Settings::tiiraServerUsername() + ":" + Settings::tiiraServerPassword() + "@");
            }
        }
        else if (field.startsWith("\"url\"")) {
            url = field.section("\":\"", 1, 1).remove("\"");
            qDebug() << "url: " << url;
        }
        else if (field.startsWith("\"suljettu\"")) {
            QString tiiraClosed = field.section("\":\"", 1, 1).remove("\"");
            qDebug() << "suljettu: " << tiiraClosed;
            if (QString::compare(tiiraClosed, "1", Qt::CaseInsensitive) == 0) {
                closed = true;
            }

        }
    }
    if( !closed && !iconUrl.isEmpty() && !url.isEmpty()) {
        emit adLoaded(iconUrl, url);
    }
    if( closed ) {
        emit tiiraClosed();
    }

}


void TiiraServiceHelper::requestFinished(QNetworkReply *reply) {
    //qDebug() << Q_FUNC_INFO << reply;
    //qDebug() << Q_FUNC_INFO << reply->readAll();
}

void TiiraServiceHelper::provideAuthenication(QNetworkReply *reply, QAuthenticator *ator) {
    qDebug() << Q_FUNC_INFO;
    if (mAuthSent) {
        disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loginRequestFinished(QNetworkReply*)));
        emit serverLoginFailed();
        return;
    }
    ator->setUser(mServerUsername);
    ator->setPassword(mServerPassword);
    mAuthSent = true;
}

void TiiraServiceHelper::uploadRecord(const QMap<QString,QString> &map, long recordId) {
    qDebug() << Q_FUNC_INFO;
//    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRequestFinished(QNetworkReply*)));

    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mSoapServerAddress, QUrl::StrictMode );
    request.setUrl( urli );
    QString xml = envelopBodyStart();
    xml.append("<v2:lataaHavainto>");

    xml.append(recordRowNodes(map));
    xml.append(envelopAuth());
    xml.append("<v2:varmenne>");
    xml.append(TiiraChecksumCalculator::getChecksum(mPwdhash, map));
    xml.append("</v2:varmenne>");
    xml.append("</v2:lataaHavainto>");
    xml.append(envelopBodyEnd());
    //qDebug() << "sending xml" << xml;
    QByteArray postdata;
    postdata.append(xml.toUtf8());
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                           QVariant( QString("text/xml;charset=utf-8")));
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                          QVariant( qulonglong(postdata.size()) ));
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QVariant( int(QNetworkRequest::AlwaysNetwork) ));

    mRequests.insert(recordId, request);
    mNetAccessMgr->post(request, postdata);
}

void TiiraServiceHelper::uploadRequestFinished(QNetworkReply *reply) {
    //qDebug() << Q_FUNC_INFO << reply;
    //disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRequestFinished(QNetworkReply*)));
    QString data = reply->readAll();
    qDebug() << Q_FUNC_INFO << data;
    if (data.contains("<ns1:lataaHavaintoResponse>")) {
        QString csvId = readCsvId(data);
        qDebug() << Q_FUNC_INFO << "upload ok" << csvId;

        long id = mRequests.key(reply->request());
        emit uploadOk(id, csvId);
        mRequests.remove(id);
        //
    } else if (data.contains("<ns1:lataaHavaintoRiviResponse>")) {
        QString row = readRowCount(data);
        qDebug() << Q_FUNC_INFO << "upload ok" << row;
        //emit rowUploadOk(mRowRequests.value(reply->request()), rows.toInt());
        long id = mRowRequests.key(reply->request());
        emit rowUploadOk(id, row.toInt());
        //mRowRequests.remove(id);

    } else if (data.contains("Virhe") || data.contains("virhe")) {
        qDebug() << Q_FUNC_INFO << "upload failed";
        QString error = readError(data);
        emit uploadFailed(error);
    }
}

void TiiraServiceHelper::uploadRecordRow(const QMap<QString,QString> &map, long recordId) {
    qDebug() << Q_FUNC_INFO;
//    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRowRequestFinished(QNetworkReply*)));

    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mSoapServerAddress, QUrl::StrictMode );
    request.setUrl( urli );
    QString xml = envelopBodyStart();
    xml.append("<v2:lataaHavaintoRivi>");

    xml.append(recordRowNodes(map));
    xml.append(envelopAuth());

    xml.append("<v2:varmenne>");
    xml.append(TiiraChecksumCalculator::getChecksum(mPwdhash, map));
    xml.append("</v2:varmenne>");
    xml.append("</v2:lataaHavaintoRivi>");
    xml.append(envelopBodyEnd());
    qDebug() << "sending xml" << xml;
    QByteArray postdata;
    postdata.append(xml.toUtf8());
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                           QVariant( QString("text/xml;charset=utf-8")));
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                          QVariant( qulonglong(postdata.size()) ));
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QVariant( int(QNetworkRequest::AlwaysNetwork) ));

    mRowRequests.insert(recordId, request);
    mNetAccessMgr->post(request, postdata);

}

void TiiraServiceHelper::uploadRowRequestFinished(QNetworkReply *reply) {
    //qDebug() << Q_FUNC_INFO << reply;
    //disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadRowRequestFinished(QNetworkReply*)));
    QString data = reply->readAll();
    qDebug() << Q_FUNC_INFO << data;
    if (data.contains("<ns1:lataaHavaintoRiviResponse>")) {
        QString row = readRowCount(data);
        qDebug() << Q_FUNC_INFO << "upload ok" << row;
        //emit rowUploadOk(mRowRequests.value(reply->request()), rows.toInt());
        long id = mRowRequests.key(reply->request());
        emit rowUploadOk(id, row.toInt());
        mRowRequests.remove(id);

    }/* else if (data.contains("CSV-tiedoston rivin tallennus ei onnistunut")) {
        qDebug() << Q_FUNC_INFO << "upload failed";
    }*/
}

QString TiiraServiceHelper::envelopBodyStart() const {
    QString xml;
    xml.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    xml.append("<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:v2=\"http://tiira.fi/ws/v2\">");
    xml.append("<soapenv:Header/>");
    xml.append("<soapenv:Body>");
    return xml;
}

QString TiiraServiceHelper::envelopAuth() const {
    QString xml;
    xml.append("<v2:sovellusavain>");
    xml.append(mTiiraAppKey);
    xml.append("</v2:sovellusavain>");
    xml.append("<v2:tunnus>");
    xml.append(mUsername);
    xml.append("</v2:tunnus>");
    return xml;
}

QString TiiraServiceHelper::envelopBodyEnd() const {
    QString xml;
    xml.append("</soapenv:Body>");
    xml.append("</soapenv:Envelope>");
    return xml;
}

QString TiiraServiceHelper::readName(const QString &data) const {
    QString name = "";
    QString startTag1("<etunimi>");
    QString stopTag1("</etunimi>");
    int firstNameStartPos = data.indexOf(startTag1) + startTag1.length();
    int firstNameStopPos = data.indexOf(stopTag1);
    name += data.mid( firstNameStartPos, firstNameStopPos-firstNameStartPos);
    QString startTag2("<sukunimi>");
    QString stopTag2("</sukunimi>");
    int lastNameStartPos = data.indexOf(startTag2) + startTag2.length();
    int lastNameStopPos = data.indexOf(stopTag2);
    name += " " + data.mid( lastNameStartPos, lastNameStopPos-lastNameStartPos);
    QString utf8name = QString::fromUtf8(name.trimmed().toAscii());
    return utf8name;
}

QString TiiraServiceHelper::recordRowNodes(const QMap<QString,QString> &map) const {
    QString data;
    QMap<QString, QString>::const_iterator i;
    for (i = map.begin(); i != map.end(); ++i) {
        data += "<v2:" + i.key() + ">" + i.value() + "</v2:" + i.key() + ">\n";
    }
    return data;
}

QString TiiraServiceHelper::readCsvId(const QString &data) const {
    QString name = "";
    QString startTag1("<ns1:lataaHavaintoResponse>");
    QString stopTag1("</ns1:lataaHavaintoResponse>");
    int firstNameStartPos = data.indexOf(startTag1) + startTag1.length();
    int firstNameStopPos = data.indexOf(stopTag1);
    name = data.mid( firstNameStartPos, firstNameStopPos-firstNameStartPos);
    return name.trimmed();
}

QString TiiraServiceHelper::readRowCount(const QString &data) const {
    QString name = "";
    QString startTag1("<ns1:lataaHavaintoRiviResponse>");
    QString stopTag1("</ns1:lataaHavaintoRiviResponse>");
    int firstNameStartPos = data.indexOf(startTag1) + startTag1.length();
    int firstNameStopPos = data.indexOf(stopTag1);
    name = data.mid( firstNameStartPos, firstNameStopPos-firstNameStartPos);
    return name.trimmed();
}

QString TiiraServiceHelper::readError(const QString &data) const {
    QString error = "";
    QString startTag1("<ns1:reason>");
    QString stopTag1("</ns1:reason>");
    int errorStartPos = data.indexOf(startTag1) + startTag1.length();
    int errorNameStopPos = data.indexOf(stopTag1);
    error = data.mid( errorStartPos, errorNameStopPos-errorStartPos);
    return error.trimmed();
}

void TiiraServiceHelper::setServer() {
    mServerUsername = Settings::tiiraServerUsername();
    mServerPassword = Settings::tiiraServerPassword();
    bool debugMode = Settings::tiiraDebugMode();
    int server = Settings::tiiraServer();
    switch (server) {
        case 0:
        if (debugMode) {
            mSoapServerAddress = "http://www.tiira.fi/ws/ws/v2/debugsoapserver.php";
        } else {
            mSoapServerAddress = "http://www.tiira.fi/ws/ws/v2/soapserver.php";
        }
        mServerAddress = "http://www.tiira.fi/mobile/mainos.php";
#ifdef HARMATTAN
        mTiiraAppKey = "harmattanAppKey";
#else
        mTiiraAppKey = "symbianAppKey";
#endif
        break;
        case 1:
        if (debugMode) {
            mSoapServerAddress = "http://testi.tiira.fi/ws/ws/v2/debugsoapserver.php";
        } else {
            mSoapServerAddress = "http://testi.tiira.fi/ws/ws/v2/soapserver.php";
        }
        mServerAddress = "http://testi.tiira.fi/mobile/mainos.php";
#ifdef HARMATTAN
        mTiiraAppKey = "harmattanAppKeyTest";
#else
        mTiiraAppKey = "symbianAppKeyTest"; 
#endif
        break;
        case 2:
        if (debugMode) {
            mSoapServerAddress = "http://dev.tiira.fi/ws/ws/v2/debugsoapserver.php";
        } else {
            mSoapServerAddress = "http://dev.tiira.fi/ws/ws/v2/soapserver.php";
        }
        mServerAddress = "http://dev.tiira.fi/mobile/mainos.php";
#ifdef HARMATTAN
        mTiiraAppKey = "harmattanAppKeyDev";
#else
        mTiiraAppKey = "symbianAppKeyDev";
#endif
        break;
    }

}

void TiiraServiceHelper::removeId(long id) {
    mRowRequests.remove(id);
}
