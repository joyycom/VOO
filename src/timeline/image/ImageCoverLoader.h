#ifndef IMAGECOVERLOADER_H
#define IMAGECOVERLOADER_H

#include "SkyImageResponse.h"

class ImageCoverLoader {
public:
    ImageCoverLoader();

    SkyImageResponse *loadImage(QString path, QSize &size);
};

#endif // IMAGECOVERLOADER_H
