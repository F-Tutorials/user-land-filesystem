#ifndef _DDRIVER_CTL_H_ 
#define _DDRIVER_CTL_H_

#include <sys/ioctl.h>   
/******************************************************************************
* SECTION: IO ctl protocol definitions
*******************************************************************************/
#define IOC_MAGIC               'A'

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