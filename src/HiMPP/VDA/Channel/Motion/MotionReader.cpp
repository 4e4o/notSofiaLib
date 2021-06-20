#include "MotionReader.h"
#include "MotionBuffer.h"

#include <mpi_vda.h>
#include <iostream>

namespace hisilicon::mpp::vda {

static void printSadInfo(const VDA_DATA_S &data) {
    const VDA_MD_DATA_S &md = data.unData.stMdData;

    if (md.bMbSadValid != HI_TRUE) {
        std::cout << "bMbSadValid is false!" << std::endl;
        return;
    }

    std::cout << "printSadInfo: " << data.u32MbHeight << std::endl;

    for (HI_U32 i = 0; i < data.u32MbHeight; i++) {
        HI_VOID *pAddr = (HI_VOID *)((HI_U32) md.stMbSadData.pAddr + i *
                                     md.stMbSadData.u32Stride);

        for (HI_U32 j = 0; j < data.u32MbWidth; j++) {
            if (VDA_MB_SAD_8BIT == md.stMbSadData.enMbSadBits) {
                HI_U8 *pu8Addr = (HI_U8 *)pAddr + j;
                std::cout << std::hex << *pu8Addr;
            } else {
                HI_U16 *pu16Addr = (HI_U16 *)pAddr + j;
                std::cout << std::hex << *pu16Addr;
            }

            std::cout << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

static void printObjInfo(const VDA_DATA_S &data) {
    const VDA_MD_DATA_S &md = data.unData.stMdData;

    if (md.bObjValid != HI_TRUE) {
        std::cout << "bObjValid is false!" << std::endl;
        return;
    }

    std::cout << "printObjInfo: " << md.stObjData.u32ObjNum << std::endl;

    for (HI_U32 i = 0; i < md.stObjData.u32ObjNum; i++) {
        VDA_OBJ_S *pstVdaObj = md.stObjData.pstAddr + i;
        std::cout << "[" << i << "]\t" <<
                  " left = " << pstVdaObj->u16Left << ", " <<
                  " top = " << pstVdaObj->u16Top << ", " <<
                  " right = " << pstVdaObj->u16Right << ", " <<
                  " bottom = " << pstVdaObj->u16Bottom << std::endl;
    }

    std::cout << std::endl;
}

static void printAlarmPixels(const VDA_DATA_S &data) {
    const VDA_MD_DATA_S &md = data.unData.stMdData;

    if (md.bPelsNumValid != HI_TRUE) {
        std::cout << "bPelsNumValid is false!" << std::endl;
        return;
    }

    std::cout << "printAlarmPixels: " << md.u32AlarmPixCnt << std::endl;
}

void MotionReader::allocate(const VDA_CHN_STAT_S &stat, VDA_DATA_S &data) {
}

void MotionReader::onData(const VDA_DATA_S &data) {
    printSadInfo(data);
    printObjInfo(data);
    printAlarmPixels(data);
}

DataBufferWrapper *MotionReader::createBufferWrapper(DataBuffer *d) {
    return new MotionBuffer(d);
}

}
