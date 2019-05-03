#include "Bebop2CtrlIF.h"



void exportViaCSymbols(int callsign) {
#ifdef __cplusplus
extern "C" {
#endif

    Bebop2CtrlIF* CppPyIF(int callSign) { return new Bebop2CtrlIF(callSign); }
    void CppPyIF_takeoffDrone(Bebop2CtrlIF *Bebop2CtrlIF_IF) { Bebop2CtrlIF_IF->takeoffDrone(); }
    void CppPyIF_killDrone(Bebop2CtrlIF *Bebop2CtrlIF_IF) { Bebop2CtrlIF_IF->killDrone(); }
    void CppPyIF_moveRelativeMetres(Bebop2CtrlIF *PyIF, float x, float y) { 
        PyIF->moveRelativeMetres(x, y); 
    }
    void CppPyIF_landDrone(Bebop2CtrlIF *PyIF) { PyIF->landDrone(); } 
    // need to pass wscDrone::MoveDirection enum here
    // void CppPyIF_moveDirection(Bebop2CtrlIF *PyIF, MoveDirection dir) { PyIF->moveDirection(dir); }
    void CppPyIF_setHeading(Bebop2CtrlIF *PyIF, float heading) { PyIF->setHeading(heading); }
    //Camera commands
    void CppPyIF_capturePhoto(Bebop2CtrlIF *PyIF) { PyIF->capturePhoto(); }
    void CppPyIF_setForward(Bebop2CtrlIF *PyIF) { PyIF->setForward(); }
    void CppPyIF_setTiltPan(Bebop2CtrlIF *PyIF, float tilt, float pan) { PyIF->setTiltPan(tilt, pan); }
    char *CppPyIF_getBuff(Bebop2CtrlIF *PyIF) { return PyIF->getFrameBuffer(); }
    //Other commands
    int CppPyIF_getBatteryLevel(Bebop2CtrlIF *PyIF) { return PyIF->getBatteryLevel(); }

#ifdef __cplusplus
}
#endif
}


int main() {
    //Not neccessary but sorta gets the point across
    exportViaCSymbols(3);

    return 0;
}