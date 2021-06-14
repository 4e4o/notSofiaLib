#ifndef READ_LOOP_DATA_BUFFER__H
#define READ_LOOP_DATA_BUFFER__H

#include <span>
#include <vector>
#include <cstdint>

namespace hisilicon::mpp {

// Буфер данных на один ReadLoop
// Идея такая:
// Несколько объектов могут добавляться в ReadLoop
// и ждать некоторых данных для чтения.
// так как луп живёт в одном потоке мы оптимизируем выделение памяти,
// нет нужды выделять буфер для каждого объекта юзающего луп чтения.
// Мы создаём один буфер и размер его будет увеличиваться по мере увеличения
// размера прочитанных данных для юзающих объектов.

class DataBuffer {
  public:
    DataBuffer();
    ~DataBuffer();

    int8_t *getBuffer(const size_t &, int index = 0);

  private:
    void freeBuffers();
    void freeBuffer(int index);
    void addBuffersIfNecessary(int index);

    typedef std::span<int8_t> TBuffer;

    std::vector<TBuffer> m_buffers;
};

}

#endif // READ_LOOP_DATA_BUFFER__H

