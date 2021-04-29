#include <iostream>
#include <fstream>

#include <chrono>
#include <thread>
#include <opencv2\opencv.hpp>
#include "./mqtt.h"

using std::chrono::operator""ms;

#define MAX_PICTUREQUEUE_SIZE 3
std::queue<std::vector<char>> pictureQueue;

int main() {
    bool mqttConnectStatus = mqtt_connect(); // connect in async thread to not block UI thread.

    subscribe("camFeed", mqttCallbackFunction([](void *context, char *topicName, int topicLen, MQTTClient_message *message) {
        char* pcBuffer = (char*) message->payload;
        size_t pcBufferSize = message->payloadlen;
        std::cout << "got picture, size: " << message->payloadlen << std::endl;
        
        std::vector<char> buffer(pcBuffer, pcBuffer+pcBufferSize);
        if (pictureQueue.size() > MAX_PICTUREQUEUE_SIZE) pictureQueue.pop();
        pictureQueue.push(buffer);
    }));

    while (1) {
        if (pictureQueue.size() > 0) {
           std::vector<char> buffer = pictureQueue.front();
           if (buffer.size()>0) {
            cv::Mat image;
            image = cv::imdecode(buffer, cv::ImreadModes::IMREAD_UNCHANGED);
            if (!image.data)
                {
                    std::cout << "Image not loaded" << std::endl;
                    continue;
                }
            cv::imshow("video", image);
            cv::waitKey(20);
           }
        } else {
            std::cout << "empty buffer" << std::endl;
            std::this_thread::sleep_for(1000ms);
        }
    }

    return 0;
}