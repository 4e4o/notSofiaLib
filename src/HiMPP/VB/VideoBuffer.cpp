#include "VideoBuffer.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"

#include <iostream>

#include <mpi_vb.h>

#define DEFAULT_MAX_POOL_COUNT 128

#define MAIN_POOL_INDEX 0

namespace hisilicon::mpp {

static void addPool(VB_CONF_S& conf, int i, HI_U32 blockSize, HI_U32 blkCount) {
    conf.astCommPool[i].u32BlkSize = blockSize;
    conf.astCommPool[i].u32BlkCnt = blkCount;
}

VideoBuffer::~VideoBuffer() {    
    HI_MPI_VB_Exit();
    std::cout << "~VideoBuffer " << this << std::endl;
}

bool VideoBuffer::startImpl() {
    // на стадии конфигурации нет нужных данных
    // они есть только на стадии запуска
    HI_U32 channelsCount = 0;
    const HI_U32 blockSize = maxViBlkSize(channelsCount);

    if (channelsCount < 1)
        throw std::runtime_error("No channels to process");

    if (blockSize < 1)
        throw std::runtime_error("Invalid block size");

    VB_CONF_S stVbConf{};

    stVbConf.u32MaxPoolCnt = DEFAULT_MAX_POOL_COUNT;

    // TODO why * 4 ??
    // < 3 not works

    // Значит тут имеется зависимость от кол-ва каналов на группу в vpps
    // Сначало было 4 канала на группу, 4 группы vpps и работало только с channelCount * 4
    // Потом сделал 1 канал на группу, 4 группы, стало работать с channelCount * 3,
    // - с 2 уже не работает, зависимость пока не понятна

    // Значит делаем следующее:
    // отключаем подсистемы (venc, vpss, vi)
    // и пытаемся понять сколько какой нужно blckcnt:
    // если mpp не достаточно блоков, то во-первых
    // cat /dev/logmpp будет сыпать ошибки get vb fail, без настройки уровня лога
    // во вторых, cat /proc/umap/vb MinFree смотрим столбец

    // по тестам выводы такие:
    // когда только vi работает подсистема, при 4-х канальном захвате жрётся 8 блоков
    // добавляем vpss на каждый vi канал группу и в каждой группе по одному vpss каналу (без venc)
    // - жрётся так же 8 блоков ( channelsCount * 2)
    // добавляем venc c на каждый vpss канал по каналу:
    // venc юзает свой vb пул, + юзается этот пул почему-то
    // при channelsCount * 8 MinFree == 15 (4 * 8 = 32 ; 32 - 15 = 17 блоков юзается)
    // 17 = (channelsCount * 4 + 1)
    // что за зависимость, хз
    // я думал может venc модулю не хватает блоков из своего пула и он берёт их из основного,
    // но нет, добавлял блоки в venc пул и увеличивал их размер - результат тот же

    // TODO page 40
    // HiMPP Media Processing Software Development Reference.pdf

    // основной пул для vi, vpss и других сервисов у которых нет апи приатачивания к пулу
    addPool(stVbConf, MAIN_POOL_INDEX, blockSize, channelsCount * 4 + 1 + EXTRA_BLOCKS_COUNT);

    if (HI_MPI_VB_SetConf(&stVbConf) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_SetConf failed");

    if (HI_MPI_VB_Init() != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_Init failed");

    return true;
}

HI_U32 VideoBuffer::maxViBlkSize(HI_U32& channelsCount) {
    channelsCount = 0;
    const vi::Subsystem* vi = parent()->vi();

    if (vi == nullptr)
        return 0;

    HI_U32 max = 0, tmp = 0;

    for (auto& device : vi->devices()) {
        for (auto& channel : device->channels()) {
            channelsCount++;
            tmp = picVbBlkSize(channel);

            if (tmp > max)
                max = tmp;
        }
    }

    return max;
}

}
