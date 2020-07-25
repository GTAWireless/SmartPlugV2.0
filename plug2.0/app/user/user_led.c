/*
 * user_gpio.c
 *
 *  Created on: 2018��10��21��
 *      Author: lenovo
 */

#include "user_common.h"
#include "esp_common.h"



#if IS_PHILIPS
    /* wifi״ָ̬ʾ */
    #define LED_GPIO_WIFI_STATUS_MUX         PERIPHS_IO_MUX_MTCK_U
    #define LED_GPIO_WIFI_STATUS_NUM         13
    #define LED_GPIO_WIFI_STATUS_FUNC        FUNC_GPIO13

    #define GPIO_WIFI_STATUS_ON                (0x00)
    #define GPIO_WIFI_STATUS_OFF            (0x01)

    /* �̵������� */
    #define LED_GPIO_RELAY_MUX             PERIPHS_IO_MUX_MTMS_U
    #define LED_GPIO_RELAY_NUM             14
    #define LED_GPIO_RELAY_FUNC            FUNC_GPIO14

    #define GPIO_RELAY_ON                    (0x01)
    #define GPIO_RELAY_OFF                    (0x00)

    /* �̵���״ָ̬ʾ */
    #define LED_GPIO_RELAY_STATUS_MUX         PERIPHS_IO_MUX_MTDI_U
    #define LED_GPIO_RELAY_STATUS_NUM         12
    #define LED_GPIO_RELAY_STATUS_FUNC        FUNC_GPIO12

    #define GPIO_RELAY_STATUS_ON            (0x01)
    #define GPIO_RELAY_STATUS_OFF            (0x00)

    /* �̵����������� */
    #define LED_GPIO_KEY_MUX                 PERIPHS_IO_MUX_GPIO4_U
    #define LED_GPIO_KEY_NUM                 4
    #define LED_GPIO_KEY_FUNC                FUNC_GPIO4

#elif IS_CHANG_XIN

    /* wifi״ָ̬ʾ */
    #define LED_GPIO_WIFI_STATUS_MUX         PERIPHS_IO_MUX_MTCK_U
    #define LED_GPIO_WIFI_STATUS_NUM         13
    #define LED_GPIO_WIFI_STATUS_FUNC        FUNC_GPIO13

    #define GPIO_WIFI_STATUS_ON              (0x01)
    #define GPIO_WIFI_STATUS_OFF             (0x00)


    #define LED_GPIO_RELAY_MUX               PERIPHS_IO_MUX_MTMS_U
    #define LED_GPIO_RELAY_NUM               14
    #define LED_GPIO_RELAY_FUNC              FUNC_GPIO14

    #define GPIO_RELAY_ON                    (0x01)
    #define GPIO_RELAY_OFF                   (0x00)

    /* �̵���״ָ̬ʾ */
    #define LED_GPIO_RELAY_STATUS_MUX         PERIPHS_IO_MUX_MTDI_U
    #define LED_GPIO_RELAY_STATUS_NUM         12
    #define LED_GPIO_RELAY_STATUS_FUNC        FUNC_GPIO12

    #define GPIO_RELAY_STATUS_ON             (0x01)
    #define GPIO_RELAY_STATUS_OFF            (0x00)

    /* �̵����������� */
    #define LED_GPIO_KEY_MUX                 PERIPHS_IO_MUX_GPIO4_U
    #define LED_GPIO_KEY_NUM                 4
    #define LED_GPIO_KEY_FUNC                FUNC_GPIO4
#elif IS_CHANG_XIN_V1

    /* wifi״ָ̬ʾ */
    #define LED_GPIO_WIFI_STATUS_MUX         PERIPHS_IO_MUX_MTCK_U
    #define LED_GPIO_WIFI_STATUS_NUM         13
    #define LED_GPIO_WIFI_STATUS_FUNC        FUNC_GPIO13

    #define GPIO_WIFI_STATUS_ON              (0x01)
    #define GPIO_WIFI_STATUS_OFF             (0x00)


    #define LED_GPIO_RELAY_MUX               PERIPHS_IO_MUX_MTMS_U
    #define LED_GPIO_RELAY_NUM               14
    #define LED_GPIO_RELAY_FUNC              FUNC_GPIO14

    #define GPIO_RELAY_ON                    (0x01)
    #define GPIO_RELAY_OFF                   (0x00)

    /* �̵���״ָ̬ʾ */
    #define LED_GPIO_RELAY_STATUS_MUX         PERIPHS_IO_MUX_MTDI_U
    #define LED_GPIO_RELAY_STATUS_NUM         12
    #define LED_GPIO_RELAY_STATUS_FUNC        FUNC_GPIO12

    #define GPIO_RELAY_STATUS_ON             (0x01)
    #define GPIO_RELAY_STATUS_OFF            (0x00)

    /* �̵����������� */
    #define LED_GPIO_KEY_MUX                 PERIPHS_IO_MUX_GPIO4_U
    #define LED_GPIO_KEY_NUM                 4
    #define LED_GPIO_KEY_FUNC                FUNC_GPIO4

