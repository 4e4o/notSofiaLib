#ifndef NVP_SAMPLE_DEFS_H
#define NVP_SAMPLE_DEFS_H

#include <nvp6134_ex_170306/common.h>
#include <nvp6134_ex_170306/video.h>

#include "sample_comm.h"

#define NVP6134A_FILE "/dev/nc_vdec"

/************** Add By LianZiHao For NVP6134 2017-02-20 Start**********************/
typedef enum sample_vi_6134_mode_e
{
    /*nextchip nvp6134*/
    SAMPLE_VI_MODE_6134_960H,
    SAMPLE_VI_MODE_6134_HDX,   //640x720
    SAMPLE_VI_MODE_6134_HD,    //720p
    SAMPLE_VI_MODE_6134_FHDX,  //960x1080
    SAMPLE_VI_MODE_6134_FHD,   //1080p
    SAMPLE_VI_MODE_6134_960H_720P_2MUX,
    SAMPLE_VI_MODE_6134_2MUX_FHD,  // 1080p
    SAMPLE_VI_MODE_6134_2MUX_MIX,  // 1080p
    SAMPLE_VI_MODE_6134_4MUX_HD,   // 4mux 720p
    SAMPLE_VI_MODE_6134_2MUX_D1,   // 4mux2 D1
    SAMPLE_VI_MODE_6134_4MUX_D1,   // 4mux1 D1 add by lianzihao 2017.8.11
    SAMPLE_VI_MODE_6134_4MUX_FHD,  // 4mux 1080p add by tmf 20181105
} SAMPLE_VI_6134_MODE_E;

HI_S32 SAMPLE_WISDOM_VI_ADStart(VIDEO_NORM_E enNorm,  HI_S32 s32CodecNum);

#endif // NVP_SAMPLE_DEFS_H
