#pragma once
/**
 * @file CoppeliaSim.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date $DATE
 * 
 * @copyright Copyright (c) 2022
 */


#include "juiz/logger.h"
#include "remoteApi/extApi.h"

/**
 * @brief 
 * 
 */
struct CoppeliaSim {
    simxInt clientID;
    std::map<std::string, simxInt> objectHandles;
    /**
     * @brief Construct a new CoppeliaSim object
     * 
     */
    CoppeliaSim(): clientID(-1) {}
};


/*
simx_opmode_oneshot
Non-blocking mode. The command is sent and a previous reply to the same command returned (if available). The function doesn't wait for the actual reply.
simx_opmode_blocking (or simx_opmode_oneshot_wait)
Blocking mode. The command is sent, and the function will wait for the actual reply and return it (if the function doesn't time out). The received command reply will be removed from the inbox buffer (other operation modes will leave their command replies in the inbox buffer)
simx_opmode_streaming + alpha
Non-blocking mode. The command is sent and a previous reply to the same command returned (if available). The command will be continuously executed on the server side. The function doesn't wait for the actual reply. Alpha is a value between 0-65535 representing the delay in ms wanted, before the same command gets executed again on the server side.
simx_opmode_oneshot_split + beta (not recommended)
Non-blocking mode. The command is sent in small chunks, and a previous reply to the same command returned (if available). The server will also send the reply in small chunks. The function doesn't wait for the actual reply. Beta is a value between 100 and 65535 representing the maximum chunk size in bytes to send. Small values won't slow down the communication framework, but it will take more time until the full command has been transferred. With large values, commands are transferred faster, but the communication framework might appear frozen while chunks are being transferred.
simx_opmode_streaming_split + beta (not recommended)
Non-blocking mode. The command is sent in small chunks and a previous reply to the same command returned (if available). The command will be continuously executed on the server side, which will also send the replies in small chunks. The function doesn't wait for the actual reply. Beta is a value between 100 and 65535 representing the maximum chunk size in bytes to send. Small values won't slow down the communication framework, but it will take more time until the full command has been transferred. With large values, commands are transferred faster, but the communication framework might appear frozen while chunks are being transferred.
simx_opmode_discontinue
Non-blocking mode. The command is sent and a previous reply to the same command returned (if available). A same command will be erased from the server side if the command is of streaming or continuous type. The same will happen on the client's input buffer. The function doesn't wait for the actual reply.
simx_opmode_buffer
Non-blocking mode. A previous reply to the same command is returned (if available). The command is not send, nor does the function wait for the actual reply.
simx_opmode_remove
Non-blocking mode. A previous reply to the same command is cleared from the input buffer (if available). The command is not send, nor does the function return any specific values, except for the return code. Can be useful to free some memory on the client side.
*/

static std::map<std::string, simxInt> str2opmode_dic{
    {"simx_opmode_oneshot", simx_opmode_oneshot},
    {"simx_opmode_blocking", simx_opmode_blocking},
    {"simx_opmode_streaming", simx_opmode_streaming},
    {"simx_opmode_oneshot_split", simx_opmode_oneshot_split},
    {"simx_opmode_streaming_split", simx_opmode_streaming_split},
    {"simx_opmode_discontinue", simx_opmode_discontinue},
    {"simx_opmode_buffer", simx_opmode_buffer},
    {"simx_opmode_remove", simx_opmode_remove}
};

inline simxInt coppelia_string2opmode(const std::string& modeStr) {
    return str2opmode_dic[modeStr];
}

static std::map<simxInt, std::string> retcode2str_dic{
    {simx_return_ok, "simx_return_ok"},
    {simx_return_novalue_flag, "simx_return_novalue_flag"},
    {simx_return_timeout_flag, "simx_return_timeout_flag"},
    {simx_return_illegal_opmode_flag, "simx_return_illegal_opmode_flag"},
    {simx_return_remote_error_flag, "simx_return_remote_error_flag"},
    {simx_return_split_progress_flag, "simx_return_split_progress_flag"},
    {simx_return_local_error_flag, "simx_return_local_error_flag"},
    {simx_return_initialize_error_flag, "simx_return_initialize_error_flag"}
};

inline std::string retcode2str(const simxInt retcode) {
    return retcode2str_dic[retcode];
}

static std::map<simxInt, std::string> retcode2help_dic{
    {simx_return_ok, "The function executed fine"},
    {simx_return_novalue_flag, "There is no command reply in the input buffer. This should not always be considered as an error, depending on the selected operation mode"},
    {simx_return_timeout_flag, "The function timed out (probably the network is down or too slow)"},
    {simx_return_illegal_opmode_flag, "The specified operation mode is not supported for the given function"},
    {simx_return_remote_error_flag, "The function caused an error on the server side (e.g. an invalid handle was specified)"},
    {simx_return_split_progress_flag, "The communication thread is still processing previous split command of the same type"},
    {simx_return_local_error_flag, "The function caused an error on the client side"},
    {simx_return_initialize_error_flag, "simxStart was not yet called"}
};

inline std::string retcode2help(const simxInt retcode) {
    return retcode2help_dic[retcode];
}



inline simxInt updateObjectHandle(CoppeliaSim& container, const std::string& objectPath) {

    juiz::logger::trace("CoppeliaSim(clientID={}) updateObjectHandle(objectPath={}) called. updating object handle.", container.clientID, objectPath);
    simxInt handle = -1;

    simxInt return_code = simxGetObjectHandle(container.clientID, objectPath.c_str(), &handle, simx_opmode_blocking);
    if (return_code != simx_return_ok) {
        juiz::logger::error("CoppeliaSim updateObjectHandle() failed. Invalid handle -1. Error({})", retcode2help(return_code));
        return -1;
    }
    return handle;
}


inline simxInt getOrUpdateObjectHandle(CoppeliaSim& container, const std::string& objectPath) {
    if (objectPath.length() <= 0) {
        juiz::logger::error("CoppeliaSim  getOrUpdateObjectHandle failed. Argument object Path is invalid.");
        return -1;
    }
    if (container.objectHandles.find(objectPath) == container.objectHandles.end() || 
        container.objectHandles[objectPath] < 0    
    ) {
        juiz::logger::trace("CoppeliaSim getOrUpdateObjectHandle updating object handle.");
        simxInt handle = updateObjectHandle(container, objectPath);
        container.objectHandles[objectPath] = handle;
    }
    return container.objectHandles[objectPath];
}