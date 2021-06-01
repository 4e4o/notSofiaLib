#ifndef NVP_6134_VO_CHANNEL_H
#define NVP_6134_VO_CHANNEL_H

#include <nvp6134_ex_170306/video.h>

#include "ChipChild.h"
#include "Utils/IdHolder.h"

namespace nvp6134 {

class VoChannel : public ChipChild, public IdHolder {
public:
    VoChannel(Chip*, int id);
    ~VoChannel();

    bool setMode(unsigned char chid, NVP6134_OUTMODE_SEL mode);
};

}

#endif // NVP_6134_VO_CHANNEL_H
