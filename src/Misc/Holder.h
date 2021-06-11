#ifndef HOLDER_H
#define HOLDER_H

template<typename TValue>
class Holder {
  public:
    Holder(const TValue &v) :
        m_value(v) { }

    TValue value() const {
        return m_value;
    }

  private:
    const TValue m_value;
};

#endif // HOLDER_H
