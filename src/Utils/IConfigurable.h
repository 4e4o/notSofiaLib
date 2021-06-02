#ifndef I_CONFIGURABLE_H
#define I_CONFIGURABLE_H

class IConfigurable {
public:
    virtual ~IConfigurable() { }
    virtual bool configure() = 0;
};

#endif // I_CONFIGURABLE_H
