#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;


extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_getJointPosition() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "getJointPosition"},
              {"defaultArg", {
                  {"objectPath", "/PioneerP3DX/rightMotor"},
                  {"relativeObjectPath", ""},
                  {"operationMode", "simx_opmode_blocking"}
              }},
            },
            [](auto& container, auto arg) {
                simxFloat position = 0.0f;
                simxInt handle = getOrUpdateObjectHandle(container, getStringValue(arg["objectPath"], ""));                
                logger::trace("CoppeliaSim_getJointPosition({}, {})", container.clientID, handle);
                simxInt return_code = simxGetJointPosition(container.clientID, handle, &position, 
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_getJointPosition() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{position};
            }
        );
    }

}
