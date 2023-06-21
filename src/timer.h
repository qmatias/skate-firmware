#ifndef FIRMWARE_TIMER_H
#define FIRMWARE_TIMER_H

#define TIMER_UNLIMITED -1

class Timer {
  public:
    explicit Timer(unsigned long interval);
    bool start();
    void stop();
    void restart();
    bool waiting();

  private:
    unsigned long mLast;
    unsigned long mInterval;
    bool mRunning;
};

#endif // FIRMWARE_TIMER_H
