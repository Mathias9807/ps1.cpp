#include "kernel_functions.h"


kernel_func_t kernel_a_funcs[] = {
	{ "open", KERNEL_TYPE_STRING, "filename", KERNEL_TYPE_INT, "accessmode" },
	{ "lseek", KERNEL_TYPE_NONE, "fd", KERNEL_TYPE_NONE, "offset", KERNEL_TYPE_NONE, "seektype" },
	{ "read", KERNEL_TYPE_NONE, "fd", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "length" },
	{ "write", KERNEL_TYPE_NONE, "fd", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "length" },
	{ "close", KERNEL_TYPE_NONE, "fd" },
	{ "ioctl", KERNEL_TYPE_NONE, "fd", KERNEL_TYPE_NONE, "cmd", KERNEL_TYPE_NONE, "arg" },
	{ "exit", KERNEL_TYPE_NONE, "exitcode" },
	{ "isatty", KERNEL_TYPE_NONE, "fd" },
	{ "getc", KERNEL_TYPE_NONE, "fd" },
	{ "putc", KERNEL_TYPE_NONE, "char", KERNEL_TYPE_NONE, "fd" },

	{ "todigit", KERNEL_TYPE_NONE, "char" },
	{ "atof", KERNEL_TYPE_NONE, "src" },     // Does NOT work - uses (ABSENT) cop1 !!!
	{ "strtoul", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "src_end", KERNEL_TYPE_NONE, "base" },
	{ "strtol", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "src_end", KERNEL_TYPE_NONE, "base" },
	{ "abs", KERNEL_TYPE_NONE, "val" },
	{ "labs", KERNEL_TYPE_NONE, "val" },
	{ "atoi", KERNEL_TYPE_NONE, "src" },
	{ "atol", KERNEL_TYPE_NONE, "src" },
	{ "atob", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "num_dst" },
	{ "setjmp", KERNEL_TYPE_NONE, "buf" },
	{ "longjmp", KERNEL_TYPE_NONE, "buf", KERNEL_TYPE_NONE, "param" },
	{ "strcat", KERNEL_TYPE_STRING, "dst", KERNEL_TYPE_STRING, "src" },
	{ "strncat", KERNEL_TYPE_STRING, "dst", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_INT, "maxlen" },
	{ "strcmp", KERNEL_TYPE_STRING, "str1", KERNEL_TYPE_STRING, "str2" },
	{ "strncmp", KERNEL_TYPE_STRING, "str1", KERNEL_TYPE_STRING, "str2", KERNEL_TYPE_INT, "maxlen" },
	{ "strcpy", KERNEL_TYPE_STRING, "dst", KERNEL_TYPE_STRING, "src" },
	{ "strncpy", KERNEL_TYPE_STRING, "dst", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_INT, "maxlen" },
	{ "strlen", KERNEL_TYPE_STRING, "src" },
	{ "index", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_CHAR, "char" },
	{ "rindex", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_CHAR, "char" },
	{ "strchr", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_CHAR, "char" },  // exactly the same as "index"
	{ "strrchr", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_CHAR, "char" }, // exactly the same as "rindex"
	{ "strpbrk", KERNEL_TYPE_STRING, "src", KERNEL_TYPE_NONE, "list" },
	{ "strspn", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "list" },
	{ "strcspn", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "list" },
	{ "strtok", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "list" },  // use strtok(0,list) in further calls
	{ "strstr", KERNEL_TYPE_STRING, "str", KERNEL_TYPE_STRING, "substr" },       // Bugged
	{ "toupper", KERNEL_TYPE_CHAR, "char" },
	{ "tolower", KERNEL_TYPE_CHAR, "char" },
	{ "bcopy", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "len" },
	{ "bzero", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "len" },
	{ "bcmp", KERNEL_TYPE_NONE, "ptr1", KERNEL_TYPE_NONE, "ptr2", KERNEL_TYPE_NONE, "len" },      // Bugged
	{ "memcpy", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "len" },
	{ "memset", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "fillbyte", KERNEL_TYPE_NONE, "len" },
	{ "memmove", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "len" },     // Bugged
	{ "memcmp", KERNEL_TYPE_NONE, "src1", KERNEL_TYPE_NONE, "src2", KERNEL_TYPE_NONE, "len" },    // Bugged
	{ "memchr", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "scanbyte", KERNEL_TYPE_NONE, "len" },
	{ "rand" },
	{ "srand", KERNEL_TYPE_NONE, "seed" },
	{ "qsort", KERNEL_TYPE_NONE, "base", KERNEL_TYPE_NONE, "nel", KERNEL_TYPE_NONE, "width", KERNEL_TYPE_NONE, "callback" },
	{ "strtod", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "src_end" }, // Does NOT work - uses (ABSENT) cop1 !!!
	{ "malloc", KERNEL_TYPE_NONE, "size" },
	{ "free", KERNEL_TYPE_NONE, "buf" },
	{ "lsearch", KERNEL_TYPE_NONE, "key", KERNEL_TYPE_NONE, "base", KERNEL_TYPE_NONE, "nel", KERNEL_TYPE_NONE, "width", KERNEL_TYPE_NONE, "callback" },
	{ "bsearch", KERNEL_TYPE_NONE, "key", KERNEL_TYPE_NONE, "base", KERNEL_TYPE_NONE, "nel", KERNEL_TYPE_NONE, "width", KERNEL_TYPE_NONE, "callback" },
	{ "calloc", KERNEL_TYPE_NONE, "sizx", KERNEL_TYPE_NONE, "sizy" },            // SLOW!
	{ "realloc", KERNEL_TYPE_NONE, "old_buf", KERNEL_TYPE_NONE, "new_siz" },     // SLOW!
	{ "InitHeap", KERNEL_TYPE_NONE, "addr", KERNEL_TYPE_NONE, "size" },
	{ "_exit", KERNEL_TYPE_NONE, "exitcode" },
	{ "getchar" },
	{ "putchar", KERNEL_TYPE_NONE, "char" },
	{ "gets", KERNEL_TYPE_NONE, "dst" },
	{ "puts", KERNEL_TYPE_NONE, "src" },
	{ "printf", KERNEL_TYPE_NONE, "txt", KERNEL_TYPE_NONE, "param1", KERNEL_TYPE_NONE, "param2", KERNEL_TYPE_NONE, "etc" },
	{ "SystemErrorUnresolvedException" },
	{ "LoadTest", KERNEL_TYPE_NONE, "filename", KERNEL_TYPE_NONE, "headerbuf" },
	{ "Load", KERNEL_TYPE_NONE, "filename", KERNEL_TYPE_NONE, "headerbuf" },
	{ "Exec", KERNEL_TYPE_NONE, "headerbuf", KERNEL_TYPE_NONE, "param1", KERNEL_TYPE_NONE, "param2" },
	{ "FlushCache" },
	{ "init_a0_b0_c0_vectors" },
	{ "GPU_dw", KERNEL_TYPE_NONE, "Xdst", KERNEL_TYPE_NONE, "Ydst", KERNEL_TYPE_NONE, "Xsiz", KERNEL_TYPE_NONE, "Ysiz", KERNEL_TYPE_NONE, "src" },
	{ "gpu_send_dma", KERNEL_TYPE_NONE, "Xdst", KERNEL_TYPE_NONE, "Ydst", KERNEL_TYPE_NONE, "Xsiz", KERNEL_TYPE_NONE, "Ysiz", KERNEL_TYPE_NONE, "src" },
	{ "SendGP1Command", KERNEL_TYPE_NONE, "gp1cmd" },
	{ "GPU_cw", KERNEL_TYPE_NONE, "gp0cmd" },   // send GP0 command word
	{ "GPU_cwp", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "num" }, // send GP0 command word and parameter words
	{ "send_gpu_linked_list", KERNEL_TYPE_NONE, "src" },
	{ "gpu_abort_dma" },
	{ "GetGPUStatus" },
	{ "gpu_sync" },
	{ "SystemError" },
	{ "SystemError" },
	{ "LoadExec", KERNEL_TYPE_NONE, "filename", KERNEL_TYPE_NONE, "stackbase", KERNEL_TYPE_NONE, "stackoffset" },
	{ "GetSysSp" },
	{ "SystemError" },
	{ "_96_init" },
	{ "_bu_init" },
	{ "_96_remove" },
	{ "return" },
	{ "return" },
	{ "return" },
	{ "return" },
	{ "dev_tty_init" },
	{ "dev_tty_open", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname", KERNEL_TYPE_NONE, "accessmode" }, // PS2: SystemError
	{ "dev_tty_in_out", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "cmd" },                             // PS2: SystemError
	{ "dev_tty_ioctl", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "cmd", KERNEL_TYPE_NONE, "arg" },                          // PS2: SystemError
	{ "dev_cd_open", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname", KERNEL_TYPE_NONE, "accessmode" },
	{ "dev_cd_read", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "len" },
	{ "dev_cd_close", KERNEL_TYPE_NONE, "fcb" },
	{ "dev_cd_firstfile", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname", KERNEL_TYPE_NONE, "direntry" },
	{ "dev_cd_nextfile", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "direntry" },
	{ "dev_cd_chdir", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "path" },
	{ "dev_card_open", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname", KERNEL_TYPE_NONE, "accessmode" },
	{ "dev_card_read", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "len" },
	{ "dev_card_write", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_NONE, "len" },
	{ "dev_card_close", KERNEL_TYPE_NONE, "fcb" },
	{ "dev_card_firstfile", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname", KERNEL_TYPE_NONE, "direntry" },
	{ "dev_card_nextfile", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "direntry" },
	{ "dev_card_erase", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname" },
	{ "dev_card_undelete", KERNEL_TYPE_NONE, "fcb", KERNEL_TYPE_NONE, "pathname" },
	{ "dev_card_format", KERNEL_TYPE_NONE, "fcb" },
	{ "dev_card_rename" },
	{ },
	{ "_bu_init" },
	{ "_96_init" },
	{ "_96_remove" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "CdAsyncSeekL", KERNEL_TYPE_NONE, "src" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "CdAsyncGetStatus", KERNEL_TYPE_NONE, "dst" },
	{ "no_func" },
	{ "CdAsyncReadSector", KERNEL_TYPE_NONE, "count", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_NONE, "mode" },
	{ "no_func" },
	{ "no_func" },
	{ "CdAsyncSetMode", KERNEL_TYPE_NONE, "mode" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "no_func" },
	{ "CdromIoIrqFunc1" },
	{ "CdromDmaIrqFunc1" },
	{ "CdromIoIrqFunc2" },
	{ "CdromDmaIrqFunc2" },
	{ "CdromGetInt5errCode", KERNEL_TYPE_NONE, "dst1", KERNEL_TYPE_NONE, "dst2" },
	{ "CdInitSubFunc" },
	{ "AddCDROMDevice" },
	{ "AddMemCardDevice" },
	{ "AddDuartTtyDevice" },
	{ "add_nullcon_driver" },
	{ "SystemError" },
	{ "SystemError" },
	{ "SetConf", KERNEL_TYPE_NONE, "num_EvCB", KERNEL_TYPE_NONE, "num_TCB", KERNEL_TYPE_NONE, "stacktop" },
	{ "GetConf", KERNEL_TYPE_NONE, "num_EvCB_dst", KERNEL_TYPE_NONE, "num_TCB_dst", KERNEL_TYPE_NONE, "stacktop_dst" },
	{ "SetCdromIrqAutoAbort", KERNEL_TYPE_NONE, "type", KERNEL_TYPE_NONE, "flag" },
	{ "SetMem", KERNEL_TYPE_NONE, "megabytes" },
};

kernel_func_t kernel_b_funcs[] = {
	{ "alloc_kernel_memory", KERNEL_TYPE_NONE, "size" },
	{ "free_kernel_memory", KERNEL_TYPE_NONE, "buf" },
	{ "init_timer", KERNEL_TYPE_NONE, "t", KERNEL_TYPE_NONE, "reload", KERNEL_TYPE_NONE, "flags" },
	{ "get_timer", KERNEL_TYPE_NONE, "t" },
	{ "enable_timer_irq", KERNEL_TYPE_NONE, "t" },
	{ "disable_timer_irq", KERNEL_TYPE_NONE, "t" },
	{ "restart_timer", KERNEL_TYPE_NONE, "t" },
	{ "DeliverEvent", KERNEL_TYPE_NONE, "class", KERNEL_TYPE_NONE, "spec" },
	{ "OpenEvent", KERNEL_TYPE_NONE, "class", KERNEL_TYPE_NONE, "spec", KERNEL_TYPE_NONE, "mode", KERNEL_TYPE_NONE, "func" },
	{ "CloseEvent", KERNEL_TYPE_NONE, "event" },
	{ "WaitEvent", KERNEL_TYPE_NONE, "event" },
	{ "TestEvent", KERNEL_TYPE_NONE, "event" },
	{ "EnableEvent", KERNEL_TYPE_NONE, "event" },
	{ "DisableEvent", KERNEL_TYPE_NONE, "event" },
	{ "OpenTh", KERNEL_TYPE_NONE, "reg_PC", KERNEL_TYPE_NONE, "reg_SP_FP", KERNEL_TYPE_NONE, "reg_GP" },
	{ "CloseTh", KERNEL_TYPE_NONE, "handle" },
	{ "ChangeTh", KERNEL_TYPE_NONE, "handle" },
	{ "jump_to_00000000h" },
	{ "InitPAD2", KERNEL_TYPE_NONE, "buf1", KERNEL_TYPE_NONE, "siz1", KERNEL_TYPE_NONE, "buf2", KERNEL_TYPE_NONE, "siz2" },
	{ "StartPAD2" },
	{ "StopPAD2" },
	{ "PAD_init2", KERNEL_TYPE_NONE, "type", KERNEL_TYPE_NONE, "button_dest", KERNEL_TYPE_NONE, "unused", KERNEL_TYPE_NONE, "unused" },
	{ "PAD_dr" },
	{ "ReturnFromException" },
	{ "ResetEntryInt" },
	{ "HookEntryInt", KERNEL_TYPE_NONE, "addr" },
	{ "SystemError" },
	{ "SystemError" },
	{ "SystemError" },
	{ "SystemError" },
	{ "SystemError" },
	{ "SystemError" },
	{ "UnDeliverEvent", KERNEL_TYPE_NONE, "class", KERNEL_TYPE_NONE, "spec" },
	{ "SystemError" },
	{ "SystemError" },
	{ "SystemError" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "SystemError" },
	{ "SystemError" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "jump_to_00000000h" },
	{ "open", KERNEL_TYPE_STRING, "filename", KERNEL_TYPE_INT, "accessmode" },
	{ "lseek", KERNEL_TYPE_INT, "fd", KERNEL_TYPE_INT, "offset", KERNEL_TYPE_NONE, "seektype" },
	{ "read", KERNEL_TYPE_INT, "fd", KERNEL_TYPE_NONE, "dst", KERNEL_TYPE_INT, "length" },
	{ "write", KERNEL_TYPE_INT, "fd", KERNEL_TYPE_NONE, "src", KERNEL_TYPE_INT, "length" },
	{ "close", KERNEL_TYPE_INT, "fd" },
	{ "ioctl", KERNEL_TYPE_INT, "fd", KERNEL_TYPE_NONE, "cmd", KERNEL_TYPE_NONE, "arg" },
	{ "exit", KERNEL_TYPE_INT, "exitcode" },
	{ "isatty", KERNEL_TYPE_INT, "fd" },
	{ "getc", KERNEL_TYPE_INT, "fd" },
	{ "putc", KERNEL_TYPE_CHAR, "char", KERNEL_TYPE_INT, "fd" },
	{ "getchar" },
	{ "putchar", KERNEL_TYPE_CHAR, "char" },
	{ "gets", KERNEL_TYPE_NONE, "dst" },
	{ "puts", KERNEL_TYPE_STRING, "src" },
	{ "cd", KERNEL_TYPE_STRING, "name" },
	{ "format", KERNEL_TYPE_NONE, "devicename" },
	{ "firstfile2", KERNEL_TYPE_NONE, "filename", KERNEL_TYPE_NONE, "direntry" },
	{ "nextfile", KERNEL_TYPE_NONE, "direntry" },
	{ "rename", KERNEL_TYPE_NONE, "old_filename", KERNEL_TYPE_NONE, "new_filename" },
	{ "erase", KERNEL_TYPE_NONE, "filename" },
	{ "undelete", KERNEL_TYPE_NONE, "filename" },
	{ "AddDrv", KERNEL_TYPE_NONE, "device_info" },  // subfunction for AddXxxDevice functions
	{ "DelDrv", KERNEL_TYPE_NONE, "device_name_lowercase" },
	{ "PrintInstalledDevices" },
};

kernel_func_t kernel_c_funcs[] = {
	{ "EnqueueTimerAndVblankIrqs", KERNEL_TYPE_NONE, "priority" }, // used with prio=1
	{ "EnqueueSyscallHandler", KERNEL_TYPE_NONE, "priority" },     // used with prio=0
	{ "SysEnqIntRP", KERNEL_TYPE_NONE, "priority", KERNEL_TYPE_NONE, "struc" },  // bugged, use with care
	{ "SysDeqIntRP", KERNEL_TYPE_NONE, "priority", KERNEL_TYPE_NONE, "struc" },  // bugged, use with care
	{ "get_free_EvCB_slot" },
	{ "get_free_TCB_slot" },
	{ "ExceptionHandler" },
	{ "InstallExceptionHandlers" },  // destroys/uses k0/k1
	{ "SysInitMemory", KERNEL_TYPE_NONE, "addr", KERNEL_TYPE_NONE, "size" },
	{ "SysInitKernelVariables" },
	{ "ChangeClearRCnt", KERNEL_TYPE_NONE, "t", KERNEL_TYPE_NONE, "flag" },
	{ "SystemError" },   // PS2: return 0
	{ "InitDefInt", KERNEL_TYPE_NONE, "priority" }, // used with prio=3
	{ "SetIrqAutoAck", KERNEL_TYPE_NONE, "irq", KERNEL_TYPE_NONE, "flag" },
	{ "no_func" },               // DTL-H2000: dev_sio_init
	{ "no_func" },               // DTL-H2000: dev_sio_open
	{ "no_func" },               // DTL-H2000: dev_sio_in_out
	{ "no_func" },               // DTL-H2000: dev_sio_ioctl
	{ "InstallDevices", KERNEL_TYPE_NONE, "ttyflag" },
	{ "FlushStdInOutPut" },
	{ "no_func" },               // DTL-H2000: SystemError
	{ "_cdevinput", KERNEL_TYPE_NONE, "circ", KERNEL_TYPE_NONE, "char" },
	{ "_cdevscan" },
	{ "_circgetc", KERNEL_TYPE_NONE, "circ" },    // uses r5 as garbage txt for _ioabort
	{ "_circputc", KERNEL_TYPE_NONE, "char", KERNEL_TYPE_NONE, "circ" },
	{ "_ioabort", KERNEL_TYPE_NONE, "txt1", KERNEL_TYPE_NONE, "txt2" },
	{ "set_card_find_mode", KERNEL_TYPE_NONE, "mode" },  // 0=normal, 1=find deleted files
	{ "KernelRedirect", KERNEL_TYPE_NONE, "ttyflag" },   // PS2: ttyflag=1 causes SystemError
	{ "AdjustA0Table" },
	{ "get_card_find_mode" },
	{ "no_func" },
};

