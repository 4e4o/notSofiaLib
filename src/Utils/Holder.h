#ifndef UTILS_HOLDER_H
#define UTILS_HOLDER_H

template<typename TValue>
class Holder {
public:
    Holder(const TValue& v) :
        m_value(v) { }

    TValue value() const {
        return m_value;
    }

private:
    const TValue m_value;
};

#endif // UTILS_HOLDER_H
