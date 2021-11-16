#include <catch2/catch.hpp>
#include "iostream"
#include "../Units/EventLoop.h"

void mockEventHandler(Event event, EventHandlerCallback callback) {
    std::cout << "event: " << event.type << std::endl;

    enum MockEventTypes {MockCallbackEvent};
    const PublisherEvent mockCallbackEvent = {
        MockCallbackEvent,
        0, 
        nullptr
    };
    return callback(&mockCallbackEvent); 
};

TEST_CASE("Test event loop") {
    auto eventLoop = EventLoop(&mockEventHandler);
};