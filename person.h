#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person
{
public:
    Person();

    Person(const QString &firstName, const QString &surName, bool registered, bool defaultName, bool saver);

    void setFirstName(const QString &name);
    void setSurName(const QString &name);
    void setRegistered(bool registered);
    void setDefault(bool defaultName);
    void setSelected(bool selected);
    void setSaver(bool saver);

    QString firstName() const;
    QString surName() const;
    QString name() const;
    bool registered() const;
    bool defaultName() const;
    bool selected() const;
    bool saver() const;

private:
    QString mFirstName;
    QString mSurName;
    bool mRegistered;
    bool mDefault;
    bool mSelected;
    bool mSaver;
};

#endif // PERSON_H
