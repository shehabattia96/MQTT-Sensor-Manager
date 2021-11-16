#include "./UnitInterface.h"

class CameraInterface : public Unit {
    CameraInterface(){}
    virtual ~CameraInterface(){}

    enum cameraSettings : uint8_t {

    };

    enum cameraEvents : uint8_t {
        connected,
        disconnected,
        
        frameAvailable
    };
    
    virtual bool setCameraSetting(cameraSettings setting) = 0;

};