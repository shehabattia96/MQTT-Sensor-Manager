#ifndef TRANSPORTINTERFACE_H
#define TRANSPORTINTERFACE_H

#include "./UnitInterface.h"

struct TransportConnectionData {
    char* host;
    char* clientName;
    char* connectionType;
};
struct TransportData {
    int id;
    char* topicName;
    int topicLength;
    void* payload;
    int payloadlen;
};
enum TransportEvents {
    CONNECTING, CONNECTED, DISCONNECTED, CONNECT_FAIL, MESSAGE_RECEIVED, MESSAGE_SENT, MESSAGE_SENT_FAIL, SUBSCRIBE_SUCCESS, SUBSCRIBE_FAIL
};

class TransportInterface : public Unit {

    public:
        TransportInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~TransportInterface() {};
        
        virtual void connect(struct TransportConnectionData* connectionData) = 0;
        virtual void disconnect() = 0;

        virtual void onConnect() = 0;
        virtual void onDisconnect() = 0;

        virtual void publish(char* topic, void* payload, size_t payloadSize) = 0;
        virtual void subscribe(char* topic) = 0;

        virtual void onReceive(char* topic, void* payload, size_t payloadSize) = 0;

};
#endif