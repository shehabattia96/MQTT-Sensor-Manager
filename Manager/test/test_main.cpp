#include <iostream>

void testEventLoop();

int main() {
    bool pass = false;
    
    try {

        testEventLoop();

        pass = true;

    
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    std::clog << "Tests " << (pass ? "Passed" : "Failed") << std::endl;

    return pass ? 0 : 1;
}