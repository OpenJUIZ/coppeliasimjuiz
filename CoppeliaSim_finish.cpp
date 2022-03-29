#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;


#include "remoteApi/extApi.h"
extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_finish() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "finish"},
              {"defaultArg", {
                  {}
              }},
            },
            [](auto& container, auto arg) {
                if (container.clientID < 0) {
                    return Value::error(logger::error("CoppeliaSim_start failed. The argument is {}. Connection Refused.", arg));
                }
                simxFinish(container.clientID);
                container.clientID = -1;
                return Value(container.clientID);
            }
        );
    }

}
