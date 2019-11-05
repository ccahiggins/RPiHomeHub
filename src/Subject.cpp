#include "Subject.hpp"
#include <algorithm>

using namespace std;

void Subject::attach(Observer *shop)
{
    list.push_back(shop);
}
void Subject::detach(Observer *shop)
{    
    list.erase(std::remove(list.begin(), list.end(), shop), list.end());    
}

void Subject::notify(int room, float temp)
{
    for(vector<Observer*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->update(room, temp);
        }
    }
}