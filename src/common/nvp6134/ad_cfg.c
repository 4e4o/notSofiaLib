#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "ad_cfg.h"

HI_S32 VI_WISDOM_NVP6134_GetVideoFMT(nvp6134_input_videofmt *pstVideofmt);

HI_S32 VI_WISDOM_NVP6134_GetVideoState(unsigned int *pu32VideoState, HI_S32 s32CodecNum) {
    if(NULL == pu32VideoState)
    {
        printf("[%s:%d] Para Error!\n", __func__, __LINE__);
        return -1;
    }

    int fd = -1;
    unsigned char ucdate = 0;
    int chip_cnt = s32CodecNum;

    fd = open(NVP6134A_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("[%s:%d] open nvp6134 (%s) fail\n", __func__, __LINE__, NVP6134A_FILE);
        return -1;
    }

    ioctl(fd, IOC_VDEC_GET_VIDEO_LOSS, pu32VideoState);

    if(2 == chip_cnt)
    {
        ucdate = (unsigned char)((*pu32VideoState)&0x3f);
    }
    else
    {
        ucdate = (unsigned char)((*pu32VideoState)&0xf);
        ucdate = (ucdate<<2)|(ucdate>>2);
        ucdate=((ucdate<<1)&0xa)|((ucdate>>1)&0x5);
    }

    //printf("[%s,%d]:chip_cnt=0x%x,pu32VideoState=0x%x,ucdate=0x%x\n",__FILE__,__LINE__,chip_cnt,*pu32VideoState,ucdate);
    //printf("[video_state : %#x][ucdate : %#x]\n", ((*pu32VideoState)&0xf), ucdate);

    *pu32VideoState = ucdate;

    close(fd);

    return 0;
}


HI_S32 VI_WISDOM_NVP6134_GetVideoFMT(nvp6134_input_videofmt *pstVideofmt) {
    if(NULL == pstVideofmt) {
        printf("[%s:%d] Para Error!\n", __func__, __LINE__);
        return -1;
    }

    int fd = -1;

    fd = open(NVP6134A_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("[%s:%d] open nvp6134 (%s) fail\n", __func__, __LINE__, NVP6134A_FILE);
        return -1;
    }

    ioctl(fd, IOC_VDEC_GET_INPUT_VIDEO_FMT, pstVideofmt);

    //printf("[%s:%d] [pstVideofmt : %#x]\n", __func__, __LINE__, *pstVideofmt);

    close(fd);

    return 0;
}

HI_S32 VI_WISDOM_NVP6134_CfgV(VIDEO_NORM_E enVideoMode, HI_S32 s32CodecNum) {
    int fd, i;
    int video_mode;
    nvp6134_opt_mode optmode;
    nvp6134_chn_mode schnmode;

    int chip_cnt = s32CodecNum;

    fd = open(NVP6134A_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("open nvp6134 (%s) fail\n", NVP6134A_FILE);
        return -1;
    }

    nvp6134_input_videofmt pstVideofmt;
    VI_WISDOM_NVP6134_GetVideoFMT(&pstVideofmt);

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? 1 : 0 ;

    SAMPLE_PRT("enVideoMode : %d\n", enVideoMode);
    SAMPLE_PRT("SAMPLE_VI_MODE_6134_2MUX_MIX!!!\n");

    for(i=0;i<chip_cnt*4;i++)
    {
        schnmode.ch = i;
        schnmode.vformat = video_mode;
        schnmode.chmode = NVP6134_VI_1080P_2530;
        ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
    }

    for(i=0;i<chip_cnt;i++)
    {
        optmode.chipsel = i;
        optmode.portsel = 2;
        optmode.portmode = NVP6134_OUTMODE_2MUX_MIX;
        optmode.chid = 0;
        ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
        optmode.portsel = 1;
        optmode.portmode = NVP6134_OUTMODE_2MUX_MIX;
        optmode.chid = 1;
        ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
    }
    printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);

    close(fd);


    return 0;
}

HI_S32 SAMPLE_WISDOM_VI_ADStart(VIDEO_NORM_E enNorm,  HI_S32 s32CodecNum) {
    HI_S32 s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, s32CodecNum);

    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                   s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
