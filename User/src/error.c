#include "error.h"
#include "sysparams.h"

void error_set(u16 code)
{
    g_run_params.st |= code;
    //printf("error_setg_run_params.st = %04x\r\n", g_run_params.st);
}

void error_clr(u16 code)
{
    g_run_params.st &= ~code;
    //printf("error_clrg_run_params.st = %04x\r\n", g_run_params.st);
}

void error_set_u8(u8 code)
{
    g_run_params.face_state |= code;
    //printf("error_setg_run_params.st = %04x\r\n", g_run_params.st);
}

void error_clr_u8(u8 code)
{
    g_run_params.face_state &= ~code;
    //printf("error_clrg_run_params.st = %04x\r\n", g_run_params.st);
}