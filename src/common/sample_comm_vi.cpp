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

#include "nvp6134/ad_cfg.h"

#include "sample_comm.h"

static VI_DEV_ATTR_S DEV_ATTR_BT656D1_2MUX = {
    VI_MODE_BT656,
    VI_WORK_MODE_2Multiplex,
    { 0xFF000000, 0x0 },
    VI_SCAN_PROGRESSIVE,
    { -1, -1, -1, -1 },
    VI_INPUT_DATA_YVYU,
    {},
    VI_PATH_BYPASS,
    VI_DATA_TYPE_YUV,
    HI_TRUE
};


/*****************************************************************************
* function : set vi mask.
*****************************************************************************/
HI_VOID SAMPLE_COMM_VI_SetMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr)
{
    switch (ViDev % 4)
    {
    case 0:
        pstDevAttr->au32CompMask[0] = 0xFF000000;
        if (VI_MODE_BT1120_STANDARD == pstDevAttr->enIntfMode)
        {
            pstDevAttr->au32CompMask[1] = 0x00FF0000;
        }
        else if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
        {
            pstDevAttr->au32CompMask[1] = 0x0;
        }
        break;
    case 1:
        pstDevAttr->au32CompMask[0] = 0xFF0000;
        if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
        {
            pstDevAttr->au32CompMask[1] = 0x0;
        }
        break;
    case 2:
        pstDevAttr->au32CompMask[0] = 0xFF00;
        if (VI_MODE_BT1120_STANDARD == pstDevAttr->enIntfMode)
        {
            pstDevAttr->au32CompMask[1] = 0xFF;
        }
        else if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
        {
            pstDevAttr->au32CompMask[1] = 0x0;
        }
        break;
    case 3:
        pstDevAttr->au32CompMask[0] = 0xFF;
        if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
        {
            pstDevAttr->au32CompMask[1] = 0x0;
        }
        break;
    default:
        HI_ASSERT(0);
    }
}

