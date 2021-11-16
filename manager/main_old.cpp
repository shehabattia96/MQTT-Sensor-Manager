#include <iostream>
#include <fstream>

#include <chrono>
#include <thread>
#include <mutex>
#include <opencv2\opencv.hpp>
// #include "./mqtt.h"

using std::chrono::operator""ms;

#define MAX_PICTUREQUEUE_SIZE 10
std::queue<std::vector<char>> pictureQueue;
std::queue<cv::Mat> pictureQueueSimulation;

std::mutex pictureQueueSimulationMutex;

std::chrono::steady_clock::time_point camFeedLastReceivedEpoch = std::chrono::steady_clock::now();

int main() {
    bool mqttConnectStatus = mqtt_connect(); // connect in async thread to not block UI thread.

    subscribe("camFeed", mqttCallbackFunction([](void *context, char *topicName, int topicLen, MQTTClient_message *message) {
        char* pcBuffer = (char*) message->payload;
        size_t pcBufferSize = message->payloadlen;

        std::chrono::steady_clock::time_point epoch = std::chrono::steady_clock::now();
        auto difference =  std::chrono::duration_cast<std::chrono::microseconds>(epoch - camFeedLastReceivedEpoch).count();
        camFeedLastReceivedEpoch = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << difference / 1000.0 << "ms, " << 1000000.0 / difference << "fps" << std::endl;
        std::cout << "got picture, size: " << message->payloadlen << std::endl;
        std::vector<char> buffer(pcBuffer, pcBuffer+pcBufferSize);
        if (pictureQueue.size() > MAX_PICTUREQUEUE_SIZE) {
            std::cout << "camFeed: receiving frames faster than they are being used." << std::endl;
            return;
        }
        pictureQueue.push(buffer);
    }));
    subscribe("vcam1", mqttCallbackFunction([](void *context, char *topicName, int topicLen, MQTTClient_message *message) {
        char* pcBuffer = (char*) message->payload;
        size_t pcBufferSize = message->payloadlen;
        std::cout << "got picture, size: " << message->payloadlen << std::endl;

		const uint numberOfPixels = pcBufferSize - 2 * sizeof(uint);

        // unpack width and height from payload
        uint width, height;
        memcpy((char*)&width, &pcBuffer[numberOfPixels], sizeof(uint));
        memcpy((char*)&height, &pcBuffer[numberOfPixels + sizeof(uint)], sizeof(uint));

        // copy payload into cv::Mat
		cv::Mat img(height, width, CV_8UC3);
        memcpy(img.data, pcBuffer, numberOfPixels);
        cv::flip(img, img, 0);
        
        
        pictureQueueSimulationMutex.lock();
        if (pictureQueueSimulation.size() > MAX_PICTUREQUEUE_SIZE) pictureQueueSimulation.pop();
        pictureQueueSimulationMutex.unlock();
        
        pictureQueueSimulation.push(img);
    }));

    bool isDisplayingFrame = false;
    while (1) {
        if (pictureQueue.size() > 0) {

            std::vector<char> buffer = pictureQueue.front();
            pictureQueue.pop();

            if (buffer.size()>0) {

                cv::Mat image;
                image = cv::imdecode(buffer, cv::ImreadModes::IMREAD_UNCHANGED);
                if (!image.data)
                    {
                        std::cout << "Image not loaded" << std::endl;
                        continue;
                    }

                cv::imshow("video", image);

                isDisplayingFrame = true;

                if (cv::waitKey(1) >=0) break;
            }
        } 
        if (pictureQueueSimulation.size() > 0) {
            
            pictureQueueSimulationMutex.lock();
            cv::Mat image = pictureQueueSimulation.front();
            pictureQueueSimulationMutex.unlock();
            
            if (!image.data)
                {
                    std::cout << "Image not loaded" << std::endl;
                    continue;
                }
            cv::imshow("video", image);
                isDisplayingFrame = true;
            if (cv::waitKey(1) >=0) break;
        } 

        if (!isDisplayingFrame) {
            std::cout << "empty buffer" << std::endl;
            std::this_thread::sleep_for(1000ms);
        }
        isDisplayingFrame = false;
    }

    return 0;
}