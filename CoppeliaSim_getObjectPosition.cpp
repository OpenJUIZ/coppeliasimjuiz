#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;

extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_getObjectPosition() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "getObjectPosition"},
              {"defaultArg", {
                  {"objectPath", "/PioneerP3DX"},
                  {"relativeObjectPath", ""},
                  {"operationMode", "simx_opmode_blocking"}
              }},
            },
            [](auto& container, auto arg) {
                logger::trace_object("CoppeliaSim_getObjectPosition()");
                simxFloat position[3] = {0, 0, 0};
                simxInt handle = getOrUpdateObjectHandle(container, getStringValue(arg["objectPath"], ""));
                simxInt relativeObjectHandle = getStringValue(arg["relativeObjectPath"], "").length() == 0 ? -1 : getOrUpdateObjectHandle(container, getStringValue(arg["relativeObjectPath"], ""));
                //logger::trace("CoppeliaSim_getObjectPosition({}, {}, {}", container.clientID, handle, relativeObjectHandle);
                simxInt return_code = simxGetObjectPosition(container.clientID, handle, relativeObjectHandle, position, 
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_getObject() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{
                    {"x", position[0]},
                    {"y", position[1]},
                    {"z", position[2]}
                };
            }
        );
    }

}
