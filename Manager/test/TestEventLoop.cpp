#include <thread>
#include <chrono>
#include <stdexcept>

#include "./mock/MockEventLoop.h"


void testEventLoop() {

    auto eventLoop = EventLoop(&mockEventLoopEventHandler);

    // ideally, in the use of this event loop, data wouldn't be passed to be mutated. Passing constant objects is ideal, but we will allow passing anything because this is an easy way to make sure the data we're sending here gets where it needs to.
    MockEventLoopMetadata mockEventLoopMetadata;

    mockEvent.data = &mockEventLoopMetadata; // add metadata to the event we're about to disptach

    eventLoop.enqueue(mockEvent);

    const int timeout = 2000; //ms
    int elapsed = 0;

    bool allEventsDelivered = false;

    while (!allEventsDelivered && elapsed < timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        elapsed += 100;
        
        allEventsDelivered = mockEventLoopMetadata.eventLoopHandlerReceivedEvent && mockEventLoopMetadata.unitEventHandlerReceivedEvent && mockEventLoopMetadata.unitEventHandlerReceivedCallback;
    }

    eventLoop.stop();

    if (!allEventsDelivered) std::throw_with_nested(std::runtime_error("Timeout"));

}