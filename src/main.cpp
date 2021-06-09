#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <stdexcept>
#include <iostream>

#include "sample_comm.h"

#include "Boards/7004_lm_v3/Board.h"

HI_S32 SAMPLE_VENC_4D1_H264(HI_VOID) {
    HI_U32 u32ViChnCnt = 4;
    HI_S32 s32Ret = HI_SUCCESS;

    /******************************************
     step 6: stream venc process -- get stream, then save it to file.
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(u32ViChnCnt/* *2 */);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_VENC_8D1_3;
    }

    printf("please press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    /******************************************
     step 7: exit process
    ******************************************/
    SAMPLE_COMM_VENC_StopGetStream();

END_VENC_8D1_3:

    return s32Ret;
}

int main() {
    boards::lm7004v3::Board* board;

    board = new boards::lm7004v3::Board();

    if (!board->configure())
        throw std::runtime_error("board configure failed");

    if (!board->start())
        throw std::runtime_error("board start failed");

    HI_S32 s32Ret = SAMPLE_VENC_4D1_H264();
    if (HI_SUCCESS == s32Ret)
        printf("program exit normally!\n");
    else
        printf("program exit abnormally!\n");

    delete board;

    return 0;
}
