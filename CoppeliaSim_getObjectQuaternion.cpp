#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;


extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_getObjectQuaternion() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "getObjectQuaternion"},
              {"defaultArg", {
                  {"objectPath", "/PioneerP3DX"},
                  {"relativeObjectPath", ""},
                  {"operationMode", "simx_opmode_blocking"}
              }},
            },
            [](auto& container, auto arg) {
                logger::trace_object("CoppeliaSim_getObjectQuaternion()");
                simxFloat orientation[4] = {0, 0, 0, 1};
                simxInt handle = getOrUpdateObjectHandle(container, getStringValue(arg["objectPath"], ""));
                simxInt relativeObjectHandle = getStringValue(arg["relativeObjectPath"], "").length() == 0 ? -1 : getOrUpdateObjectHandle(container, getStringValue(arg["relativeObjectPath"], ""));
                /// logger::trace("CoppeliaSim_getObjectOrientation({}, {}, {}", container.clientID, handle, relativeObjectHandle);
                simxInt return_code = simxGetObjectQuaternion(container.clientID, handle, relativeObjectHandle, orientation, 
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_getObjectQuaternion() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{
                    {"x", orientation[0]},
                    {"y", orientation[1]},
                    {"z", orientation[2]},
                    {"w", orientation[3]}
                };
            }
        );
    }

}
