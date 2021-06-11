#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

class Configurable {
  public:
    virtual ~Configurable();

    bool configure();

  protected:
    Configurable();

    virtual bool configureImpl();

    bool configured() const;

  private:
    bool m_configured;
};

#endif // CONFIGURABLE_H
