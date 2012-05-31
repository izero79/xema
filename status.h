#ifndef STATUS_H
#define STATUS_H

#include <QString>

class Status
{
public:
    Status();
    Status(const QString &name, const QString &abbrev, const QString &swename = QString(), const QString &engname = QString());

    void setName(const QString &name);
    void setSweName(const QString &name);
    void setEngName(const QString &name);
    void setAbbrev(const QString &abbr);
    void setSelected(bool selected);

    QString name() const;
    QString sweName(bool allowEmpty = false) const;
    QString engName(bool allowEmpty = false) const;
    QString abbreviation() const;
    bool selected() const;

private:
    QString mName;
    QString mSweName;
    QString mEngName;
    QString mAbbrev;
    bool mSelected;
};

#endif // STATUS_H
