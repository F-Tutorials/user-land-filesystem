#ifndef _DDRIVER_CTL_H_ 
#define _DDRIVER_CTL_H_

#include "include/win_compat.h"   
/******************************************************************************
* SECTION: IO ctl protocol definitions
*******************************************************************************/

struct ddriver_state
{
    int write_cnt;
    int read_cnt;
    int seek_cnt;
};

#define IOC_REQ_DEVICE_SIZE     0
#define IOC_REQ_DEVICE_STATE    1
#define IOC_REQ_DEVICE_RESET    2
#define IOC_REQ_DEVICE_IO_SZ    3
#endif