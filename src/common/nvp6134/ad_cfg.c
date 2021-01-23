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

static nvp6134_input_videofmt g_pstVideofmt;
static struct ChannelInfo g_channelInfo[16];

enum TDetectedFormat {
    DF_NOT_DETECTED = 0x00,

    DF_CVBS_NTSC = 0x01,
    DF_CVBS_PAL = 0x02,

    DF_720P_NTSC = 0x04,
    DF_720P_PAL = 0x08,
    DF_720P_RT_NTSC = 0x51,
    DF_720P_RT_PAL = 0x52,

    DF_1080P_NTSC = 0x40,
    DF_1080P_PAL = 0x80,

    DF_HD_EXC_30P = 0x11,
    DF_HD_EXC_25P = 0x12,
    DF_HD_EXT_30A = 0x31,
    DF_HD_EXT_24A = 0x32,
    DF_HD_EXT_30B = 0x34,
    DF_HD_EXT_25B = 0x38,

    DF_HD_EXC_60P = 0x51,
    DF_HD_EXC_50P = 0x52,
    DF_HD_EXT_60 = 0x54,
    DF_HD_EXT_50 = 0x58,

    DF_FHD_EXC_30P = 0x71,
    DF_FHD_EXC_25P = 0x72,
    DF_FHD_EXT_30P = 0x74,
    DF_FHD_EXT_25P = 0x78,

    DF_QHD_AHD_30P = 0x81,
    DF_QHD_AHD_25P = 0x82,
    DF_QHD_AHD_NRT_15P = 0x83,

    DF_5M_AHD_12d5P = 0xA0,
    DF_5M_20P = 0xA1,
    DF_5M_EXT_12d5P = 0xA2,

    DF_3M_AHD_NRT_18P = 0x90,
    DF_3M_AHD_RT_30P = 0x91,
    DF_3M_AHD_RT_25P = 0x92,
    DF_3M_EXT_NRT_18P = 0x93
};

static enum _nvp6134_vi_mode fmtToNvpMode(enum TDetectedFormat df) {
    switch(df) {
    case DF_CVBS_NTSC:
    case DF_CVBS_PAL:
        //        return NVP6134_VI_960H;
        //        return NVP6134_VI_1280H; // работает !!!
        //                return NVP6134_VI_1440H;
        // TODO
        // вот тут нипонятно нихуя, почему этот режим выбирается при cvbs захвате?
        return NVP6134_VI_1920H; // !!!!
        //        return NVP6134_VI_1440H;
    case DF_1080P_NTSC:
        return NVP6134_VI_1080P_2530;
    default:
        return NVP6134_VI_1080P_NOVIDEO;
    }
}

static int isPal(enum TDetectedFormat df) {
    return (df == DF_CVBS_PAL) || (df == DF_720P_PAL) || (df == DF_720P_RT_PAL) || (df == DF_1080P_PAL);
}

static HI_S32 VI_WISDOM_NVP6134_GetVideoFMT(nvp6134_input_videofmt *pstVideofmt) {
    int i;

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

    for(i = 0; i < 4; i++) {
        printf("i=%d videofmt     =0x%x \n",i,pstVideofmt->getvideofmt[i]);
    }

    close(fd);

    return 0;
}

