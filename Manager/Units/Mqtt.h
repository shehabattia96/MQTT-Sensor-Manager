#ifndef MQTT_h
#define MQTT_h
#include "MQTTClient.h"
#include <map>

#include "./Interfaces/TransportInterface.h"

// mqttCallbackFunction will be the required callback template for mqtt subscription callback functions.
typedef std::function<void(void *context, char *topicName, int topicLen, MQTTClient_message *message)> mqttCallbackFunction;

class MQTT : public TransportInterface {

        MQTT(EventHandler eventHandler) : TransportInterface(eventHandler) {
        }

        ~MQTT() {

        }

        void connect();
        void disconnect();

        void publish(char* topic, void* payload, size_t payloadSize);
        void subscribe(char* topic, void* callback);

        void onReceive(char* topic, void* payload, size_t payloadSize);

        private:

            MQTTClient client;

            std::map<std::string, mqttCallbackFunction> subscriptions; // all subscribed topics and their callbacks are stored here.

            int defaultMessageCallback(void *context, char *topicName, int topicLen, MQTTClient_message *message);
            void defaultConnectionLostCallback(void *context, char *cause);

}


#endif
