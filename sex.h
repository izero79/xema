#ifndef SEX_H
#define SEX_H

#include <QString>

class Sex
{
public:
    Sex();
    Sex(const QString &finname, const QString &swename, const QString &engname);

    void setFinName(const QString &name);
    void setSweName(const QString &name);
    void setEngName(const QString &name);

    QString finName() const;
    QString sweName() const;
    QString engName() const;

private:
    QString mFinName;
    QString mSweName;
    QString mEngName;
};
#endif // SEX_H
