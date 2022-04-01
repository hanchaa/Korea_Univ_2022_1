#include "asset.h"

AAssetManager *Asset::manager = nullptr;

void Asset::setManager(AAssetManager *aAssetManager) {
    LOG_PRINT_DEBUG("Set asset manager");

    // set manager
    manager = aAssetManager;
}

AAssetManager *Asset::getManager() {
//    LOG_PRINT_DEBUG("Get asset manager");

    // check manager
    if (!manager) {
        LOG_PRINT_ERROR("Fail to get manager");
        return nullptr;
    }

    // return manager
    return manager;
}

const char *Asset::load(const char *filename) {
    LOG_PRINT_DEBUG("Load asset: %s", filename);

    // open asset
    AAsset* aAsset = AAssetManager_open(getManager(), filename, AASSET_MODE_UNKNOWN);

    // check asset
    if (!aAsset) {
        LOG_PRINT_ERROR("Fail to open asset: %s", filename);
        return nullptr;
    }

    // get length of asset
    size_t aAssetLength = (size_t) AAsset_getLength(aAsset);

    // create buffer for null-terminated string
    char* buffer = new char[aAssetLength];
    buffer[aAssetLength] = '\0';

    // read asset
    AAsset_read(aAsset, buffer, aAssetLength);

    // close asset
    AAsset_close(aAsset);

    // return buffer
    return buffer;
}
