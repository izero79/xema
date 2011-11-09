#ifndef LOCATION_H
#define LOCATION_H

#include <QString>

class Location
{
public:
    Location();

    Location( const QString &town, const QString &place );

    void setTown( const QString &name );
    void setPlace( const QString &name );
    void setYKJCoordinate( const QString &ykj );
    void setWGSCoordinate( const QString &wgs );

    QString town() const;
    QString place() const;
    QString ykjCoordinate() const;
    QString wgsCoordinate() const;
    QString coordinate() const;

private:
    QString mTown;
    QString mPlace;
    QString mYKJCoordinate;
    QString mWGSCoordinate;
};

#endif // LOCATION_H
