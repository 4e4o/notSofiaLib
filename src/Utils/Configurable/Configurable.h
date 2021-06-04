#ifndef I_CONFIGURABLE_H
#define I_CONFIGURABLE_H

class Configurable {
public:
    virtual ~Configurable();

    bool configure();

protected:
    Configurable();

    virtual bool configureImpl() = 0;

    bool configured() const;
private:
    bool m_configured;
};

#endif // I_CONFIGURABLE_H
