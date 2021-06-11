#ifndef STARTABLE_H
#define STARTABLE_H

class Startable {
  public:
    virtual ~Startable();

    bool start();

  protected:
    Startable();

    virtual bool startImpl();

    bool started() const;

  private:
    bool m_started;
};

#endif // STARTABLE_H
