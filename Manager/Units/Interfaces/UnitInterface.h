#ifndef UNITINTERFACE_H
#define UNITINTERFACE_H

#include "stdint.h"

class Unit;
typedef Unit const * const UnitPtr;

struct alignas(16) Event {
    int type;
    uint32_t epoch;
    UnitPtr parentUnit;
    void* data;
};

typedef struct Event Event;

typedef Event SubscriptionEvent;
typedef Event PublisherEvent;

typedef SubscriptionEvent const * const SubscriptionEventPtr;
typedef PublisherEvent const * const PublisherEventPtr;

// a function pointer EventHandlerCallback that takes an argument of PublisherEventPtr
typedef void (*EventHandlerCallback)(PublisherEventPtr event);
typedef void (*EventHandler)(Event, EventHandlerCallback);

class Unit
{
    public:
        Unit(EventHandler eventHandler) {

            this->eventHandler = eventHandler;

        }
        virtual ~Unit() {}
    
        EventHandler eventHandler = nullptr;
};
#endif
