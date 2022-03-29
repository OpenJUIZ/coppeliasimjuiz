#include <juiz/juiz.h>
#include <juiz/container.h>

#include "CoppeliaSim.h"

using namespace juiz;


extern "C" {

    JUIZ_OPERATION  void* CoppeliaSim_getStringSignal() {
        return containerOperationFactory<CoppeliaSim>(
            {
              {"typeName", "getStringSignal"},
              {"defaultArg", {
                  {"signalName", "measuredData"},
                  {"operationMode", "simx_opmode_blocking"},
                  {"output", "floats"}
              }},
            },
            [](auto& container, auto arg) -> juiz::Value {
                logger::trace_object to("CoppeliaSim_getStringSignal()");
                simxUChar* signalValue;
                simxInt signalLength = 0;
                auto signalName = getStringValue(arg["signalName"], "");
                simxInt return_code = simxGetStringSignal(container.clientID, signalName.c_str(), &signalValue, &signalLength, 
                              coppelia_string2opmode(getStringValue(arg["operationMode"], "simx_opmode_blocking").c_str()));
                if (return_code != simx_return_ok) {
                    return Value::error(logger::error("CoppeliaSim_getObjectQuaternion() failed. ERROR: {}='{}'", return_code, retcode2help(return_code)));
                }
                if (arg["output"].stringValue() == "floats") {
                    Value v = Value::list();
                    logger::verbose("value:");
                    for (int i=0;i<signalLength/4;i++) {
                        v.push_back({((float*)signalValue)[i]});
                        logger::verbose(" - {}", ((float*)signalValue)[i]);
                    }
                    logger::verbose("----");
                    return v;
                } else if (arg["output"].stringValue() == "int32s") {
                    Value v = Value::list();
                    
                    std::vector<int64_t> vs;
                    for (int i=0;i<signalLength/4;i++)
                        v.push_back({((int32_t*)signalValue)[i]});
                    
                    return v;
                } 
                return Value::error(logger::error("CoppeliaSim_getStringSIgnal failed. Invalid output style setting."));
            }
        );
    }

}
