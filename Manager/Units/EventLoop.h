#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "./ApplicationUnits.h"
#include "./Interfaces/EventLoopInterface.h"

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