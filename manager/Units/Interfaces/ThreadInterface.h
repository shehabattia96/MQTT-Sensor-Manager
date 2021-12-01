#ifndef THREADINTERFACE_H
#define THREADINTERFACE_H

#include "./UnitInterface.h"

class ThreadInterface : public Unit {
    public:
        ThreadInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~ThreadInterface() {};

        virtual void join() = 0;
        virtual void detach() = 0;
        virtual bool joinable() = 0;
};

#endif