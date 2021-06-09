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

#include "sample_comm.h"

static pthread_t gs_VencPid;
static SAMPLE_VENC_GETSTREAM_PARA_S gs_stPara;

/******************************************************************************
* funciton : get file postfix according palyload_type.
******************************************************************************/
HI_S32 SAMPLE_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char *szFilePostfix)
{
    if (PT_H264 == enPayload)
    {
        strcpy(szFilePostfix, ".h264");
    }
    else if (PT_JPEG == enPayload)
    {
        strcpy(szFilePostfix, ".jpg");
    }
    else if (PT_MJPEG == enPayload)
    {
        strcpy(szFilePostfix, ".mjp");
    }
    else if (PT_MP4VIDEO == enPayload)
    {
        strcpy(szFilePostfix, ".mp4");
    }
    else
    {
        SAMPLE_PRT("payload type err!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


/******************************************************************************
* funciton : save H264 stream
******************************************************************************/
HI_S32 SAMPLE_COMM_VENC_SaveH264(FILE* fpH264File, VENC_STREAM_S *pstStream)
{
    HI_S32 i;

    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        fwrite(pstStream->pstPack[i].pu8Addr[0],
                pstStream->pstPack[i].u32Len[0], 1, fpH264File);

        fflush(fpH264File);

        if (pstStream->pstPack[i].u32Len[1] > 0)
        {
            fwrite(pstStream->pstPack[i].pu8Addr[1],
                    pstStream->pstPack[i].u32Len[1], 1, fpH264File);

            fflush(fpH264File);
        }
    }
    

    return HI_SUCCESS;
}

/******************************************************************************
* funciton : save stream
******************************************************************************/
HI_S32 SAMPLE_COMM_VENC_SaveStream(PAYLOAD_TYPE_E enType,FILE *pFd, VENC_STREAM_S *pstStream)
{
    HI_S32 s32Ret;

    if (PT_H264 == enType)
    {
        s32Ret = SAMPLE_COMM_VENC_SaveH264(pFd, pstStream);
    }
    else
    {
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* funciton : get stream from each channels and save them
******************************************************************************/
HI_VOID* SAMPLE_COMM_VENC_GetVencStreamProc(HI_VOID *p)
{
    HI_S32 i;
    HI_S32 s32ChnTotal;
    VENC_CHN_ATTR_S stVencChnAttr;
    SAMPLE_VENC_GETSTREAM_PARA_S *pstPara;
    HI_S32 maxfd = 0;
    struct timeval TimeoutVal;
    fd_set read_fds;
    HI_S32 VencFd[VENC_MAX_CHN_NUM];
    HI_CHAR aszFileName[VENC_MAX_CHN_NUM][64];
    FILE *pFile[VENC_MAX_CHN_NUM];
    char szFilePostfix[10];
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;
    VENC_CHN VencChn;
    PAYLOAD_TYPE_E enPayLoadType[VENC_MAX_CHN_NUM];
    
    pstPara = (SAMPLE_VENC_GETSTREAM_PARA_S*)p;
    s32ChnTotal = pstPara->s32Cnt;

    /******************************************
     step 1:  check & prepare save-file & venc-fd
    ******************************************/
    if (s32ChnTotal >= VENC_MAX_CHN_NUM)
    {
        SAMPLE_PRT("input count invaild\n");
        return nullptr;
    }
    for (i = 0; i < s32ChnTotal; i++)
    {
        /* decide the stream file name, and open file to save stream */
        VencChn = i;
        s32Ret = HI_MPI_VENC_GetChnAttr(VencChn, &stVencChnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
                       VencChn, s32Ret);
            return nullptr;
        }
        enPayLoadType[i] = stVencChnAttr.stVeAttr.enType;

        s32Ret = SAMPLE_COMM_VENC_GetFilePostfix(enPayLoadType[i], szFilePostfix);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("SAMPLE_COMM_VENC_GetFilePostfix [%d] failed with %#x!\n", \
                       stVencChnAttr.stVeAttr.enType, s32Ret);
            return nullptr;
        }
        sprintf(aszFileName[i], "stream_chn%d%s", i, szFilePostfix);
        pFile[i] = fopen(aszFileName[i], "wb");
        if (!pFile[i])
        {
            SAMPLE_PRT("open file[%s] failed!\n",
                       aszFileName[i]);
            return nullptr;
        }

        /* Set Venc Fd. */
        VencFd[i] = HI_MPI_VENC_GetFd(i);
        if (VencFd[i] < 0)
        {
            SAMPLE_PRT("HI_MPI_VENC_GetFd failed with %#x!\n",
                       VencFd[i]);
            return nullptr;
        }
        if (maxfd <= VencFd[i])
        {
            maxfd = VencFd[i];
        }
    }

    /******************************************
     step 2:  Start to get streams of each channel.
    ******************************************/
    while (HI_TRUE == pstPara->bThreadStart)
    {
        FD_ZERO(&read_fds);
        for (i = 0; i < s32ChnTotal; i++)
        {
            FD_SET(VencFd[i], &read_fds);
        }

        TimeoutVal.tv_sec  = 2;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(maxfd + 1, &read_fds, nullptr, nullptr, &TimeoutVal);
        if (s32Ret < 0)
        {
            SAMPLE_PRT("select failed!\n");
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get venc stream time out, exit thread\n");
            continue;
        }
        else
        {
            for (i = 0; i < s32ChnTotal; i++)
            {
                if (FD_ISSET(VencFd[i], &read_fds))
                {
                    /*******************************************************
                     step 2.1 : query how many packs in one-frame stream.
                    *******************************************************/
                    memset(&stStream, 0, sizeof(stStream));
                    s32Ret = HI_MPI_VENC_Query(i, &stStat);
                    if (HI_SUCCESS != s32Ret)
                    {
                        SAMPLE_PRT("HI_MPI_VENC_Query chn[%d] failed with %#x!\n", i, s32Ret);
                        break;
                    }

                    /*******************************************************
                     step 2.2 : malloc corresponding number of pack nodes.
                    *******************************************************/
                    stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
                    if (nullptr == stStream.pstPack)
                    {
                        SAMPLE_PRT("malloc stream pack failed!\n");
                        break;
                    }
                    
                    /*******************************************************
                     step 2.3 : call mpi to get one-frame stream
                    *******************************************************/
                    stStream.u32PackCount = stStat.u32CurPacks;
                    s32Ret = HI_MPI_VENC_GetStream(i, &stStream, HI_TRUE);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", \
                                   s32Ret);
                        break;
                    }

                    /*******************************************************
                     step 2.4 : save frame to file
                    *******************************************************/
                    s32Ret = SAMPLE_COMM_VENC_SaveStream(enPayLoadType[i], pFile[i], &stStream);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        SAMPLE_PRT("save stream failed!\n");
                        break;
                    }
                    /*******************************************************
                     step 2.5 : release stream
                    *******************************************************/
                    s32Ret = HI_MPI_VENC_ReleaseStream(i, &stStream);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        break;
                    }
                    /*******************************************************
                     step 2.6 : free pack nodes
                    *******************************************************/
                    free(stStream.pstPack);
                    stStream.pstPack = nullptr;
                }
            }
        }
    }

    /*******************************************************
    * step 3 : close save-file
    *******************************************************/
    for (i = 0; i < s32ChnTotal; i++)
    {
        fclose(pFile[i]);
    }

    return nullptr;
}

/******************************************************************************
* funciton : start get venc stream process thread
******************************************************************************/
HI_S32 SAMPLE_COMM_VENC_StartGetStream(HI_S32 s32Cnt)
{
    gs_stPara.bThreadStart = HI_TRUE;
    gs_stPara.s32Cnt = s32Cnt;

    return pthread_create(&gs_VencPid, 0, SAMPLE_COMM_VENC_GetVencStreamProc, (HI_VOID*)&gs_stPara);
}

/******************************************************************************
* funciton : stop get venc stream process.
******************************************************************************/
HI_S32 SAMPLE_COMM_VENC_StopGetStream()
{
    if (HI_TRUE == gs_stPara.bThreadStart)
    {
        gs_stPara.bThreadStart = HI_FALSE;
        pthread_join(gs_VencPid, 0);
    }
    return HI_SUCCESS;
}
