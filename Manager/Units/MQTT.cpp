#include "./MQTT.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "Manager"

MQTT::publish(char* topic, void* payload, size_t payloadSize) {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = payloadSize;
    pubmsg.qos = 0; // Fire and forget
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTResponse rc;
    if ((rc = MQTTClient_publishMessage5(client, topic, &pubmsg, &token)).reasonCode != MQTTREASONCODE_SUCCESS)
    {
         printf("Failed to publish message, return code %d\n", rc.reasonCode);
    }

}

MQTT::disconnect() {
    int rc;
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    	printf("Failed to disconnect, return code %d\n", rc);
    MQTTClient_destroy(&client);
    std::cout << "Disconnected from mqtt." << std::endl;
}

int MQTT::defaultMessageCallback(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    if(subscriptions.count(std::string(topicName)) > 0) {
        subscriptions[topicName](context, topicName, topicLen, message); // call the callback function
    } else {
        printf("Message arrived\n");
        printf("     topic: %s\n", topicName);
        printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    }
    
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

MQTT::onReceive(char* topic, void* payload, size_t payloadSize) {

}
 
MQTT::defaultConnectionLostCallback(void *context, char *cause)
{
    this->onDisconnect()
}

MQTT::onDisconnect() {

    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);

}

// void subscribe(const std::string topic, mqttCallbackFunction callback) {
MQTT::subscribe(char* topic, void* callback) {
    mqttCallbackFunction callbackCast = (mqttCallbackFunction) callback;
    MQTTSubscribe_options subscribeOptions = MQTTSubscribe_options_initializer;
    MQTTProperties subscribeProperties = MQTTProperties_initializer;
    MQTTResponse response;
    if ((response = MQTTClient_subscribe5(client, topic, 0, &subscribeOptions, &subscribeProperties)).reasonCode != MQTTREASONCODE_SUCCESS)
        {
            printf("Failed to subscribe, return code %d\n", response.reasonCode);
            return;
        }
    subscriptions[topic] = callbackCast;
}

MQTT::connect() {
    int rc;
    
    MQTTClient_createOptions options = { {'M', 'Q', 'C', 'O'}, 0, MQTTVERSION_5 };
    if ((rc = MQTTClient_createWithOptions(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL, &options)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc);
        return false;
    }
    
    if ((rc = MQTTClient_setCallbacks(client, NULL, this->defaultConnectionLostCallback, this->defaultMessageCallback, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        return false;
    }

    MQTTProperties	connectProperties = MQTTProperties_initializer;
    MQTTProperties 	willProperties = MQTTProperties_initializer;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer5;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 0;
    
    MQTTResponse response;
    if ((response = MQTTClient_connect5(client, &conn_opts, &connectProperties, &willProperties)).reasonCode != MQTTREASONCODE_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", response.reasonCode);
        return false;
    }

    std::cout << "Connected to mqtt." << std::endl;
    return true;
}