// Now starting learning c ++
#include "Network.h"

Network::Network(const int pin) : _pin(pin), _duty()
{
    _pin = pin;
    _speed = 0;
}
void Network::motorOn()
{
    digitalWrite(_pin, HIGH);
}
void Network::motorOff()
{
    digitalWrite(_pin, LOW);
}
void Network::motorPWM()
{
    analogWrite(_pin, _duty);
}
void Network::setSpeed(int speed)
{
    _duty = speed;
}

private:
const int _pin;
int _duty;