/******************************************************************************
  Some simple Hisilicon HI3531 video input functions.

  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-8 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

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


/************** Add By LianZiHao For NVP6134 2017-02-28 Start**********************/

HI_S32 VI_WISDOM_NVP6134_GetVideoState(unsigned int *pu32VideoState,
    HI_S32 s32CodecNum)
{
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


/*HI_S32 VI_WISDOM_NVP6134_GetVideoFMT(nvp6124_input_videofmt *pstVideofmt)
{
    if(NULL == pstVideofmt)
    {
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
*/

/************** Add By LianZiHao For NVP6134 2017-02-28 End **********************/



















/************** Modify By LianZiHao For NVP6134 2017-08-10 Start**********************/
HI_S32 VI_WISDOM_NVP6134_CfgV(VIDEO_NORM_E enVideoMode,SAMPLE_VI_6134_MODE_E enViMode, HI_S32 s32CodecNum) {
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

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? 1 : 0 ;

    SAMPLE_PRT("enVideoMode : %d\n", enVideoMode);

    switch(enViMode) {
        case SAMPLE_VI_MODE_6134_2MUX_D1:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_2MUX_D1!!!\n");
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = NVP6134_VI_720H;
                printf(" ch %d vformat %d chmode %d \n",schnmode.ch,schnmode.vformat,schnmode.chmode);
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 2;
                optmode.portmode = NVP6134_OUTMODE_2MUX_SD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.chipsel = i;
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_2MUX_SD;
                optmode.chid = 1;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_4MUX_D1:
        {//add by lianzihao 2017.8.11
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_4MUX_D1!!!\n");
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = NVP6134_VI_720H;
                printf(" ch %d vformat %d chmode %d \n",schnmode.ch,schnmode.vformat,schnmode.chmode);
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 2;
                optmode.portmode = NVP6134_OUTMODE_4MUX_SD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.chipsel = i;
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_4MUX_SD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_960H:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_960H!!!\n");
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = NVP6134_VI_720H;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 2;
                optmode.portmode = NVP6134_OUTMODE_4MUX_SD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.chipsel = i;
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_4MUX_SD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_HDX:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_HDX!!!\n");
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = NVP6134_VI_720P_2530;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 2;
                optmode.portmode = NVP6134_OUTMODE_4MUX_HD_X;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_4MUX_HD_X;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_HD:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_HD!!!\n");
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = NVP6134_VI_720P_2530;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 2;
                optmode.portmode = NVP6134_OUTMODE_2MUX_HD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_2MUX_HD;
                optmode.chid = 1;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_FHDX:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_FHDX!!!\n");
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
                optmode.portmode = NVP6134_OUTMODE_2MUX_FHD_X;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_2MUX_FHD_X;
                optmode.chid = 1;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_FHD:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_FHD!!!\n");
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
                optmode.portmode = NVP6134_OUTMODE_1MUX_FHD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_1MUX_FHD;
                optmode.chid = 1;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            printf("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_960H_720P_2MUX:
        {
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_960H_720P_2MUX!!!\n");
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = i%2?NVP6134_VI_720P_2530:NVP6134_VI_1920H;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;
                optmode.portsel = 2;
                optmode.portmode = NVP6134_OUTMODE_2MUX_HD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_2MUX_HD;
                optmode.chid = 1;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            SAMPLE_PRT("[%s:%d] SET OK!!!\n", __func__, __LINE__);
            break;
        }
        case SAMPLE_VI_MODE_6134_2MUX_MIX:
        {
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
            break;
        }
        case SAMPLE_VI_MODE_6134_4MUX_HD:
        {
            //add by lianzihao 2017_11_15
            SAMPLE_PRT("SAMPLE_VI_MODE_6134_4MUX_HD!!!\n");

            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = NVP6134_VI_720P_2530;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }

            for(i=0;i<chip_cnt;i++)
            {
                optmode.chipsel = i;// 0 1
                //optmode.portsel = 2;
                //optmode.portmode = NVP6134_OUTMODE_4MUX_HD;
                //optmode.chid = 0;
                //ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                optmode.portsel = 1;
                optmode.portmode = NVP6134_OUTMODE_4MUX_HD;
                optmode.chid = 0;
                ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
            }
            break;
        }
        default:
        {
            SAMPLE_PRT("enViMode %d not supported!!!\n", enViMode);
            break;
        }

        close(fd);
    }

    return 0;
}
/************** Add By LianZiHao For NVP6134 2017-02-20 End **********************/

/************** Add By LianZiHao For NVP6134 2017-02-20 Start**********************/
HI_S32 SAMPLE_WISDOM_VI_ADStart(SAMPLE_VI_MODE_E enViMode,
    VIDEO_NORM_E enNorm,  HI_S32 s32CodecNum)
{
    VI_WORK_MODE_E enWorkMode;
    HI_S32 s32Ret;
    SAMPLE_VI_6134_MODE_E enMode;

    switch (enViMode)
    {
        case SAMPLE_VI_MODE_4_D1:
        {
            SAMPLE_PRT("enViMode SAMPLE_VI_MODE_4_D1\n");

            enMode = SAMPLE_VI_MODE_6134_2MUX_D1;
            SAMPLE_PRT("enWorkMode SAMPLE_VI_MODE_6134_2MUX_D1\n");

            s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, enMode, s32CodecNum);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        }
        case SAMPLE_VI_MODE_8_D1:
        {
            //add by lianzihao 2017.8.11
            SAMPLE_PRT("enViMode SAMPLE_VI_MODE_8_D1\n");

            enWorkMode = SAMPLE_VI_MODE_6134_4MUX_D1;

            SAMPLE_PRT("enWorkMode SAMPLE_VI_MODE_6134_4MUX_D1\n");
            s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, enWorkMode, s32CodecNum);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        }
        case SAMPLE_VI_MODE_16_960H:
        {
            enMode = SAMPLE_VI_MODE_6134_960H;
            s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, enMode, s32CodecNum);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        }
        case SAMPLE_VI_MODE_4_720P:
        {
            SAMPLE_PRT("enViMode SAMPLE_VI_MODE_4_720P\n");
            enMode = SAMPLE_VI_MODE_6134_HD;
            SAMPLE_PRT("enWorkMode SAMPLE_VI_MODE_6134_HD\n");

            s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, enMode, s32CodecNum);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        }
/*        case SAMPLE_VI_MODE_8_720P:
        {   //add by lianzihao 2017.8.11
            SAMPLE_PRT("enViMode SAMPLE_VI_MODE_8_720P\n");

            enMode = SAMPLE_VI_MODE_6134_4MUX_HD;

            SAMPLE_PRT("enWorkMode SAMPLE_VI_MODE_6134_4MUX_HD\n");
            s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, enMode, s32CodecNum);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        }
        case SAMPLE_VI_MODE_8_1080P:
        {
            SAMPLE_PRT("AD nvp6134 not support 8 ch 1080P!\n");
            break;
        }*/
        case SAMPLE_VI_MODE_4_1080P:
        {
            enMode = SAMPLE_VI_MODE_6134_2MUX_MIX;
            s32Ret = VI_WISDOM_NVP6134_CfgV(enNorm, enMode, s32CodecNum);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("VI_WISDOM_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        }
        default:
            SAMPLE_PRT("AD not support!\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}
/************** Add By LianZiHao For NVP6134 2017-02-20 End **********************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
