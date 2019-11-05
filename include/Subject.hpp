#ifndef SUBJECT_HPP
#define SUBJECT_HPP


#include <vector>
#include <list>
#include "Observer.hpp"

class Subject
{
    //Lets keep a track of all the shops we have observing
    std::vector<Observer*> list;

public:
    void attach(Observer *product);
    void detach(Observer *product);
    void notify(int room, float temp); 
};

#endif