#include "bird.h"

Bird::Bird() :
    mId(0)
{
}

Bird::Bird(int id,  const QString &finGroup, const QString &sweGroup, const QString &latinGroup, const QString &finName, const QString &sweName, const QString &latin, const QString &abbr, const QString &category)  :
    mId(id),
    mFinGroup(finGroup),
    mSweGroup(sweGroup),
    mEngGroup(sweGroup),
    mLatinGroup(latinGroup),
    mFinName(finName),
    mSweName(sweName),
    mEngName(sweName),
    mLatinName(latin),
    mAbbreviation(abbr),
    mCategory(category)
{

}

void Bird::setId(int id)
{
    mId = id;
}

void Bird::setFinGroup(const QString &group)
{
    mFinGroup = group;
}

void Bird::setSweGroup(const QString &group)
{
    mSweGroup = group;
}

void Bird::setEngGroup(const QString &group)
{
    mEngGroup = group;
}

void Bird::setLatinGroup(const QString &group)
{
    mLatinGroup = group;
}


void Bird::setFinName(const QString &name)
{
    mFinName = name;
}

void Bird::setSweName(const QString &name)
{
    mSweName = name;
}

void Bird::setEngName(const QString &name)
{
    mEngName = name;
}

void Bird::setAbbreviation(const QString &abbr)
{
    mAbbreviation = abbr;
}

void Bird::setLatinName(const QString &name)
{
    mLatinName = name;
}

void Bird::setCategory(const QString &category)
{
    mCategory = category;
}

int Bird::id() const
{
    return mId;
}

QString Bird::finGroup() const
{
    return mFinGroup;
}

QString Bird::sweGroup(bool allowEmpty) const
{
    if (mSweGroup.isEmpty() == false) {
        return mSweGroup;
    }
    else if (allowEmpty) {
        return mSweGroup;
    }
    return mFinGroup;
}

QString Bird::engGroup(bool allowEmpty) const
{
    if (mEngGroup.isEmpty() == false) {
        return mEngGroup;
    }
    else if (allowEmpty) {
        return mEngGroup;
    }
    return mFinGroup;
}

QString Bird::latinGroup() const
{
    return mLatinGroup;
}


QString Bird::finName() const
{
    return mFinName;
}

QString Bird::sweName(bool allowEmpty) const
{
    if (mSweName.isEmpty() == false) {
        return mSweName;
    }
    else if (allowEmpty) {
        return mSweName;
    }
    return mFinName;
}

QString Bird::engName(bool allowEmpty) const
{
    if (mEngName.isEmpty() == false) {
        return mEngName;
    }
    else if (allowEmpty) {
        return mEngName;
    }
    return mFinName;
}

QString Bird::abbreviation() const
{
    return mAbbreviation;
}

QString Bird::latinName() const
{
    return mLatinName;
}

QString Bird::category() const
{
    return mCategory;
}