static HI_S32 VI_WISDOM_NVP6134_CfgV(HI_S32 s32CodecNum) {
    int fd, i;
    nvp6134_opt_mode optmode;
    nvp6134_chn_mode schnmode;
    int chip_cnt = s32CodecNum;

    fd = open(NVP6134A_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("open nvp6134 (%s) fail\n", NVP6134A_FILE);
        return -1;
    }

    SAMPLE_PRT("SAMPLE_VI_MODE_6134_2MUX_MIX!!!\n");

    for(i=0;i<chip_cnt*4;i++)
    {
        schnmode.ch = i;
        schnmode.vformat = isPal(g_pstVideofmt.getvideofmt[i]) ? 1 : 0;
        schnmode.chmode = fmtToNvpMode(g_pstVideofmt.getvideofmt[i]);
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


static HI_S32 SAMPLE_COMM_SYS_GetPicSizeMYs(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
    switch (enPicSize)
    {
    case PIC_CIF:
        pstSize->u32Width = D1_WIDTH / 2;
        pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
        break;

    case PIC_D1:
        //1920x576i(480)
        // TODO fix it !!!!! wtf ????
        pstSize->u32Width = 1920 / 2 ;//960;//1280;//1920;
        pstSize->u32Height = 480;//576;
        //  pstSize->u32Width = D1_WIDTH;
        //pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
        break;
    case PIC_HD1080:
        // TODO / 2 ???
        pstSize->u32Width = 1920  / 2 ;
        pstSize->u32Height = 1080;
        break;
    default:
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


static PIC_SIZE_E fmtToPicSizeType(enum TDetectedFormat df) {
    switch(df) {
    case DF_CVBS_NTSC:
    case DF_CVBS_PAL:
        return PIC_D1;
    case DF_1080P_NTSC:
        return PIC_HD1080;
    default:
        return PIC_BUTT;
    }
}

SIZE_S limitedByHI3520DChSize(SIZE_S s) {
    SIZE_S res = s;

    if(res.u32Width > 960)
        res.u32Width = 960;

    return res;
}

static void initPicSize(struct ChannelInfo* chi) {
    if (SAMPLE_COMM_SYS_GetPicSizeMYs(chi->norm, chi->sizeType, &(chi->stDestSize)) != HI_SUCCESS) {
        SAMPLE_PRT("1. not implemented size type %i !\n", chi->sizeType);
        return;
    }

    if (SAMPLE_COMM_SYS_GetPicSize(chi->norm, chi->sizeType, &(chi->stPicSize)) != HI_SUCCESS) {
        SAMPLE_PRT("2. not implemented size type %i !\n", chi->sizeType);
        return;
    }

    chi->stPicSize = limitedByHI3520DChSize(chi->stPicSize);

    // TODO fix it !!!
    // ну здесь понятно мы ограничиваем макс возможным сайзом
    /*    if (chi->stDestSize.u32Width > 960) {
        chi->stDestSize.u32Width = 960;
    }

    if (chi->stDestSize.u32Height > 1080) {
        chi->stDestSize.u32Height = 1080;
    }
*/

    chi->stCapRect.s32X = 0;
    chi->stCapRect.s32Y = 0;
    chi->stCapRect.u32Width = chi->stDestSize.u32Width;
    chi->stCapRect.u32Height = chi->stDestSize.u32Height;

    // здесь тоже всё понятно
    //
    switch(chi->sizeType) {
    case PIC_D1: {
        chi->scanMode = VI_SCAN_INTERLACED;
        // TODO whyyyyy????
        chi->stCapRect.u32Width *= 2;
        break;
    }
    default: {
        chi->scanMode = VI_SCAN_PROGRESSIVE;
        break;
    }
    }
}

int fromMPPChannelIndexToNvp(int index) {
    switch(index) {
    case 0:
        return 2;
    case 1:
        return 3;
    case 2:
        return 0;
    case 3:
        return 1;
    }

    return index;
}

const struct ChannelInfo* getChannelInfo(int ch) {
    const int chIndex = fromMPPChannelIndexToNvp(ch);
    return &g_channelInfo[chIndex];
}

void adInit() {
    int i;
    memset(&g_pstVideofmt, 0, sizeof(g_pstVideofmt));
    VI_WISDOM_NVP6134_GetVideoFMT(&g_pstVideofmt);

    for (i = 0 ; i < 16 ; i++) {
        if (g_pstVideofmt.getvideofmt[i] == DF_NOT_DETECTED) {
            g_channelInfo[i].norm = VIDEO_ENCODING_MODE_NTSC;
            g_channelInfo[i].sizeType = PIC_CIF;
        } else {
            g_channelInfo[i].norm = isPal(g_pstVideofmt.getvideofmt[i]) ? VIDEO_ENCODING_MODE_PAL : VIDEO_ENCODING_MODE_NTSC;
            g_channelInfo[i].sizeType = fmtToPicSizeType(g_pstVideofmt.getvideofmt[i]);
        }

        initPicSize(&g_channelInfo[i]);
    }
}

HI_S32 SAMPLE_WISDOM_VI_ADStart(HI_S32 s32CodecNum) {
    HI_S32 s32Ret = VI_WISDOM_NVP6134_CfgV(s32CodecNum);

    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                   s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
