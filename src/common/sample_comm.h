#ifndef __SAMPLE_COMM_H__
#define __SAMPLE_COMM_H__

#include "hi_common.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vpss.h"
#include "hi_comm_vdec.h"
#include "hi_comm_vda.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_comm_hdmi.h"
#include "hi_defines.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vpss.h"
#include "mpi_vdec.h"
#include "mpi_vda.h"
#include "mpi_region.h"
#include "mpi_adec.h"
#include "mpi_aenc.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "mpi_hdmi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/*******************************************************
    macro define 
*******************************************************/
#define ALIGN_BACK(x, a)              ((a) * (((x) / (a))))
#define SAMPLE_SYS_ALIGN_WIDTH      64
#define SAMPLE_PIXEL_FORMAT         PIXEL_FORMAT_YUV_SEMIPLANAR_422
#define SAMPLE_COMM_VI_GetSubChn(x) x+16

#define D1_WIDTH 704

/*** define vi chn(2/6/10/14)'s sub_chn size                                          */
/***           vi chn(0/4/8/12)'s sub_chn size define D1 in sample program */
#define SAMPLE_VI_SUBCHN_2_W 720
#define SAMPLE_VI_SUBCHN_2_H  640

#define SAMPLE_PRT(fmt...)   \
    do {\
        printf("[%s]-%d: ", __FUNCTION__, __LINE__);\
        printf(fmt);\
       }while(0)

typedef enum 
{
    VI_DEV_BT656_D1_1MUX = 0,
    VI_DEV_BT656_D1_4MUX,
    VI_DEV_BT656_960H_1MUX,
    VI_DEV_BT656_960H_4MUX,
    VI_DEV_720P_HD_1MUX,
    VI_DEV_1080P_HD_1MUX,
    VI_DEV_BUTT
}SAMPLE_VI_DEV_TYPE_E;

typedef enum sample_vi_chn_set_e
{
    VI_CHN_SET_NORMAL = 0, /* mirror, filp close */
    VI_CHN_SET_MIRROR,      /* open MIRROR */
    VI_CHN_SET_FILP		/* open filp */
}SAMPLE_VI_CHN_SET_E;

typedef enum sample_vo_mode_e
{
    VO_MODE_1MUX  = 0,
    VO_MODE_4MUX = 1,
    VO_MODE_9MUX = 2,
    VO_MODE_16MUX = 3,
    VO_MODE_BUTT
}SAMPLE_VO_MODE_E;

typedef struct hisample_MEMBUF_S
{
    VB_BLK  hBlock;
    VB_POOL hPool;
    HI_U32  u32PoolId;
    
    HI_U32  u32PhyAddr;
    HI_U8   *pVirAddr;
    HI_S32  s32Mdev;
} SAMPLE_MEMBUF_S;

typedef enum sample_rc_e
{
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_FIXQP
}SAMPLE_RC_E;

typedef enum sample_rgn_change_type_e
{
    RGN_CHANGE_TYPE_FGALPHA = 0,
    RGN_CHANGE_TYPE_BGALPHA,
    RGN_CHANGE_TYPE_LAYER
}SAMPLE_RGN_CHANGE_TYPE_EN;


/*******************************************************
    structure define 
*******************************************************/
typedef struct sample_vi_param_s
{
    HI_S32 s32ViDevCnt;		// VI Dev Total Count
    HI_S32 s32ViDevInterval;	// Vi Dev Interval
    HI_S32 s32ViChnCnt;		// Vi Chn Total Count
    HI_S32 s32ViChnInterval;	// VI Chn Interval
}SAMPLE_VI_PARAM_S;

typedef struct sample_vo_param_s
{
    VO_DEV VoDev;
    HI_CHAR acMmzName[20];
    HI_U32 u32WndNum;
    SAMPLE_VO_MODE_E enVoMode;
    VO_PUB_ATTR_S stVoPubAttr;
    HI_BOOL bVpssBind;
}SAMPLE_VO_PARAM_S;

typedef struct sample_video_loss_s
{
    HI_BOOL bStart;
    pthread_t Pid;
//    SAMPLE_VI_MODE_E enViMode;
} SAMPLE_VIDEO_LOSS_S;


typedef struct sample_venc_getstream_s
{
     HI_BOOL bThreadStart;
     HI_S32  s32Cnt;
}SAMPLE_VENC_GETSTREAM_PARA_S;

/*******************************************************
    function announce  
*******************************************************/
HI_S32 SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize);

