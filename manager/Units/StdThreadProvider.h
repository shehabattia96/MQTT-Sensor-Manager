#include <thread>
#include <mutex>
#include <queue>

#include "./Interfaces/ThreadInterface.h"

template <typename Dispatch, typename... Args>
class StdThreadProvider : public ThreadInterface {
    public:
        StdThreadProvider(Dispatch handler, Args... args) : ThreadInterface(nullptr) {
            this->_thread = std::thread(handler, std::forward<Args>(args)...);
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