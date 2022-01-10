#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "./Units/ApplicationUnits.h"

enum ApplicationStates {
    CONNECT_TO_MQTT,
    DISPLAY_CAMERA_FRAMES
};

ApplicationStates currentState = ApplicationStates::CONNECT_TO_MQTT;

enum MqttEventTypes {};

void eventLoopEventHandler(Event event, EventHandlerCallback callback) {
    std::cout << "eventLoopEventHandler: got event: " << event.type << std::endl;

    switch (currentState) {
        case ApplicationStates::CONNECT_TO_MQTT:
            if (event.type)
            break;
        case ApplicationStates::DISPLAY_CAMERA_FRAMES:
            break;
    }
};
void mqttEventHandler(Event event, EventHandlerCallback callback) {
    std::cout << "mqttEventHandler: got event: " << event.type << std::endl;

    switch (currentState) {
        case ApplicationStates::CONNECT_TO_MQTT:
            if (event.type)
            break;
        case ApplicationStates::DISPLAY_CAMERA_FRAMES:
            break;
    }
};

int main() {

    std::cout << "Type exit to quit." << std::endl;

    // initialize our event loop.
    auto eventLoop = EventLoop(&eventLoopEventHandler);

    

    struct TransportConnectionData mqttConnection {"tcp://localhost:1883", "Manager"};
    auto mqtt = MQTT(&mqttEventHandler);
    mqtt.connect(&mqttConnection);

    std::string input;
    
    while (input != "exit") {
        std::getline(std::cin, input);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "Cleaning up and quitting" << std::endl;
    mqtt.disconnect();

    eventLoop.stop();

    return 0;
};