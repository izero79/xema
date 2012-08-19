#ifndef DRESS_H
#define DRESS_H

#include <QString>

class Dress
{
public:
    Dress();
    Dress(const QString &value, const QString &finname, const QString &swename, const QString &engname);

    void setValue(const QString &value);
    void setFinName(const QString &name);
    void setSweName(const QString &name);
    void setEngName(const QString &name);

    QString value() const;
    QString localizedName() const;
    QString finName() const;
    QString sweName() const;
    QString engName() const;

private:
    QString mValue;
    QString mFinName;
    QString mSweName;
    QString mEngName;
};
#endif // DRESS_H
