#ifndef MODELDATAWRITER_H
#define MODELDATAWRITER_H

#include <QObject>

class ModelDataWriter : public QObject
{
    Q_OBJECT
public:
    explicit ModelDataWriter(QObject *parent = 0);

    static void writeNewObservation( const QString &data );

signals:

public slots:

};

#endif // MODELDATAWRITER_H
