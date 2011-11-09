#ifndef BIRD_H
#define BIRD_H

#include <QString>

class Bird
{
public:
    Bird();

    Bird( int id, const QString &finName, const QString &sweName, const QString &abbr, const QString &latin, const QString &category );

    void setId( int id );
    void setFinName( const QString &name );
    void setSweName( const QString &name );
    void setAbbreviation( const QString &abbr );
    void setLatinName( const QString &name );
    void setCategory( const QString &category );

    int id() const;
    QString finName() const;
    QString sweName() const;
    QString abbreviation() const;
    QString latinName() const;
    QString category() const;

private:
    int mId;
    QString mFinName;
    QString mSweName;
    QString mAbbreviation;
    QString mLatinName;
    QString mCategory;
};

#endif // BIRD_H