HI_S32 SAMPLE_COMM_VI_Mode2Param(SAMPLE_VI_PARAM_S *pstViParam) {
    pstViParam->s32ViDevCnt = 2;
    pstViParam->s32ViDevInterval = 1;
    pstViParam->s32ViChnCnt = 4;
    pstViParam->s32ViChnInterval = 2;

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi dev (cfg vi_dev_attr; set_dev_cfg; enable dev)
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_StartDev(VI_DEV ViDev)
{
    HI_S32 s32Ret;
    VI_DEV_ATTR_S stViDevAttr;

    memcpy(&stViDevAttr,&DEV_ATTR_BT656D1_2MUX,sizeof(stViDevAttr));
    SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
    
    s32Ret = HI_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_EnableDev(ViDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi chn
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_StartChn(int chIndex, VI_CHN ViChn, SAMPLE_VI_CHN_SET_E enViChnSet)
{
    HI_S32 s32Ret;
    VI_CHN_ATTR_S stChnAttr;
    VI_CHN_BIND_ATTR_S stChnBindAttr;
    const struct ChannelInfo* channelInfo = getChannelInfo(chIndex);

    // TODO
    // HiMPP Media Processing Software Development Reference.pdf
    // page 121

    if (chIndex == 0) {
        stChnBindAttr.ViDev = 0;
        stChnBindAttr.ViWay = 0;
    } else if (chIndex == 1) {
        stChnBindAttr.ViDev = 0;
        stChnBindAttr.ViWay = 1;
    } else if (chIndex == 2) {
        stChnBindAttr.ViDev = 1;
        stChnBindAttr.ViWay = 0;
    } else if (chIndex == 3) {
        stChnBindAttr.ViDev = 1;
        stChnBindAttr.ViWay = 1;
    }

    {
        s32Ret = HI_MPI_VI_ChnUnBind(ViChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VI_ChnUnBind failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        s32Ret = HI_MPI_VI_ChnBind(ViChn, &stChnBindAttr);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VI_ChnBind failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    /* step  5: config & start vicap dev */

    memcpy(&stChnAttr.stCapRect, &channelInfo->stCapRect, sizeof(RECT_S));
    memcpy(&stChnAttr.stDestSize, &channelInfo->stDestSize, sizeof(SIZE_S));

    stChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;   /* sp420 or sp422 */
    stChnAttr.bMirror = (VI_CHN_SET_MIRROR == enViChnSet)?HI_TRUE:HI_FALSE;
    stChnAttr.bFlip = (VI_CHN_SET_FILP == enViChnSet)?HI_TRUE:HI_FALSE;
    stChnAttr.enCapSel = VI_CAPSEL_BOTH;
    stChnAttr.bChromaResample = HI_FALSE;
    stChnAttr.s32SrcFrameRate = -1;
    stChnAttr.s32FrameRate = -1;

    s32Ret = HI_MPI_VI_SetChnScanMode(ViChn, channelInfo->scanMode);

    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_EnableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi according to product type
*            if vi input is hd, we will start sub-chn for cvbs preview
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_Start()
{
    VI_DEV ViDev;
    VI_CHN ViChn;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;
    
    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(&stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }
    
    /*** Start VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = SAMPLE_COMM_VI_StartDev(ViDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    // TODO
    // id каналов фиксированы и зависят от хики чипа И режима его работы!
    // в нашем случае 4channel 720p режим для hi3520d v200
    // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
    // page 616
    
    /*** Start VI Chn ***/
    for(i=0; i<stViParam.s32ViChnCnt; i++)
    {
        ViChn = i * stViParam.s32ViChnInterval;
        
        s32Ret = SAMPLE_COMM_VI_StartChn(i, ViChn, VI_CHN_SET_NORMAL);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("call SAMPLE_COMM_VI_StarChn failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
/*****************************************************************************
* function : stop vi accroding to product type
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_Stop()
{
    VI_DEV ViDev;
    VI_CHN ViChn;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;

    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(&stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }

    /*** Stop VI Chn ***/
    for(i=0;i<stViParam.s32ViChnCnt;i++)
    {
        /* Stop vi phy-chn */
        ViChn = i * stViParam.s32ViChnInterval;
        s32Ret = HI_MPI_VI_DisableChn(ViChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopChn failed with %#x\n",s32Ret);
            return HI_FAILURE;
        }
        /* HD mode, we will stop vi sub-chn */
        /*        if (HI_TRUE == SAMPLE_COMM_VI_IsHd(enViMode))
        {
            ViChn += 16;
            s32Ret = HI_MPI_VI_DisableChn(ViChn);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("SAMPLE_COMM_VI_StopChn failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }
        }*/
    }

    /*** Stop VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = HI_MPI_VI_DisableDev(ViDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : Vi chn bind vpss group
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_BindVpss()
{
    HI_S32 j, s32Ret;
    VPSS_GRP VpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_CHN ViChn;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(&stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }
    
    VpssGrp = 0;
    for (j=0; j<stViParam.s32ViChnCnt; j++)
    {
        ViChn = j * stViParam.s32ViChnInterval;
        
        stSrcChn.enModId = HI_ID_VIU;
        stSrcChn.s32DevId = 0;
        stSrcChn.s32ChnId = ViChn;

        stDestChn.enModId = HI_ID_VPSS;
        stDestChn.s32DevId = VpssGrp;
        stDestChn.s32ChnId = 0;

        s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        
        VpssGrp ++;
    }
    return HI_SUCCESS;
}


/*****************************************************************************
* function : Vi chn unbind vpss group
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_UnBindVpss()
{
    HI_S32 i, j, s32Ret;
    VPSS_GRP VpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_DEV ViDev;
    VI_CHN ViChn;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(&stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }
    
    VpssGrp = 0;
    for (i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;

        for (j=0; j<stViParam.s32ViChnCnt; j++)
        {
            ViChn = j * stViParam.s32ViChnInterval;
            
            stSrcChn.enModId = HI_ID_VIU;
            stSrcChn.s32DevId = ViDev;
            stSrcChn.s32ChnId = ViChn;

            stDestChn.enModId = HI_ID_VPSS;
            stDestChn.s32DevId = VpssGrp;
            stDestChn.s32ChnId = 0;

            s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("failed with %#x!\n", s32Ret);
                return HI_FAILURE;
            }
            
            VpssGrp ++;
        }
    }
    return HI_SUCCESS;
}
