#ifndef TRANSPORTINTERFACE_H
#define TRANSPORTINTERFACE_H

#include <stdio.h>

#include "./UnitInterface.h"

class TransportInterface : public Unit {

    public:
        TransportInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~TransportInterface() {}
        
        virtual void connect();
        virtual void disconnect();

        virtual void onConnect();
        virtual void onDisconnect();

        virtual void publish(char* topic, void* payload, size_t payloadSize);
        virtual void subscribe(char* topic, void* callback);

        virtual void onReceive(char* topic, void* payload, size_t payloadSize);

    private:
        
};
#endif