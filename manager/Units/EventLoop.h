#include <thread>
#include <mutex>
#include <queue>

#include "./EventLoopInterface.h"


template <typename Dispatch, typename... Args>
class ThreadInterface {
    public:
        ThreadInterface(Dispatch function, Args... args){};
        virtual ~ThreadInterface() {};

        // virtual void join() = 0;
        // virtual void joinable() = 0;
};


template <typename Dispatch, typename... Args>
class StdThreadProvider : public ThreadInterface<Dispatch, Args...> {
    public:
        StdThreadProvider(Dispatch function, Args... args) : ThreadInterface(function, std::forward<Args>(args)...) {
            this->_thread = std::thread(function, std::forward<Args>(args)...);
        }; 

        ~StdThreadProvider() {
            
            if (_thread.joinable()) {
                _thread.join();
            }

        };
    private:
        std::thread _thread;

};

void test(EventHandler eventHandler) {};

class EventLoop : public EventLoopInterface {

    public:
        EventLoop(EventHandler eventHandler) : EventLoopInterface(eventHandler) {

            this->_thread = new StdThreadProvider<void (*)(EventHandler), EventHandler>(&test, eventHandler);

        }

        ~EventLoop(){
            delete this->_thread;
        }

        void enqueue(Event event) {
            this->_eventLoopQueue.push(event);
        }
        
        void setThreadProvider(ThreadInterface<void (*)(EventHandler), EventHandler>* threadProvider) {

            this->_thread = threadProvider;

        }

    private:
        // std::mutex _mutex;
        ThreadInterface<void (*)(EventHandler), EventHandler>* _thread;
        std::queue<Event> _eventLoopQueue;


        void eventLoop(EventHandler eventHandler) {
            for (;;) {
                
                if ( _eventLoopQueue.empty() ) continue;

                Event event = _eventLoopQueue.front();
                _eventLoopQueue.pop();

                eventHandler(event, nullptr);
            }
        }

};