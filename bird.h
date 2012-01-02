#ifndef BIRD_H
#define BIRD_H

#include <QString>

class Bird
{
public:
    Bird();

    Bird( int id,  const QString &finGroup, const QString &sweGroup, const QString &latinGroup,
          const QString &finName, const QString &sweName, const QString &latin, const QString &abbr,
          const QString &category );

    void setId( int id );
    void setFinGroup( const QString &group );
    void setSweGroup( const QString &group );
    void setLatinGroup( const QString &group );
    void setFinName( const QString &name );
    void setSweName( const QString &name );
    void setLatinName( const QString &name );
    void setAbbreviation( const QString &abbr );
    void setCategory( const QString &category );

    int id() const;
    QString finGroup() const;
    QString sweGroup() const;
    QString latinGroup() const;
    QString finName() const;
    QString sweName() const;
    QString latinName() const;
    QString abbreviation() const;
    QString category() const;

private:
    int mId;
    QString mFinGroup;
    QString mSweGroup;
    QString mLatinGroup;
    QString mFinName;
    QString mSweName;
    QString mLatinName;
    QString mAbbreviation;
    QString mCategory;
};

#endif // BIRD_H
