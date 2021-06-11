#ifndef NVP_6134_VO_CHANNEL_H
#define NVP_6134_VO_CHANNEL_H

#include <nvp6134_ex_170306/video.h>

#include "ChipChild.h"
#include "Misc/IdHolder.h"

namespace nvp6134 {

class VoChannel : public ChipChild, public IdHolder {
  public:
    VoChannel(Chip *, int id);
    ~VoChannel();

    bool is1MuxMode() const;
    bool is2MuxMode() const;
    bool is4MuxMode() const;
    bool isMixMode() const;

    bool setMode(unsigned char chid, NVP6134_OUTMODE_SEL mode);

  private:
    NVP6134_OUTMODE_SEL m_mode;
};

}

#endif // NVP_6134_VO_CHANNEL_H
