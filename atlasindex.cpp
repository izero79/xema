#include "atlasIndex.h"
#include "settings.h"

AtlasIndex::AtlasIndex() {
}

AtlasIndex::AtlasIndex(const QString &value, const QString &finname, const QString &swename, const QString &engname) :
    mValue(value),
    mFinName(finname),
    mSweName(swename),
    mEngName(engname)
{

}

void AtlasIndex::setFinName(const QString &name)
{
    mFinName = name;
}

void AtlasIndex::setSweName(const QString &name)
{
    mSweName = name;
}

void AtlasIndex::setEngName(const QString &name)
{
    mEngName = name;
}

void AtlasIndex::setValue(const QString &value)
{
    mValue = value;
}

QString AtlasIndex::localizedName() const
{
    QString lang = Settings::lang();
    if (lang == "sv") {
        return sweName();
    } else if (lang == "en") {
        return engName();
    }
    else
        return finName();

}

QString AtlasIndex::finName() const
{
    return mFinName;
}

QString AtlasIndex::sweName() const
{
    if (mSweName.isEmpty()) {
        return mFinName;
    }
    return mSweName;
}

QString AtlasIndex::engName() const
{
    if (mEngName.isEmpty()) {
        return mFinName;
    }
    return mEngName;
}

QString AtlasIndex::value() const
{
    return mValue;
}
