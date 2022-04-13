#ifndef OPENGLES_ASSET_H
#define OPENGLES_ASSET_H

#include "global.h"

class Asset {

private:
    static AAssetManager *manager;

public:
    static void setManager(AAssetManager *aAssetManager);
    static AAssetManager *getManager();
    static const char *load(const char *filename);
};

#endif // OPENGLES_ASSET_H
