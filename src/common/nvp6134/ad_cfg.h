#ifndef NVP_SAMPLE_DEFS_H
#define NVP_SAMPLE_DEFS_H

#include "sample_comm.h"

void adInit();

struct ChannelInfo {
    int hasSignal;
    VIDEO_NORM_E norm;
    PIC_SIZE_E sizeType;
    VI_SCAN_MODE_E scanMode;

    SIZE_S stPicSize;

    RECT_S stCapRect;
    SIZE_S stDestSize;
};

const ChannelInfo* getChannelInfo(int ch);

#endif // NVP_SAMPLE_DEFS_H
