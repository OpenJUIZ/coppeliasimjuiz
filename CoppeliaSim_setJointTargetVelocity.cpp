#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;


extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_setJointTargetVelocity() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "setJointTargetVelocity"},
              {"defaultArg", {
                  {"objectPath", "/PioneerP3DX"},
                  {"targetVelocity", 1.0f},
                  {"operationMode", "simx_opmode_blocking"}
              }},
            },
            [](auto& container, auto arg) {
                logger::trace_object("CoppeliaSim_setJointTargetVelocity()");
                simxFloat position[3] = {0, 0, 0};
                simxInt handle = getOrUpdateObjectHandle(container, getStringValue(arg["objectPath"], ""));
                simxFloat targetVelocity = getDoubleValue(arg["targetVelocity"], 0.0);
                // logger::debug("CoppeliaSim_setJointTargetVelocity({}, {}, {}", container.clientID, handle, targetVelocity);
                simxInt return_code = simxSetJointTargetVelocity(container.clientID, handle, targetVelocity,  
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_getObject() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{0};
            }
        );
    }

}
