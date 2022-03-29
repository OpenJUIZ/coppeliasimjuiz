#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;


extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_getObjectOrientation() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "getObjectOrientation"},
              {"defaultArg", {
                  {"objectPath", "/PioneerP3DX"},
                  {"relativeObjectPath", ""},
                  {"operationMode", "simx_opmode_blocking"}
              }},
            },
            [](auto& container, auto arg) {
                logger::trace("CoppeliaSim_getObjectOrientation()");
                simxFloat orientation[3] = {0, 0, 0};
                simxInt handle = getOrUpdateObjectHandle(container, getStringValue(arg["objectPath"], ""));
                simxInt relativeObjectHandle = getStringValue(arg["relativeObjectPath"], "").length() == 0 ? -1 : getOrUpdateObjectHandle(container, getStringValue(arg["relativeObjectPath"], ""));
                simxInt return_code = simxGetObjectOrientation(container.clientID, handle, relativeObjectHandle, orientation, 
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_getObjectOrientation() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{
                    {"r", orientation[0]},
                    {"y", orientation[1]},
                    {"p", orientation[2]}
                };
            }
        );
    }

}
