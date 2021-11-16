#include "stdint.h"

typedef enum EventTypes EventTypes;

typedef struct {
    int type;
    uint32_t epoch;
    void* data;
} Event;

typedef Event SubscriptionEvent;
typedef Event PublisherEvent;

typedef SubscriptionEvent const * const SubscriptionEventPtr;
typedef PublisherEvent const * const PublisherEventPtr;

// a function pointer EventHandlerCallback that takes an argument of PublisherEventPtr
typedef void (*EventHandlerCallback)(PublisherEventPtr event);
typedef void (*EventHandler)(Event, EventHandlerCallback);

class Unit {
    public:
        Unit(EventHandler eventHandler) {

            this->eventHandler = eventHandler;

        }
        virtual ~Unit() {}
    
    private:
        EventHandler eventHandler;
};