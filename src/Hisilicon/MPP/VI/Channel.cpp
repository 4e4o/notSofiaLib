#include "Channel.h"
#include "Device.h"
#include "Hisilicon/MPP/VI/Source/ChannelInfo.h"
#include "Hisilicon/MPP/Utils.h"

#include <stdexcept>

#include <hi_comm_vi.h>
#include <mpi_vi.h>

namespace hisilicon {
namespace mpp {
namespace vi {

static VI_CHN_ATTR_S* createStandardAttr() {
    VI_CHN_ATTR_S* attr = new VI_CHN_ATTR_S{};

    attr->bMirror = HI_FALSE;
    attr->bFlip = HI_FALSE;
    attr->enCapSel = VI_CAPSEL_BOTH;
    attr->bChromaResample = HI_FALSE;
    attr->s32SrcFrameRate = -1;
    attr->s32FrameRate = -1;

    return attr;
}

Channel::Channel(MPP *p, Device* d, int id)
    : MPPChild(p), IdHolder(id), Holder<Device*>(d),
      m_enabled(false),
      m_info(NULL),
      m_attr(createStandardAttr()) {
}

Channel::~Channel() {
    if (m_enabled)
        HI_MPI_VI_DisableChn(id());
}

ChannelInfo* Channel::info() const {
    return m_info;
}

TSize Channel::createDestSize() const {
    return m_info->imgSize();
}

VI_CHN_BIND_ATTR_S Channel::createBindAttrs() const {
    VI_CHN_BIND_ATTR_S attrs;
    attrs.ViDev = device()->id();
    attrs.ViWay = id();
    return attrs;
}

bool Channel::configure() {
    if (m_enabled)
        throw std::runtime_error("vi::Channel already configured");

    if (m_info == NULL)
        throw std::runtime_error("[vi::Channel] Vi info is not set");

    if (m_attr.get() == NULL)
        throw std::runtime_error("[vi::Channel] Vi attr is not set");

    {
        // TODO
        // HiMPP Media Processing Software Development Reference.pdf
        // page 121

        VI_CHN_BIND_ATTR_S attrs = createBindAttrs();

        if (HI_MPI_VI_ChnUnBind(id()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_ChnUnBind failed");

        if (HI_MPI_VI_ChnBind(id(), &attrs) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_ChnBind failed");
    }

    {
        m_attr->stCapRect = Utils::toMppRect(m_info->capSize());
        m_attr->stDestSize = Utils::toMppSize(createDestSize());
        m_attr->enPixFormat = m_info->pixelFormat();

        if (HI_MPI_VI_SetChnScanMode(id(), m_info->scanMode()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_SetChnScanMode failed");

        if (HI_MPI_VI_SetChnAttr(id(), m_attr.get()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_SetChnAttr failed");

        if (HI_MPI_VI_EnableChn(id()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_EnableChn failed");
    }

    m_enabled = true;

    return true;
}

void Channel::setAttr(VI_CHN_ATTR_S* attr) {
    m_attr.reset(attr);
}

void Channel::setInfo(ChannelInfo *info) {
    m_info = info;
}

SIZE_S Channel::destSize() const {
    return m_attr->stDestSize;
}

SIZE_S Channel::imgSize() const {
    return Utils::toMppSize(m_info->imgSize());
}

bool Channel::pal() const {
    return m_info->pal();
}

Device *Channel::device() const {
    return Holder<Device*>::value();
}

}
}
}
