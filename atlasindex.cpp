#include "atlasindex.h"

AtlasIndex::AtlasIndex() :
    mIndex(0)
{
}

AtlasIndex::AtlasIndex(int value) :
    mIndex(value)
{
}

void AtlasIndex::setIndex(int value)
{
    mIndex = value;
}

int AtlasIndex::index() const
{
    return mIndex;
}
