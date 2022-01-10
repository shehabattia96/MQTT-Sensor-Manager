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

        MQTTClient_createOptions options = {{'M', 'Q', 'C', 'O'}, 0, MQTTVERSION_5};
        if ((rc = MQTTClient_createWithOptions(&client, connectionData->host, connectionData->id,
                                               MQTTCLIENT_PERSISTENCE_NONE, NULL, &options)) != MQTTCLIENT_SUCCESS)
        {
            printf("Failed to create client, return code %d\n", rc);
            return;
        }

        if ((rc = MQTTClient_setCallbacks(client, this, &MQTT::defaultConnectionLostCallback, MQTT::defaultMessageCallback, NULL)) != MQTTCLIENT_SUCCESS)
        {
            printf("Failed to set callbacks, return code %d\n", rc);
            return;
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
            return;
        }

        std::cout << "Connected to mqtt." << std::endl;
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

    void onConnect(){};
    void onDisconnect()
    {

        printf("\nConnection lost\n");
        // printf("     cause: %s\n", cause);
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

    void subscribe(char *topic, void *callback)
    {
        mqttCallbackFunction callbackCast = (mqttCallbackFunction)callback;
        MQTTSubscribe_options subscribeOptions = MQTTSubscribe_options_initializer;
        MQTTProperties subscribeProperties = MQTTProperties_initializer;
        MQTTResponse response;
        if ((response = MQTTClient_subscribe5(client, topic, 0, &subscribeOptions, &subscribeProperties)).reasonCode != MQTTREASONCODE_SUCCESS)
        {
            printf("Failed to subscribe, return code %d\n", response.reasonCode);
            return;
        }
        subscriptions[topic] = callbackCast;
    };

    void onReceive(char *topic, void *payload, size_t payloadSize){};

private:
    MQTTClient client;

    std::map<std::string, mqttCallbackFunction> subscriptions; // all subscribed topics and their callbacks are stored here.

    static void defaultConnectionLostCallback(void *context, char *cause)
    {
        ((MQTT *)context)->onDisconnect();
    };

    static int defaultMessageCallback(void *context, char *topicName, int topicLen, MQTTClient_message *message)
    {
        MQTT *mqttContext = (MQTT *)context;
        if (mqttContext->subscriptions.count(std::string(topicName)) > 0)
        {
            mqttContext->subscriptions[topicName](context, topicName, topicLen, message); // call the callback function
        }
        else
        {
            printf("Message arrived\n");
            printf("     topic: %s\n", topicName);
            printf("   message: %.*s\n", message->payloadlen, (char *)message->payload);
        }

        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    };
};

#endif
