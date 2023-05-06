#ifndef Network_H_
#define Network_H_

class Network
{
public:
    Network(int pin);
    void motorOn();
    void motorOff();
    void motorPWM();

private:
    const int _pin;
    int _speed;

}; // end class

#endif