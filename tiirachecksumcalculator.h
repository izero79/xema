#ifndef TIIRACHECKSUMCALCULATOR_H
#define TIIRACHECKSUMCALCULATOR_H

#include <QObject>
#include <QMap>

class TiiraChecksumCalculator : public QObject
{
    Q_OBJECT
public:
    explicit TiiraChecksumCalculator(QObject *parent = 0);
    static QString getMd5(const QString &text);
    static QString getChecksum(const QString &pwdHash, const QMap<QString,QString> map);
    
signals:
    
public slots:
    
};

#endif // TIIRACHECKSUMCALCULATOR_H
