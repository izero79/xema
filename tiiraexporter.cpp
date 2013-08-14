#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>


#include "tiiraexporter.h"
#include "tiirachecksumcalculator.h"

TiiraExporter::TiiraExporter(const QString &username, const QString &pwdHash, QObject *parent) :
    QObject(parent),
    mNetAccessMgr(0),
    mReady(false),
    mUsername(username),
    mPwdhash(pwdHash)
{
}

void TiiraExporter::init(const QNetworkConfiguration &config) {
    qDebug() << Q_FUNC_INFO;
    if (mReady) {
        return;
    }
    mNetAccessMgr = new QNetworkAccessManager(this);

    mNetAccessMgr->setConfiguration(config);
//    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
    connect(mNetAccessMgr, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this,
                SLOT(provideAuthenication(QNetworkReply*,QAuthenticator*)));
    mServerUsername = "testaaja";
    mServerPassword = "testeri-2011";
    mServerAddress.append("http://testi.tiira.fi/ws/ws/soapserver.php");
    mReady = true;

}

void TiiraExporter::testRequest() {
    qDebug() << Q_FUNC_INFO;
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
    QMap<QString,QString> map;
    TiiraChecksumCalculator::getChecksum(mPwdhash, map);
    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mServerAddress, QUrl::StrictMode );
    request.setUrl( urli );
    QString xml;
    xml.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    xml.append("<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ws=\"http://tiira.fi/ws\">");
    xml.append("<soapenv:Header/>");
    xml.append("<soapenv:Body>");
    xml.append("<ws:kukaOlen>");
    xml.append("<ws:tunnus>iZero</ws:tunnus>");
    xml.append("<ws:varmenne>f21c0fb0f6cb672fb08f66a9338990be</ws:varmenne>");
    xml.append("</ws:kukaOlen>");
    xml.append("</soapenv:Body>");
    xml.append("</soapenv:Envelope>");
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

void TiiraExporter::login() {
    qDebug() << Q_FUNC_INFO;
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loginRequestFinished(QNetworkReply*)));
    QMap<QString,QString> map;

    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mServerAddress, QUrl::StrictMode );
    request.setUrl( urli );
    QString xml = envelopBodyStart();
    xml.append("<ws:kukaOlen>");
    xml.append("<ws:tunnus>");
    xml.append(mUsername);
    xml.append("</ws:tunnus>");
    xml.append("<ws:varmenne>");
    xml.append(TiiraChecksumCalculator::getChecksum(mPwdhash, map));
    xml.append("</ws:varmenne>");
    xml.append("</ws:kukaOlen>");
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

void TiiraExporter::loginRequestFinished(QNetworkReply *reply) {
    disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loginRequestFinished(QNetworkReply*)));
    qDebug() << Q_FUNC_INFO << reply;
    QString data = reply->readAll();
    qDebug() << Q_FUNC_INFO << data;
    if (data.contains("autentikointivirhe")) {
        qDebug() << Q_FUNC_INFO << "wrong credientals";
        emit wrongCredientals();
    } else if (data.contains("<ns1:kukaOlenResponse>")) {
        mUsersName = readName(data);
        qDebug() << Q_FUNC_INFO << "login ok" << mUsersName;
        testUpload();
    }
}

void TiiraExporter::testUpload() {
    qDebug() << Q_FUNC_INFO;
    connect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(testUploadRequestFinished(QNetworkReply*)));
    QMap<QString,QString> map;

    QNetworkRequest request;
    QUrl urli;
    urli.setEncodedUrl( mServerAddress, QUrl::StrictMode );
    request.setUrl( urli );
    QString xml = envelopBodyStart();
    xml.append("<ws:lataaHavainto>");
    xml.append("<ws:tunnus>");
    xml.append(mUsername);
    xml.append("</ws:tunnus>");
    xml.append("<ws:varmenne>");
    xml.append(TiiraChecksumCalculator::getChecksum(mPwdhash, map));
    xml.append("</ws:varmenne>");
    xml.append("</ws:lataaHavainto>");
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

void TiiraExporter::testUploadRequestFinished(QNetworkReply *reply) {
    qDebug() << Q_FUNC_INFO << reply;
    disconnect(mNetAccessMgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(testUploadRequestFinished(QNetworkReply*)));
    QString data = reply->readAll();
    qDebug() << Q_FUNC_INFO << data;
    if (data.contains("ei ole oikeutta ladata havaintoja")) {
        qDebug() << Q_FUNC_INFO << "no csv rights";
        emit noUploadRights();
    } else if (data.contains("CSV-tiedoston rivin tallennus ei onnistunut")) {
        qDebug() << Q_FUNC_INFO << "login ok";
        emit loginOk(mUsersName);
    }
}


void TiiraExporter::requestFinished(QNetworkReply *reply) {
    qDebug() << Q_FUNC_INFO << reply;
    qDebug() << Q_FUNC_INFO << reply->readAll();
}

void TiiraExporter::provideAuthenication(QNetworkReply *reply, QAuthenticator *ator) {
    ator->setUser(mServerUsername);
    ator->setPassword(mServerPassword);
}

void TiiraExporter::uploadRecord() {

}

void TiiraExporter::uploadRecordRow() {

}

QString TiiraExporter::envelopBodyStart() const {
    QString xml;
    xml.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    xml.append("<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ws=\"http://tiira.fi/ws\">");
    xml.append("<soapenv:Header/>");
    xml.append("<soapenv:Body>");
    return xml;
}

QString TiiraExporter::envelopBodyEnd() const {
    QString xml;
    xml.append("</soapenv:Body>");
    xml.append("</soapenv:Envelope>");
    return xml;
}

QString TiiraExporter::readName(const QString &data) const {
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
    return name.trimmed();
}
