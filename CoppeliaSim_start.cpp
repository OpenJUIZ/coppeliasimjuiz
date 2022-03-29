#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;

#include "remoteApi/extApi.h"

extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_start() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "start"},
              {"defaultArg", {
                  {"connectionAddress", "127.0.0.1"},
                  {"connectionPort", 19997},
                  {"waitUntilConnected", true},
                  {"doNotRecoonectOnceDisconnected", false},
                  {"timeOutInMs", 10000},
                  {"commThreadCycleInMs", 100}
              }},
            },
            [](auto& container, auto arg) {
                logger::trace_object to("CoppeliaSim_start()");
                if (container.clientID < 0) {
                    container.clientID = simxStart(
                        getStringValue(arg["connectionAddress"], "").c_str(),
                        getIntValue(arg["connectionPort"], 0),
                        getBoolValue(arg["waitUntilConnected"], true),
                        getBoolValue(arg["doNotReconnectOnceDisconnected"], false),
                        getIntValue(arg["timeOutInMs"], 10000),
                        getIntValue(arg["commThreadCycleInMs"], 100));
                    if (container.clientID < 0) {
                        return Value::error(logger::error("CoppeliaSim_start failed. The argument is {}. Connection Refused.", arg));
                    }
                }
                return Value(container.clientID);
            }
        );
    }

}
