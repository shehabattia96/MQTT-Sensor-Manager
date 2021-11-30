#ifndef EVENTLOOPINTERFACE_H
#define EVENTLOOPINTERFACE_H

#include "./UnitInterface.h"

class EventLoopInterface : public Unit {

    public:
        EventLoopInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~EventLoopInterface() {}
        virtual void enqueue(Event event) = 0;
        virtual void stop() = 0;

    private:
        virtual void eventLoop(EventHandler eventHandler) = 0;
        
};
#endif