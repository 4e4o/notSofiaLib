#ifndef EPOLL_OPERATION_TYPE_H
#define EPOLL_OPERATION_TYPE_H

struct EpoolOperationType {
    bool READ : 1 = 0;
    bool WRITE : 1 = 0;
};

#endif // EPOLL_OPERATION_TYPE_H
