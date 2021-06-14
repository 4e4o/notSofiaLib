#include "Binder.h"
#include "BindItem.h"

#include <mpi_sys.h>

#include <stdexcept>
#include <iostream>

namespace hisilicon::mpp::vpss {

Binder::Binder(BindItem *in, BindItem *out)
    : m_source(in), m_receiver(out) {
    in->setBindedItem(out, false);
    out->setBindedItem(in, true);
}

Binder::~Binder() {
    HI_MPI_SYS_UnBind(m_in.get(), m_out.get());
}

bool Binder::configureImpl() {
    m_in.reset(createBindItem(m_source, true));
    m_out.reset(createBindItem(m_receiver, false));

    if (HI_MPI_SYS_Bind(m_in.get(), m_out.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_SYS_Bind failed");

    return true;
}

MPP_CHN_S *Binder::createBindItem(BindItem *item, bool source) {
    MPP_CHN_S *param = new MPP_CHN_S();
    param->enModId = item->bindMode(source);
    param->s32DevId = item->bindDeviceId(source);
    param->s32ChnId = item->bindChannelId(source);
    return param;
}

}
