#include <QCryptographicHash>
#include <QDebug>

#include "tiirachecksumcalculator.h"

TiiraChecksumCalculator::TiiraChecksumCalculator(QObject *parent) :
    QObject(parent)
{
}

QString TiiraChecksumCalculator::getMd5(const QString &text) {
    QString found = QCryptographicHash::hash( text.toLatin1(), QCryptographicHash::Md5 ).toHex();
    qDebug() << text << found;
    return found;
}

QString TiiraChecksumCalculator::getChecksum(const QString &pwdHash, const QMap<QString,QString> map) {
    QString arguments = "";
    QMap<QString, QString>::const_iterator i;
    for (i = map.begin(); i != map.end(); ++i) {
        arguments += i.key() + "=" + i.value();
    }
    arguments += pwdHash;
    QString checksum = QCryptographicHash::hash( arguments.toUtf8(), QCryptographicHash::Md5 ).toHex();
    qDebug() << arguments << checksum;
    return checksum;
}
