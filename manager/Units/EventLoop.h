#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <thread>
#include <mutex>
#include <queue>

#include "./EventLoopInterface.h"


class ThreadInterface {
    public:
        virtual ~ThreadInterface() {};

        virtual void join() = 0;
        virtual void detach() = 0;
        virtual bool joinable() = 0;
};


template <typename Dispatch, typename... Args>
class StdThreadProvider : public ThreadInterface {
    public:
        StdThreadProvider(Dispatch function, Args... args) {
            this->_thread = std::thread(function, std::forward<Args>(args)...);
        }; 

        ~StdThreadProvider() {};

        void join() {
            this->_thread.join();
        }

        void detach() {
            this->_thread.detach();
        }

        bool joinable() {
            return this->_thread.joinable();
        }
        
    private:
        std::thread _thread;

};

class EventLoop : public EventLoopInterface {

    public:
        EventLoop(EventHandler eventHandler) : EventLoopInterface(eventHandler) {

            this->_thread = new StdThreadProvider<void (EventLoop::*)(EventHandler),
                EventLoop*,
                EventHandler>
                (&EventLoop::eventLoop, this, eventHandler);

        }

        ~EventLoop() {
            this->stop();
        }

        void enqueue(Event event) {
            this->_eventLoopQueue.push(event);
        }
        
        void setThreadProvider(ThreadInterface* threadProvider) {

            this->_thread = threadProvider;

        }

        void stop() {
            
            // we want to avoid trying to join and delete the _thread multiple times.
            if (this->keepRunning == false) return;

            this->keepRunning = false;

            if (this->_thread->joinable()) {
                this->_thread->join();
            }

            delete this->_thread;
        }

    private:
        // std::mutex _mutex;
        ThreadInterface* _thread;
        std::queue<Event> _eventLoopQueue;
        bool keepRunning = true;

        void eventLoop(EventHandler eventHandler) {
            while (this->keepRunning) {
                
                if ( _eventLoopQueue.empty() ) continue;

                Event event = _eventLoopQueue.front();
                _eventLoopQueue.pop();

                eventHandler(event, nullptr);
            }
        }


};

#endif