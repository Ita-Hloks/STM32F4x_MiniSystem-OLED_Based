/**
 ****************************************************************************************************
 * @file        spi.c
 * @version     V1.0
 * @brief       SPI ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "./BSP/SPI/spi.h"


/**
 * @brief       SPI��ʼ������
 * @note        ����ģʽ,8λ����,��ֹӲ��Ƭѡ
 * @param       ��
 * @retval      ��
 */
void spi1_init(void)
{
	  spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(SPI1_SCK_GPIO_CLK);        /* ʹ��SPI1_SCK IO��ʱ�� */
    rcu_periph_clock_enable(SPI1_MISO_GPIO_CLK);       /* ʹ��SPI1_MISO IO��ʱ�� */
    rcu_periph_clock_enable(SPI1_MOSI_GPIO_CLK);       /* ʹ��SPI1_MOSI IO��ʱ�� */
    rcu_periph_clock_enable(SPI1_SPI_CLK);             /* ʹ��SPI1ʱ�� */  

    /* ����ʹ�õ�SPI1����:
             SPI1_SCK->PB13
             SPI1_MISO->PB14
             SPI1_MOSI->PB15 */

    /* ����SPI1���ŵĸ��ù��� */
    gpio_af_set(SPI1_SCK_GPIO_PORT, SPI1_SCK_GPIO_AF, SPI1_SCK_GPIO_PIN);  
    gpio_af_set(SPI1_MISO_GPIO_PORT, SPI1_MISO_GPIO_AF, SPI1_MISO_GPIO_PIN);
    gpio_af_set(SPI1_MOSI_GPIO_PORT, SPI1_MOSI_GPIO_AF, SPI1_MOSI_GPIO_PIN);

    /* SPI1_SCK����ģʽ���� ����������� */
    gpio_mode_set(SPI1_SCK_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI1_SCK_GPIO_PIN);
    gpio_output_options_set(SPI1_SCK_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI1_SCK_GPIO_PIN);

    /* SPI1_MISO����ģʽ���� ����������� */
    gpio_mode_set(SPI1_MISO_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI1_MISO_GPIO_PIN);
    gpio_output_options_set(SPI1_MISO_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI1_MISO_GPIO_PIN);

    /* SPI1_MOSI����ģʽ���� ����������� */
    gpio_mode_set(SPI1_MOSI_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI1_MOSI_GPIO_PIN);
    gpio_output_options_set(SPI1_MOSI_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI1_MOSI_GPIO_PIN);

    spi_i2s_deinit(SPI1_SPI);                                          /* ��λSPI1 */
    spi_struct_para_init(&spi_init_struct);                            /* ��ʼ��SPI�ṹ�������в���ΪĬ��ֵ */
    
	  spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;   /* ����ģʽ����:����Ϊȫ˫��ģʽ */
    spi_init_struct.device_mode          = SPI_MASTER;                 /* ����Ϊ����ģʽ */
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;         /* ����֡��ʽ����;8λ����֡��ʽ */ 
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;    /* ����״̬��,CLK���ָߵ�ƽ,�ڵڶ���ʱ�������زɼ���һ������ */
    spi_init_struct.nss                  = SPI_NSS_SOFT;               /* NSS���ģʽ,NSS��ƽȡ����SWNSSλ */
    spi_init_struct.prescale             = SPI_PSC_256;                /* Ԥ��Ƶ������:Ĭ��ʹ��256��Ƶ,�ٶ���� */
    spi_init_struct.endian               = SPI_ENDIAN_MSB;             /* ��˻�С��ģʽ����;�ȷ��������Чλ */
    spi_init(SPI1_SPI, &spi_init_struct);                              /* ��ʼ��SPI1 */
	  
    spi_enable(SPI1_SPI);	                                             /* ʹ��SPI1 */	
}

/**
 * @brief       SPI1�ٶ����ú���
 * @note        SPI1ʱ��ѡ������APB1, ��PCLK1, Ϊ60Mhz
 *              SPI�ٶ� = PCLK1 / 2^(speed + 1)
 * @param       speed   : SPIʱ�ӷ�Ƶϵ��
 * @retval      ��
 */
void spi1_set_speed(uint8_t speed)
{
	  speed &= 0X07;                      /* ���Ʒ�Χ */
	  spi_disable(SPI1_SPI);	            /* SPIʧ�� */
	  SPI_CTL0(SPI1_SPI) &= ~(7 << 3);    /* ������ */
	  SPI_CTL0(SPI1_SPI) |= speed << 3;   /* ���÷�Ƶϵ�� */
	  spi_enable(SPI1_SPI);	              /* SPIʹ�� */
}

/**
 * @brief       SPI1��дһ���ֽ�����
 * @param       txdata  : Ҫ���͵�����(1�ֽ�)
 * @retval      ���յ�������(1�ֽ�)
 */
uint8_t spi1_read_write_byte(uint8_t txdata)
{   
	  while(RESET == spi_i2s_flag_get(SPI1_SPI, SPI_FLAG_TBE));    /* �ȴ����ͻ������� */
	
	  spi_i2s_data_transmit(SPI1_SPI, txdata);                     /* ����һ���ֽ� */
	
	  while(RESET == spi_i2s_flag_get(SPI1_SPI, SPI_FLAG_RBNE));   /* �ȴ����ջ������ǿ� */
	
	  return spi_i2s_data_receive(SPI1_SPI);                       /* �����յ������� */
}






