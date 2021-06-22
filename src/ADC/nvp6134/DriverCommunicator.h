#ifndef NVP_6134_DRIVER_COMMUNICATOR_H
#define NVP_6134_DRIVER_COMMUNICATOR_H

#include <memory>
#include <vector>

#include <nvp6134_ex/common.h>
#include <nvp6134_ex/video.h>

namespace nvp6134 {

class ViChannel;
class VoChannel;

class DriverCommunicator {
  public:
    struct ViChannelFormat {
        unsigned int inputvideofmt;
        unsigned int getvideofmt;
        unsigned int geteqstage;
        unsigned int getacpdata[8];
    };

    DriverCommunicator(int chipCount);
    ~DriverCommunicator();

    ViChannelFormat *getVideoFmt(const ViChannel *);
    bool setViChannelMode(const ViChannel *, bool pal, NVP6134_VI_MODE chmode);
    bool setVoChannelMode(const VoChannel *, unsigned char chid,
                          NVP6134_OUTMODE_SEL mode);

  private:
    void resetInputVideoFmt();
    bool getVideoFmt();
    void detectVideoFmt();
    bool openDriver();
    bool closeDriver();

    int m_chipCount;
    int m_driverFd;
    std::unique_ptr<nvp6134_input_videofmt> m_inputFormat;
    std::vector<std::unique_ptr<ViChannelFormat>> m_channelInputFormats;
};

}

#endif // NVP_6134_DRIVER_COMMUNICATOR_H