#else

    /* wifi״ָ̬ʾ */
    #define LED_GPIO_WIFI_STATUS_MUX         PERIPHS_IO_MUX_MTCK_U
    #define LED_GPIO_WIFI_STATUS_NUM         13
    #define LED_GPIO_WIFI_STATUS_FUNC        FUNC_GPIO13

    #define GPIO_WIFI_STATUS_ON              (0x01)
    #define GPIO_WIFI_STATUS_OFF             (0x00)

    /* �̵������� */
    #define LED_GPIO_RELAY_MUX               PERIPHS_IO_MUX_MTDO_U
    #define LED_GPIO_RELAY_NUM               15
    #define LED_GPIO_RELAY_FUNC              FUNC_GPIO15

    #define GPIO_RELAY_ON                    (0x01)
    #define GPIO_RELAY_OFF                   (0x00)

    /* �̵���״ָ̬ʾ */
    #define LED_GPIO_RELAY_STATUS_MUX         PERIPHS_IO_MUX_MTDI_U
    #define LED_GPIO_RELAY_STATUS_NUM         12
    #define LED_GPIO_RELAY_STATUS_FUNC        FUNC_GPIO12

    #define GPIO_RELAY_STATUS_ON              (0x01)
    #define GPIO_RELAY_STATUS_OFF             (0x00)

    /* �������� */
    #define LED_GPIO_KEY_MUX                   PERIPHS_IO_MUX_GPIO4_U
    #define LED_GPIO_KEY_NUM                   4
    #define LED_GPIO_KEY_FUNC                  FUNC_GPIO4

#endif



STATIC  os_timer_t stLedLinkTimer;



VOID LED_GpioInit( VOID )
{
    /* wifi״ָ̬ʾ */
    PIN_FUNC_SELECT(LED_GPIO_WIFI_STATUS_MUX, LED_GPIO_WIFI_STATUS_FUNC);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_WIFI_STATUS_NUM), GPIO_WIFI_STATUS_OFF);

    /* �̵������� */
    PIN_FUNC_SELECT(LED_GPIO_RELAY_MUX, LED_GPIO_RELAY_FUNC);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_RELAY_NUM), GPIO_RELAY_OFF);

    /* �̵���״ָ̬ʾ */
    PIN_FUNC_SELECT(LED_GPIO_RELAY_STATUS_MUX, LED_GPIO_RELAY_STATUS_FUNC);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_RELAY_STATUS_NUM), GPIO_RELAY_STATUS_OFF);

    /* �������� */
#if IS_CHANG_XIN
    gpio16_input_conf();
#else
    PIN_FUNC_SELECT(LED_GPIO_KEY_MUX, LED_GPIO_KEY_FUNC);
    GPIO_AS_INPUT(GPIO_ID_PIN(LED_GPIO_KEY_NUM));
#endif
}


VOID LED_WifiStatusOn( VOID )
{
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_WIFI_STATUS_NUM), GPIO_WIFI_STATUS_ON);
}

VOID LED_WifiStatusOff( VOID )
{
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_WIFI_STATUS_NUM), GPIO_WIFI_STATUS_OFF);
}


VOID LED_RelayOn( VOID )
{
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_RELAY_NUM),         GPIO_RELAY_ON);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_RELAY_STATUS_NUM),  GPIO_RELAY_STATUS_ON);
}


VOID LED_RelayOff( VOID )
{
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_RELAY_NUM),         GPIO_RELAY_OFF);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_RELAY_STATUS_NUM),  GPIO_RELAY_STATUS_OFF);
}


UINT32 LED_GetKeyStatus( VOID )
{
#if IS_CHANG_XIN
    return gpio16_input_get();
#else
    return GPIO_INPUT_GET(GPIO_ID_PIN(LED_GPIO_KEY_NUM));
#endif
}

STATIC VOID USER_LedLinkTimerCallbak( VOID *Para)
{
    STATIC  UINT8 ucLedLinkStatus = 0;

    ucLedLinkStatus ^= 0x01;

    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_WIFI_STATUS_NUM), ucLedLinkStatus);
}

STATIC VOID LED_LedLinkTimerInit(UINT32 uiTime )
{
    os_timer_disarm(&stLedLinkTimer);
    os_timer_setfn(&stLedLinkTimer, (os_timer_func_t *)USER_LedLinkTimerCallbak, NULL);
    os_timer_arm(&stLedLinkTimer, uiTime, 1);
}

VOID LED_SetWifiStatus( LED_WIFI_STATUS_E eStatus )
{
    STATIC UINT uiLastStatus;

    if ( uiLastStatus != eStatus)
    {
        os_timer_disarm(&stLedLinkTimer);

        switch ( eStatus )
        {
            /* wifi����ʧ�ܣ����� */
            case LED_WIFI_STATUS_OFF :
                LED_WifiStatusOff();
                break;

            /* wifi�����ӣ����� */
            case LED_WIFI_STATUS_ON :
            /* ͨ��������wifiģʽ����Ϊstationģʽ������  */
            case LED_WIFI_STATUS_SET_STA:
                LED_WifiStatusOn();
                break;

            /* Ѱ��wifi�ȵ�,��˸���2s */
            case LED_WIFI_STATUS_FIND_WIFI :
                LED_LedLinkTimerInit(1000);
                break;

            /* APģʽ�µȴ�ͬ��ʱ��,��˸���2s */
            case LED_WIFI_STATUS_SYNC_TIME :
                LED_LedLinkTimerInit(1000);
                break;

            /* ��������wifi�ȵ�,��˸���1s*/
            case LED_WIFI_STATUS_CONNECTTING :
                LED_LedLinkTimerInit(500);
                break;

            /* ͨ��������wifiģʽ����ΪAPģʽ����˸���0.2s */
            case LED_WIFI_STATUS_SET_AP :
                LED_LedLinkTimerInit(100);
                break;

            default:
                break;
        }

        uiLastStatus = eStatus;
    }
}

