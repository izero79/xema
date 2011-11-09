#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person
{
public:
    Person();

    Person( const QString &firstName, const QString &surName, bool registered );

    void setFirstName( const QString &name );
    void setSurName( const QString &name );
    void setRegistered( bool registered );

    QString firstName() const;
    QString surName() const;
    QString name() const;
    bool registered() const;

private:
    QString mFirstName;
    QString mSurName;
    bool mRegistered;

};

#endif // PERSON_H
