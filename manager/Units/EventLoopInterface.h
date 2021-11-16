#include "./UnitInterface.h"

class EventLoopInterface : public Unit {

    public:
        EventLoopInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~EventLoopInterface() {}
        virtual void enqueue(Event event) = 0;

    private:
        virtual void eventLoop(EventHandler eventHandler) = 0;
        
};