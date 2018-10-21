// cstdlib header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// package header files
#include <ti/syslink/Std.h>     /* must be first */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>
#include <ti/ipc/MultiProc.h>

#include "../host/app/log.h"

// local header files
#include "app/app.h"

// private functions
static Int Main_main(Void);

int main()
{
    LOG_INFO("--> main:\n");

    // SysLink initialization
    SysLink_setup();

    // application create, exec, delete
    Main_main();

    // SysLink finalization
    SysLink_destroy();

    return 0;
}


Int Main_main(Void)
{
    UInt16      remoteProcId;
    Int         status = 0;


    LOG_INFO("--> Main_main:\n");

    // invoke the SysLink load callback
    remoteProcId = MultiProc_getId("DSP");
    status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_LOADCALLBACK, NULL);

    if (status < 0)
    {
    	LOG_FATAL("Main_main: load callback failed\n");
        goto leave;
    }

    // invoke the SysLink start callback
    status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_STARTCALLBACK, NULL);

    if (status < 0)
    {
    	LOG_FATAL("Main_main: start callback failed\n");
        goto leave;
    }

    // BEGIN application phase
    // application create phase

    App_create(remoteProcId);

    // application execute phase
    App_exec();

    // application delete phase
    App_delete();


    // invoke the SysLink stop callback
    status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_STOPCALLBACK, NULL);

    if (status < 0)
    {
    	LOG_FATAL("Main_main: stop callback failed\n");
        goto leave;
    }

leave:
    LOG_INFO("<-- Main_main:\n");

    status = (status >= 0 ? 0 : status);
    return (status);
}


