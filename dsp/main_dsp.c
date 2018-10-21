#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/syslink/SysLink.h>
#include <ti/sysbios/hal/Hwi.h>

#include "../dsp/app/message_loop.h"
#include "../dsp/driver/setgpio.h"


Void message_loop_fun(UArg arg0, UArg arg1)
{
    Int             status = 0;
    Error_Block  eb;
    UInt16        proc_id;
    Bool           running = TRUE;
    UInt           loop = 0;

    Log_print0(Diags_ENTRY | Diags_INFO, "--> message_loop_fun:");

    Error_init(&eb);

    while (running)
    {
        do {
            status = Ipc_start();
        } while (status == Ipc_E_NOTREADY);

        if (status < 0)
        {
            Log_error0("message_loop_fun: Ipc_start() failed");
            goto leave;
        }

        proc_id = MultiProc_getId("HOST");

        do {
            loop++;
            status = Ipc_attach(proc_id);

            if ((status == Ipc_E_NOTREADY) && ((loop & 0xFFF) == 0))
            {
                Task_sleep(1);
            }
        } while (status == Ipc_E_NOTREADY);

        if (status < 0)
        {
            Log_error0("message_loop_fun: Ipc_attach() failed");
            goto leave;
        }

        MessageLoop *msg_loop = message_loop_new();
        message_loop_start(msg_loop);
        message_loop_destroy(msg_loop);

        do {
            status = Ipc_detach(proc_id);

            if ((status == Ipc_E_NOTREADY) && ((++loop & 0xFFF) == 0)) {
                Task_sleep(1);
            }
        } while (status == Ipc_E_NOTREADY);

        if (status < 0)
        {
            Log_error0("message_loop_fun: Ipc_detach() failed");
            goto leave;
        }

        Ipc_stop();
    } 

leave:
    Log_print1(Diags_EXIT,"<-- message_loop_fun: %d", (IArg)status);
    return;
}

Int main(Int argc, Char* argv[])
{
    Log_print0(Diags_ENTRY, "--> main:");

    //Init GPIO
    GPIOInit();
    //Turn OFF LED
    switchLED1(0);
    switchLED2(0);


    Error_Block eb;
    Task_Params taskParams;
    Error_init(&eb);

    Task_Params_init(&taskParams);
    taskParams.instance->name = "message_loop_fun";
	taskParams.priority = 15;
    taskParams.arg0 = (UArg)argc;
    taskParams.arg1 = (UArg)argv;
    taskParams.stackSize = 0x1000;
    Task_create(message_loop_fun, &taskParams, &eb);

    if (Error_check(&eb)) 
    	System_abort("main: failed to create application startup thread");

    BIOS_start();

    Log_print0(Diags_EXIT, "<-- main:");
    return (0);
}
