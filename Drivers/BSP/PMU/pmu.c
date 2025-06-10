/**
 ****************************************************************************************************
 * @file        pmu.c
 * @version     V1.0
 * @brief       待机模式 驱动代码
 ****************************************************************************************************
 * @attention   Waiken-Smart 慧勤智远
 *
 * 实验平台:    GD32F470VET6小系统板
 *
 ****************************************************************************************************
 */
 
#include "./BSP/PMU/pmu.h"
#include "./BSP/LED/led.h"


/**
 * @brief       低功耗模式下的按键初始化(用于唤醒待机模式)
 * @param       无
 * @retval      无
 */
void pmu_wkup_key_init(void)
{
    rcu_periph_clock_enable(PMU_WKUP_GPIO_CLK);     /* 使能WKUP引脚时钟 */
  
    gpio_mode_set(PMU_WKUP_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, PMU_WKUP_GPIO_PIN);  /* WKUP引脚模式设置,下拉输入 */
}

/**
 * @brief       进入待机模式
 * @param       无
 * @retval      无
 */
void pmu_enter_standby(void)
{
    rcu_periph_clock_enable(RCU_PMU);               /* 使能PMU时钟 */     

    pmu_wakeup_pin_enable();                        /* WKUP引脚唤醒使能 */
  
    pmu_flag_clear(PMU_FLAG_RESET_STANDBY);         /* 复位待机标志 */
  
    pmu_to_standbymode();                           /* 进入待机模式 */
}
