HI_U32 MaxPicVbBlkSize(PIXEL_FORMAT_E enPixFmt, HI_U32 u32AlignWidth);

HI_U32 SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, PIXEL_FORMAT_E enPixFmt, HI_U32 u32AlignWidth);
HI_VOID SAMPLE_COMM_SYS_Exit(void);
HI_S32 SAMPLE_COMM_SYS_Init(VB_CONF_S *pstVbConf);

HI_S32 SAMPLE_COMM_VI_Mode2Param(SAMPLE_VI_PARAM_S *pstViParam);
//HI_S32 SAMPLE_COMM_VI_Mode2Size(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm, RECT_S *pstCapRect, SIZE_S *pstDestSize);
VI_DEV SAMPLE_COMM_VI_GetDev(VI_CHN ViChn);
HI_S32 SAMPLE_COMM_VI_StartDev(VI_DEV ViDev);
HI_S32 SAMPLE_COMM_VI_StartChn(int chIndex, VI_CHN ViChn, SAMPLE_VI_CHN_SET_E enViChnSet);
HI_S32 SAMPLE_COMM_VI_Start();
HI_S32 SAMPLE_COMM_VI_Stop();
HI_S32 SAMPLE_COMM_VI_BindVpss();
HI_S32 SAMPLE_COMM_VI_UnBindVpss();
HI_S32 SAMPLE_COMM_VI_GetSubChnSize(VI_CHN ViChn_Sub, VIDEO_NORM_E enNorm, SIZE_S *pstSize);
	
HI_S32 SAMPLE_COMM_VPSS_Start(HI_S32 s32GrpCnt, HI_S32 s32ChnCnt);
HI_S32 SAMPLE_COMM_VPSS_Stop(HI_S32 s32GrpCnt, HI_S32 s32ChnCnt) ;
HI_S32 SAMPLE_COMM_DisableVpssPreScale(VPSS_GRP VpssGrp,SIZE_S stSize);
HI_S32 SAMPLE_COMM_EnableVpssPreScale(VPSS_GRP VpssGrp,SIZE_S stSize);

HI_S32 SAMPLE_COMM_VENC_MemConfig(HI_VOID);
HI_S32 SAMPLE_COMM_VENC_Start(VPSS_GRP Vpssgrp, VENC_GRP VencGrp,VENC_CHN VencChn, PAYLOAD_TYPE_E enType, SAMPLE_RC_E enRcMode);
HI_S32 SAMPLE_COMM_VENC_Stop(VENC_GRP VencGrp,VENC_CHN VencChn);
HI_S32 SAMPLE_COMM_VENC_SnapStart(VENC_GRP VencGrp,VENC_CHN VencChn, SIZE_S *pstSize);
HI_S32 SAMPLE_COMM_VENC_SnapProcess(VENC_GRP VencGrp, VENC_CHN VencChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
HI_S32 SAMPLE_COMM_VENC_SnapProcessEx(VENC_GRP VencGrp, VENC_CHN VencChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
HI_S32 SAMPLE_COMM_VENC_SnapStop(VENC_GRP VencGrp,VENC_CHN VencChn);
HI_S32 SAMPLE_COMM_VENC_StartGetStream(HI_S32 s32Cnt);
HI_S32 SAMPLE_COMM_VENC_StopGetStream();
HI_S32 SAMPLE_COMM_VENC_BindVpss(VENC_GRP GrpChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
HI_S32 SAMPLE_COMM_VENC_UnBindVpss(VENC_GRP GrpChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);

HI_S32 SAMLE_COMM_VDEC_BindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp);
HI_S32 SAMLE_COMM_VDEC_UnBindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp);
HI_S32 SAMLE_COMM_VDEC_BindVo(VDEC_CHN VdChn, VO_DEV VoDev, VO_CHN VoChn);
HI_S32 SAMLE_COMM_VDEC_UnBindVo(VDEC_CHN VdChn, VO_DEV VoDev, VO_CHN VoChn);
HI_S32 SAMPLE_COMM_VDEC_MemConfig(HI_VOID);

HI_S32 SAMPLE_COMM_VENC_PlanToSemi(HI_U8 *pY, HI_S32 yStride, 
                       HI_U8 *pU, HI_S32 uStride,
					   HI_U8 *pV, HI_S32 vStride, 
					   HI_S32 picWidth, HI_S32 picHeight);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __SAMPLE_COMMON_H__ */
