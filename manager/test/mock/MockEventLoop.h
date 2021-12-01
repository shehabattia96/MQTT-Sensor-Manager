#ifndef MOCKEVENTLOOP_H
#define MOCKEVENTLOOP_H

#include <iostream>

#include "../../Units/ApplicationUnits.h"

class MockUnit : public Unit {

    public:
        MockUnit(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~MockUnit() {}
};

void mockUnitEventHandler(Event event, EventHandlerCallback callback);
MockUnit mockUnit = {&mockUnitEventHandler};

enum MockEventTypes {MockEvent, MockCallbackEvent};
PublisherEvent mockCallbackEvent = {
    MockEventTypes::MockCallbackEvent,
    0,
    &mockUnit,
    nullptr
};
SubscriptionEvent mockEvent = {
    MockEventTypes::MockEvent,
    0, 
    &mockUnit,
    nullptr
};

struct MockEventLoopMetadata {
    // the event loop handler should be able to recognize the event.
    bool eventLoopHandlerReceivedEvent = false;
    // the unit event handler should be able to recognize the event.
    bool unitEventHandlerReceivedEvent = false;
    // the unit event handler should call the callback function.
    bool unitEventHandlerReceivedCallback = false;
};

void mockUnitEventHandler(Event event, EventHandlerCallback callback) {
    std::cout << "mockUnitEventHandler: got event: " << event.type << std::endl;
    switch (event.type) {
        case MockEventTypes::MockEvent:
            ((MockEventLoopMetadata*)event.data)->unitEventHandlerReceivedEvent = true;
    }
    if (callback != nullptr) {
        mockCallbackEvent.data = event.data;
        return callback(&mockCallbackEvent); 
    }
};

void mockEventLoopEventHandler(Event event, EventHandlerCallback callback) {
    std::cout << "mockEventLoopEventHandler: got event: " << event.type << std::endl;
    switch (event.type) {
        case MockEventTypes::MockEvent:
            ((MockEventLoopMetadata*)event.data)->eventLoopHandlerReceivedEvent = true;
            auto waitForUnitCallback = [](PublisherEventPtr publisherEvent)
            { 
                std::cout << "mockEventLoopEventHandler: callback: got event: " << publisherEvent->type << std::endl;

                if (publisherEvent->type == MockEventTypes::MockCallbackEvent) {
                    ((MockEventLoopMetadata*)publisherEvent->data)->unitEventHandlerReceivedCallback = true;
                }
            };
            // create a pointer to the callback lambda so we can pass it and call it in the parentUnit. References https://stackoverflow.com/a/41528436/9824103
            void (decltype(waitForUnitCallback)::*ptr)(PublisherEventPtr event)const = &decltype(waitForUnitCallback)::operator();
            
            event.parentUnit->eventHandler(event, waitForUnitCallback );
    }
};

#endif