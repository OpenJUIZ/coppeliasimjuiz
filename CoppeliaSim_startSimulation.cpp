#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;

#include "remoteApi/extApi.h"

extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_startSimulation() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "startSimulation"},
              {"defaultArg", {
                  {"operationMode", "simx_opmode_blocking"}
              }}
              ,{"description", "Argument operationMode must be ....<br/> simx_opmode_oneshot, simx_opmode_blocking, simx_opmode_streaming + alpha, simx_opmode_oneshot_split + beta (not recommended), simx_opmode_streaming_split + beta (not recommended), simx_opmode_discontinue, simx_opmode_buffer, simx_opmode_remove"}
            },
            [](auto& container, auto arg) {
                auto return_code = simxStartSimulation(
                    container.clientID,
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_startSimulation() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{return_code};
            }
        );
    }

}
