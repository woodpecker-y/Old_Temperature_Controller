#include "sysparams.h"

/* lengthÎª×Ö½ÚÊý */
void eeprom_write_sys(u8 offset, void* sys_params, u8 length)
{
      eeprom_init();
      eeprom_write(offset, (u8*)sys_params, length);
      eeprom_close();
}



SysParams g_sys_params;
RunParams g_run_params;