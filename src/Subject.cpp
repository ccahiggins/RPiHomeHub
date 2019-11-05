#include "Subject.hpp"
#include <algorithm>

using namespace std;

void Subject::Attach(Observer *shop)
{
    list.push_back(shop);
}
void Subject::Detach(Observer *shop)
{    
    list.erase(std::remove(list.begin(), list.end(), shop), list.end());    
}

void Subject::Notify(float price)
{
    for(vector<Observer*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->update(price);
        }
    }
}