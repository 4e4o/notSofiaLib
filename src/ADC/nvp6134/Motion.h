#ifndef NVP_6134_MOTION_H
#define NVP_6134_MOTION_H

#include "Misc/Holder.h"

#include <array>
#include <functional>
#include <stdint.h>

namespace nvp6134 {

class ViChannel;

class Motion : public Holder<ViChannel *> {
  public:
    typedef std::function<void(Motion *)> TMotionEvent;

    // in blocks
    static constexpr int AREA_WIDTH = 16;
    static constexpr int AREA_HEIGHT = 12;
    static constexpr int AREA_SIZE = AREA_WIDTH * AREA_HEIGHT;
    static_assert (AREA_SIZE == 192, "AREA_SIZE must be == 192");
    typedef std::array<bool, AREA_WIDTH> TAreaRow;
    typedef std::array<TAreaRow, AREA_HEIGHT> TArea;

    Motion(ViChannel *);
    ~Motion();

    // значения взяты из Софии
    enum class TemporalSensitivity : uint8_t {
        HIGHEST = 0x30,
        VERY_HIGH = 0x3a,
        HIGH = 0x59,
        MEDIUM = 0x78,
        LOW = 0x88,
        LOWEST = 0x98
    };

    enum class PixelSensitivity : uint8_t {
        PS_BYPASS = 0,
        PS_1S2 = 0x01,
        PS_1S4 = 0x02,
        PS_1S8 = 0x03
    };

    TMotionEvent event() const;
    void setEvent(const TMotionEvent &event);

    bool enabled() const;
    void setEnabled(bool enabled);

    bool visualize() const;
    void setVisualize(bool visualize);

    uint8_t temporalSensitivity() const;
    void setTemporalSensitivity(const TemporalSensitivity &);
    void setTemporalSensitivity(const uint8_t &);

    void setAreaBlock(uint8_t row, uint8_t column, bool enabled);
    void setAreaRow(uint8_t row, bool enabled);
    void setAreaColumn(uint8_t column, bool enabled);
    void setAreaAll(bool enabled);
    const TArea &area() const;

    ViChannel *channel() const;

    PixelSensitivity pixelSensitivity() const;
    void setPixelSensitivity(const PixelSensitivity &);

    uint8_t brightessSensitivity() const;
    void setBrightessSensitivity(const uint8_t &);

  private:
    bool m_enabled;
    uint8_t m_temporalSensitivity;
    PixelSensitivity m_pixelSensitivity;
    uint8_t m_brightessSensitivity;
    TArea m_area;
    bool m_visualize;
    TMotionEvent m_event;
};

}

#endif // NVP_6134_MOTION_H
