#ifndef SUBJECT_CLASS
#define SUBJECT_CLASS

#include "Observer.h"
#include <cstddef>

template <class TEventArgs>
class Subject
{
  public:
    Subject() {
        _observers = new Observer<TEventArgs> *[3];
    }
    void attach(Observer<TEventArgs> *observer)
    {
        _observers[observerCount] = observer;
        observerCount++;
    }
    void deattach(Observer<TEventArgs> *observer)
    {
        //TODO: Method not implemented
    }
    void notify(TEventArgs args)
    {
         for (int i = 0; i < observerCount; i++) {
            _observers[i]->notify(args);
        }
    }

  private:
    Observer<TEventArgs> **_observers = NULL;
    int observerCount = 0;

};

#endif