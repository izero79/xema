#ifndef STATUS_H
#define STATUS_H

#include <QString>

class Status
{
public:
    Status();
    Status(const QString &name, const QString &abbrev);

    void setName(const QString &name);
    void setAbbrev(const QString &abbr);
    void setSelected(bool selected);

    QString name() const;
    QString abbreviation() const;
    bool selected() const;

private:
    QString mName;
    QString mAbbrev;
    bool mSelected;
};

#endif // STATUS_H
