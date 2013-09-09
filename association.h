#ifndef ASSOCIATION_H
#define ASSOCIATION_H


#include <QString>

class Association
{
public:
    Association();
    Association(const QString &abbrev, const QString &name);

    void setAbbrev(const QString &abbrev);
    void setName(const QString &name);
    void setSelected(bool selected);

    QString abbrev() const;
    QString name() const;
    bool selected() const;

private:
    QString mAbbrev;
    QString mName;
    bool mSelected;
};
#endif // ASSOCIATION_H
