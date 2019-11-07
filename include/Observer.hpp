#ifndef OBSERVER_HPP
#define OBSERVER_HPP

class Observer
{
public:
    virtual void update(int room, float temp) = 0;
};

#endif