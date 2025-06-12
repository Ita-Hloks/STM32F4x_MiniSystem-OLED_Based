#ifndef __UFLASH_H__
#define __UFLASH_H__

#include "./SYSTEM/sys/sys.h"
#include "../../Drivers/BSP/NORFLASH/norflash.h"

// Flash 状态码定义
typedef enum
{
    FLS_OK = 0,
    FLS_ERR_INIT = 1,
    FLS_ERR_ADDR = 2,
    FLS_ERR_WRITE = 3,
    FLS_ERR_READ = 4,
    FLS_ERR_ERASE = 5,
    FLS_ERR_BUSY = 6
} FLS_Status;

// Flash 操作状态机状态
typedef enum
{
    FLS_STATE_IDLE = 0,
    FLS_STATE_READING,
    FLS_STATE_WRITING,
    FLS_STATE_ERASING,
    FLS_STATE_ERROR
} FLS_State;

FLS_Status fls_init(void);
FLS_Status fls_write(uint32_t addr, uint8_t *data, uint16_t len);
FLS_Status fls_read(uint32_t addr, uint8_t *data, uint16_t len);
FLS_Status fls_erase_sector(uint32_t addr);
FLS_State fls_get_state(void);
FLS_Status fls_check_addr(uint32_t addr, uint16_t len);

#endif /* __UFLASH_H__ */