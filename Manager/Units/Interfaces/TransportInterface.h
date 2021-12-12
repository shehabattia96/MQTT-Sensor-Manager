#ifndef TRANSPORTINTERFACE_H
#define TRANSPORTINTERFACE_H

#include <stdio.h>

#include "./UnitInterface.h"

class TransportInterface : public Unit {

    public:
        TransportInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~TransportInterface() {}
        
        void connect();
        void disconnect();

        void onConnect();
        void onDisconnect();

        void publish(char* topic, void* payload, size_t payloadSize);
        void subscribe(char* topic, void* callback);

        void onReceive(char* topic, void* payload, size_t payloadSize);

    private:
        
};
#endif