#ifndef MQTT_h
#define MQTT_h
#include <map>
#include <string>
#include <iostream>

#include "MQTTClient.h" // references vcpkg paho-mqtt library

#include "./Interfaces/TransportInterface.h"

// mqttCallbackFunction will be the required callback template for mqtt subscription callback functions.
typedef void (*mqttCallbackFunction)(void *context, char *topicName, int topicLen, MQTTClient_message *message);

class MQTT : public TransportInterface
{

public:
    MQTT(EventHandler eventHandler) : TransportInterface(eventHandler){};

    ~MQTT(){

    };

    void connect(struct TransportConnectionData *connectionData)
    {
        int rc;
        bool connectFailed = false;

        MQTTClient_createOptions options = {{'M', 'Q', 'C', 'O'}, 0, MQTTVERSION_5};
        if ((rc = MQTTClient_createWithOptions(&client, connectionData->host, connectionData->clientName,
                                               MQTTCLIENT_PERSISTENCE_NONE, NULL, &options)) != MQTTCLIENT_SUCCESS)
        {
            printf("Failed to create client, return code %d\n", rc);
            return this->onConnectFailed();
        }

        if ((rc = MQTTClient_setCallbacks(client, this, &MQTT::defaultConnectionLostCallback, MQTT::defaultMessageCallback, NULL)) != MQTTCLIENT_SUCCESS)
        {
            printf("Failed to set callbacks, return code %d\n", rc);
            return this->onConnectFailed();
        }

        MQTTProperties connectProperties = MQTTProperties_initializer;
        MQTTProperties willProperties = MQTTProperties_initializer;
        MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer5;
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 0;

        MQTTResponse response;
        if ((response = MQTTClient_connect5(client, &conn_opts, &connectProperties, &willProperties)).reasonCode != MQTTREASONCODE_SUCCESS)
        {
            printf("Failed to connect, return code %d\n", response.reasonCode);
            
            return this->onConnectFailed();
        }

        std::cout << "Connected to mqtt." << std::endl;
        this->onConnect();
        return;
    };

    void disconnect()
    {
        int rc;
        if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
            printf("Failed to disconnect, return code %d\n", rc);
        MQTTClient_destroy(&client);
        std::cout << "Disconnected from mqtt." << std::endl;
    };

    void onConnectFailed() {
        
            SubscriptionEvent connectEvent = {
                TransportEvents::CONNECT_FAIL,
                0, 
                this,
                nullptr
            };

            this->eventHandler(connectEvent, nullptr);

    }

    void onConnect(){
        
        SubscriptionEvent connectEvent = {
            TransportEvents::CONNECTED,
            0, 
            this,
            nullptr
        };

        this->eventHandler(connectEvent, nullptr);

    };
    void onDisconnect()
    {

        printf("\nConnection lost\n");
        
        SubscriptionEvent subscriptionEvent = {
            TransportEvents::DISCONNECTED,
            0, 
            this,
            nullptr
        };

        this->eventHandler(subscriptionEvent, nullptr);
    };

    void publish(char *topic, void *payload, size_t payloadSize)
    {

        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = payload;
        pubmsg.payloadlen = (int)payloadSize;
        pubmsg.qos = 0; // Fire and forget
        pubmsg.retained = 0;
        MQTTClient_deliveryToken token;
        MQTTResponse rc;
        if ((rc = MQTTClient_publishMessage5(client, topic, &pubmsg, &token)).reasonCode != MQTTREASONCODE_SUCCESS)
        {
            printf("Failed to publish message, return code %d\n", rc.reasonCode);
        }
    };

    void subscribe(char *topic)
    {
        MQTTSubscribe_options subscribeOptions = MQTTSubscribe_options_initializer;
        MQTTProperties subscribeProperties = MQTTProperties_initializer;
        MQTTResponse response;
        if ((response = MQTTClient_subscribe5(client, topic, 0, &subscribeOptions, &subscribeProperties)).reasonCode != MQTTREASONCODE_SUCCESS)
        {
            printf("Failed to subscribe, return code %d\n", response.reasonCode);
            return;
        }
    };

    void onReceive(char *topic, void *payload, size_t payloadSize){};

private:
    MQTTClient client;

    static void defaultConnectionLostCallback(void *context, char *cause)
    {
        MQTT *mqttContext = (MQTT *)context;
        mqttContext->onDisconnect();
    };

    static int defaultMessageCallback(void *context, char *topicName, int topicLen, MQTTClient_message *message)
    {
        MQTT *mqttContext = (MQTT *)context;

        TransportData data {
            message->msgid, topicName, topicLen, message->payload, message->payloadlen
        };
        
        SubscriptionEvent subscriptionEvent = {
            TransportEvents::MESSAGE_RECEIVED,
            0, 
            mqttContext,
            &data
        };
        mqttContext->eventHandler(subscriptionEvent, nullptr);

        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    };
};

#endif
