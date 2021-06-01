#ifndef ID_HOLDER_H
#define ID_HOLDER_H

class IdHolder {
public:
    IdHolder(int id);

    int id() const;

private:

    int m_id;
};

#endif // ID_HOLDER_H
