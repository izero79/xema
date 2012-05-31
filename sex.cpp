#include "sex.h"

Sex::Sex() {
}

Sex::Sex(const QString &finname, const QString &swename, const QString &engname) :
    mFinName(finname),
    mSweName(swename),
    mEngName(engname)
{

}

void Sex::setFinName(const QString &name)
{
    mFinName = name;
}

void Sex::setSweName(const QString &name)
{
    mSweName = name;
}

void Sex::setEngName(const QString &name)
{
    mEngName = name;
}

QString Sex::finName() const
{
    return mFinName;
}

QString Sex::sweName() const
{
    if (mSweName.isEmpty()) {
        return mFinName;
    }
    return mSweName;
}

QString Sex::engName() const
{
    if (mEngName.isEmpty()) {
        return mFinName;
    }
    return mEngName;
}
