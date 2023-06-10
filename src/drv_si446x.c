#include <board.h>
#include "drv_si446x.h"
#include "radio.h"

/**
 * @brief receive rx buff
 * @param {uint8_t} *rxbuf
 * @return {uint8_t} size of recevie,max size is 64
 */
uint8_t drv_si446x_rx(uint8_t *rxbuf)
{
    return si4460_RF_RX(rxbuf);
}

/**
 * @brief receive rx buff
 * @param {uint8_t} *rxbuf
 * @return {*} size of recevie
 */
void drv_si446x_tx(uint8_t *txbuf, uint8_t length)
{
    si4460_RF_TX(txbuf,length);
}

int rt_hw_si446x_init(vodi)
{
    si4460_InitRF(868.26);
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_si446x_init);

void rssi_test(void)
{
    rt_kprintf("hello RT-Thread!\n");
}

MSH_CMD_EXPORT(rssi_test , rssi);
