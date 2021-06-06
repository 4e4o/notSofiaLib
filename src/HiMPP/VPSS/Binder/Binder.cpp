#include "Binder.h"
#include "BindItem.h"

#include <mpi_sys.h>

#include <stdexcept>
#include <iostream>

namespace hisilicon::mpp::vpss {

static MPP_CHN_S* createBindSource(BindSource *source) {
    MPP_CHN_S* param = new MPP_CHN_S();
    param->enModId = source->sourceBindMode();
    param->s32DevId = source->sourceBindDeviceId();
    param->s32ChnId = source->sourceBindChannelId();
    return param;
}

static MPP_CHN_S* createBindReceiver(BindReceiver *source) {
    MPP_CHN_S* param = new MPP_CHN_S();
    param->enModId = source->receiverBindMode();
    param->s32DevId = source->receiverBindDeviceId();
    param->s32ChnId = source->receiverBindChannelId();
    return param;
}

Binder::Binder(BindSource *in, BindReceiver *out)
    : m_source(in), m_receiver(out) {
}

Binder::~Binder() {
    HI_MPI_SYS_UnBind(m_in.get(), m_out.get());
}

bool Binder::configureImpl() {
    m_in.reset(createBindSource(m_source));
    m_out.reset(createBindReceiver(m_receiver));
    return true;
}

bool Binder::startImpl() {
    if (HI_MPI_SYS_Bind(m_in.get(), m_out.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_SYS_Bind failed");

    return true;
}

}
