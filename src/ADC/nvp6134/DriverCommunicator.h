#ifndef NVP_6134_DRIVER_COMMUNICATOR_H
#define NVP_6134_DRIVER_COMMUNICATOR_H

#include <memory>
#include <vector>
#include <thread>

#include <nvp6134_ex/common.h>
#include <nvp6134_ex/video.h>

#include "Misc/Configurator/Configurable.h"

namespace nvp6134 {

class Chip;
class ViChannel;
class VoChannel;
class MotionLoop;

class DriverCommunicator : public Configurable {
  public:
    typedef std::pair<int, int> TChipChannel;
    typedef std::vector<TChipChannel> TChipsChannels;

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
    bool setViMotion(const ViChannel *);
    TChipsChannels getMotion() const;

  private:
    bool configureImpl() override final;

    void resetInputVideoFmt();
    bool getVideoFmt();
    void detectVideoFmt();
    bool openDriver();
    bool closeDriver();
    bool setViMotionSensitivity(const ViChannel *);
    bool setViMotionArea(const ViChannel *);
    bool setViMotionEnabled(const ViChannel *);
    bool setViMotionVisualize(const ViChannel *);
    void startMotionThread();
    void stopMotionThread();

    int m_chipCount;
    int m_driverFd;
    std::unique_ptr<nvp6134_input_videofmt> m_inputFormat;
    std::vector<std::unique_ptr<ViChannelFormat>> m_channelInputFormats;
    std::unique_ptr<MotionLoop> m_motionLoop;
    std::unique_ptr<std::thread> m_motionThread;
};

}

#endif // NVP_6134_DRIVER_COMMUNICATOR_H
