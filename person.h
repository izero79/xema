#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person
{
public:
    Person();

    Person( const QString &firstName, const QString &surName, bool registered, bool defaultName = false );

    void setFirstName( const QString &name );
    void setSurName( const QString &name );
    void setRegistered( bool registered );
    void setDefault( bool defaultName );
    void setSelected( bool selected );

    QString firstName() const;
    QString surName() const;
    QString name() const;
    bool registered() const;
    bool defaultName() const;
    bool selected() const;

private:
    QString mFirstName;
    QString mSurName;
    bool mRegistered;
    bool mDefault;
    bool mSelected;
};

#endif // PERSON_H
