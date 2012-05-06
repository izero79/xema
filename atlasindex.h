#ifndef ATLASINDEX_H
#define ATLASINDEX_H

#include <QString>

class AtlasIndex
{
public:
    AtlasIndex();
    AtlasIndex(const QString &value, const QString &finname, const QString &swename, const QString &engname);

    void setValue(const QString &value);
    void setFinName(const QString &name);
    void setSweName(const QString &name);
    void setEngName(const QString &name);

    QString value() const;
    QString finName() const;
    QString sweName() const;
    QString engName() const;

private:
    QString mValue;
    QString mFinName;
    QString mSweName;
    QString mEngName;
};

#endif // ATLASINDEX_H
