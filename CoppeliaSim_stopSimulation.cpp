#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;

#include "remoteApi/extApi.h"

extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_stopSimulation() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "stopSimulation"},
              {"defaultArg", {
                  {"operationMode", "simx_opmode_blocking"}
              }},
            },
            [](auto& container, auto arg) {
                auto return_code = simxStopSimulation(
                    container.clientID,
                    coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_stopSimulation() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                return Value{return_code};
            }
        );
    }

}
