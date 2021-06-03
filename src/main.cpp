#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "sample_comm.h"

#include "Boards/7004_lm_v3/lm7004v3.h"
#include "nvp6134/ad_cfg.h"


HI_S32 SAMPLE_VENC_4D1_H264(HI_VOID) {
    HI_U32 u32ViChnCnt = 4;
    HI_S32 s32VpssGrpCnt = 4;
    PAYLOAD_TYPE_E enPayLoad[2]= {PT_H264, PT_H264};
  //  VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VENC_GRP VencGrp;
    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR;
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_VENC_8D1_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);

    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_VENC_8D1_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss();

    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_VENC_8D1_2;
    }

    /******************************************
     step 5: start stream venc (big + little)
    ******************************************/
    for (i = 0; i < u32ViChnCnt; i++) {
        /*** main stream **/
        VencGrp = i * 2;
        VencChn = i * 2;
        VpssGrp = i;
        s32Ret = SAMPLE_COMM_VENC_Start(VpssGrp, VencGrp, VencChn, enPayLoad[0], enRcMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8D1_2;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencGrp, VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8D1_3;
        }

        /*** Sub stream **/
        VencGrp ++;
        VencChn ++;
        s32Ret = SAMPLE_COMM_VENC_Start(VpssGrp, VencGrp, VencChn, enPayLoad[1], enRcMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8D1_3;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn, VpssGrp, VPSS_PRE0_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8D1_3;
        }
    }

    /******************************************
     step 6: stream venc process -- get stream, then save it to file.
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(u32ViChnCnt *2 );
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
    for (i = 0; i < u32ViChnCnt * 2 ; i++)
    {
        VencGrp = i;
        VencChn = i;
        VpssGrp = i / 2;
        VpssChn = (VpssGrp%2)?VPSS_PRE0_CHN:VPSS_BSTR_CHN;
        SAMPLE_COMM_VENC_UnBindVpss(VencGrp, VpssGrp, VpssChn);
        SAMPLE_COMM_VENC_Stop(VencGrp,VencChn);
    }
    SAMPLE_COMM_VI_UnBindVpss();
END_VENC_8D1_2:	//vpss stop
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_VENC_8D1_1:	//vi stop
    SAMPLE_COMM_VI_Stop();
END_VENC_8D1_0:	//system exit

    return s32Ret;
}

int main() {
    boards::lm7004v3::Lm7004v3Board* board;

    board = new boards::lm7004v3::Lm7004v3Board();
    board->configure();

    // это временное явление
    initAdCompatLayer(board);

    HI_S32 s32Ret = SAMPLE_VENC_4D1_H264();
    if (HI_SUCCESS == s32Ret)
        printf("program exit normally!\n");
    else
        printf("program exit abnormally!\n");

    exit(s32Ret);
}
