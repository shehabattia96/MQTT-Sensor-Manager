#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include "./UnitInterface.h"

class CameraInterface : public Unit {
    public:
        CameraInterface(EventHandler eventHandler) : Unit(eventHandler) {}
        virtual ~CameraInterface(){}

        virtual void powerOn();
        virtual void powerOff();
        virtual void getFrame();

        enum cameraSettings : uint8_t {};

        enum cameraEvents : uint8_t {
            connected,
            disconnected,
            
            frameAvailable
        };
        
        virtual bool setCameraSetting(cameraSettings setting) = 0;
        

};

#endif