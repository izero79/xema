#ifndef ATLASINDEX_H
#define ATLASINDEX_H

#include <QString>

class AtlasIndex
{
public:
    AtlasIndex();
    AtlasIndex( int value );

    void setIndex( int value );

    int index() const;

private:
    int mIndex;
};

#endif // ATLASINDEX_H
