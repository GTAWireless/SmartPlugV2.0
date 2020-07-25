/*
 * user_infrare.c
 *
 *  Created on: 2018��11��17��
 *      Author: lenovo
 */

#include "esp_common.h"
#include "user_common.h"

#define INFRA_GPIO_NUM            GPIO_Pin_5

UINT32 uiInfraredValue = 0;

INFRARED_SET_S g_stinfrared_Set = { FALSE, FALSE, 0};
INFRARED_VALUE_S g_astInfrared_Value[INFRARED_MAX];

VOID INFRA_InfrareHandle(VOID)
{
    static UINT32 infraredValue = 0;    /* ����ĺ���ֵ */
    static UINT32 uiCurTime = 0;
    static UINT32 uiLastTime = 0;
    static UINT8  Step = 0;
    static UINT uiPluse = 0;
    static uint8_t  BitCount = 0;       /* BitCount����32ʱ������� */

    //GPIO_INPUT_GET( GPIO_ID_PIN(INFRA_GPIO_NUM) );
    //_xt_isr_mask(1 << ETS_GPIO_INUM);

    uiCurTime = system_get_time();

    /* ����2���½�������ʱ�� */
    if ( uiCurTime > uiLastTime )
    {
        uiPluse = uiCurTime - uiLastTime;
    }
    else
    {
        uiPluse = 0xFFFFFFFF + uiCurTime - uiLastTime;
    }

    uiLastTime = uiCurTime;

    switch ( Step )
    {
        case 0 :
            /* ������ߵ�ƽ9ms���͵�ƽ4.5ms  �ж��Ƿ������ɹ� */
            if ( 13000 < uiPluse && 14000 > uiPluse )
            {
                Step = 1;
                BitCount = 0;
            }
            else
            {
                Step = 0;
            }
            break;
        case 1 :
            /* ��ʼ���� */
            infraredValue <<= 1;
            BitCount ++;
            /* ����1 �á��ߵ�ƽ0.56ms���͵�ƽ1.69ms=2.25ms����ʾ */
            if ( 2000 < uiPluse && 2500 > uiPluse )
            {
                /* ��λΪ1 */
                infraredValue |= 0x01;
            }
            /* ��������ʱ0 �á�0.56ms �ߵ�ƽ��0.565ms �͵�ƽ=1.125ms����ʾ */
            else if ( 1000 < uiPluse && 1300 > uiPluse )
            {
                /* ��λΪ0 */
            }
            else
            {
                /* ���ܱ�ʶ�����ó�ʼ���� */
                BitCount = 0;
                Step = 0;
            }

            /* ������� */
            if ( 32 <= BitCount )
            {
                Step = 0;
                g_stinfrared_Set.uiValue = infraredValue;
                g_stinfrared_Set.bIsRefresh = TRUE;
                //LOG_OUT(LOGOUT_DEBUG, "infraredValue:%X", infraredValue);
            }
            break;
        default:
            break;
    }

    //GPIO_REG_WRITE( GPIO_STATUS_W1TC_ADDRESS, INFRA_GPIO_NUM );
    //_xt_isr_unmask(1 << ETS_GPIO_INUM);

}

void INFRA_GPIOInit(void)
{
    GPIO_ConfigTypeDef stGpioCfg;

    stGpioCfg.GPIO_IntrType = GPIO_PIN_INTR_NEGEDGE;
    stGpioCfg.GPIO_Mode     = GPIO_Mode_Input;
    stGpioCfg.GPIO_Pullup   = GPIO_PullUp_EN;
    stGpioCfg.GPIO_Pin      = INFRA_GPIO_NUM;
    gpio_config( &stGpioCfg );
}


void INFRA_InfraredInit(void)
{
    COMM_ExtiIntRegister(INFRA_GPIO_NUM, INFRA_GPIOInit, NULL, INFRA_InfrareHandle, "INFRA_InfrareHandle");
}

VOID INFRARED_JudgeInfrared( VOID )
{
    UINT uiLoopi = 0;
    INFRARED_VALUE_S *pstinfrared = NULL;

    /* ��ȡ���µĺ����ֵ */
    if ( g_stinfrared_Set.bIsRefresh && !g_stinfrared_Set.bIsSetting )
    {
        g_stinfrared_Set.bIsRefresh = FALSE;

        pstinfrared = g_astInfrared_Value;
        /* ����Ƚϼ�ֵ�����趨�ļ�ֵ�Ƿ�ƥ�� */
        for ( uiLoopi = 0 ; uiLoopi < INFRARED_MAX ; uiLoopi++, pstinfrared++ )
        {
            if ( !pstinfrared->bEnable )
            {
                continue;
            }

            /* ���on��offֵһ����̵������涯�� */
            if ( g_stinfrared_Set.uiValue == pstinfrared->uiOnValue &&
                pstinfrared->uiOnValue == pstinfrared->uiOffValue)
            {
                LOG_OUT(LOGOUT_DEBUG, "Recv infrared:%X", g_stinfrared_Set.uiValue);
                PLUG_SetRelayReversal( TRUE );
                break;
            }
            /* on���� */
            else if ( g_stinfrared_Set.uiValue == pstinfrared->uiOnValue )
            {
                LOG_OUT(LOGOUT_DEBUG, "Recv infrared:%X", g_stinfrared_Set.uiValue);
                PLUG_SetRelayOn( TRUE );
                break;
            }
            /* off���� */
            else if ( g_stinfrared_Set.uiValue == pstinfrared->uiOffValue )
            {
                LOG_OUT(LOGOUT_DEBUG, "Recv infrared:%X", g_stinfrared_Set.uiValue);
                PLUG_SetRelayOff( TRUE );
                break;
            }
        }
    }
}


UINT INFRARED_SetInfrared( UINT8 ucNum, UINT8 ucSwitch, UINT uiTimeOut_s )
{
    INFRARED_VALUE_S* pstData = NULL;
    UINT uiCount = 0;
    UINT uiRet = 0;

    if ( ucNum > INFRARED_MAX )
    {
        LOG_OUT(LOGOUT_ERROR, "Invalid infrared num: %d", ucNum);
        return FAIL;
    }
    g_stinfrared_Set.bIsSetting = TRUE;


    while ( !g_stinfrared_Set.bIsRefresh && uiCount < uiTimeOut_s*10 )
    {
        //LOG_OUT(LOGOUT_DEBUG, "waitting for set infrared, %d times", uiCount);
        uiCount++;
        vTaskDelay( 100/portTICK_RATE_MS );
    }

    if ( g_stinfrared_Set.bIsRefresh )
    {
        pstData = INFRARED_GetInfraredData( ucNum-1 );

        if ( ucSwitch )
        {
            pstData->uiOnValue = g_stinfrared_Set.uiValue;
        }
        else
        {
            pstData->uiOffValue = g_stinfrared_Set.uiValue;
        }

        g_stinfrared_Set.bIsRefresh = FALSE;
        g_stinfrared_Set.bIsSetting = FALSE;
        uiRet = INFRARED_SaveInfraredData(pstData);
        if ( uiRet != OK )
        {
            LOG_OUT(LOGOUT_ERROR, "SaveInfraredData failed");
            return FAIL;
        }

        return OK;
    }

    LOG_OUT(LOGOUT_ERROR, "Set infrared timeout %d S", uiTimeOut_s);

    g_stinfrared_Set.bIsSetting = FALSE;
    return FAIL;
}


UINT INFRARED_GetInfraredValue( UINT8 ucNum, UINT8 ucSwitch, UINT uiTimeOut_s )
{
    UINT uiCount = 0;
    UINT uiRet = 0;

    if ( ucNum > INFRARED_MAX )
    {
        LOG_OUT(LOGOUT_ERROR, "Invalid infrared num: %d", ucNum);
        return 0;
    }
    g_stinfrared_Set.bIsSetting = TRUE;


    while ( !g_stinfrared_Set.bIsRefresh && uiCount < uiTimeOut_s*10 )
    {
        //LOG_OUT(LOGOUT_DEBUG, "waitting for set infrared, %d times", uiCount);
        uiCount++;
        vTaskDelay( 100/portTICK_RATE_MS );
    }

    if ( g_stinfrared_Set.bIsRefresh )
    {
        g_stinfrared_Set.bIsRefresh = FALSE;
        g_stinfrared_Set.bIsSetting = FALSE;

        return g_stinfrared_Set.uiValue;
    }

    LOG_OUT(LOGOUT_ERROR, "Set infrared timeout %d S", uiTimeOut_s);

    g_stinfrared_Set.bIsSetting = FALSE;
    return 0;
}

VOID INFRARED_InfraredDataDeInit( VOID )
{
    UINT uiLoopi = 0;
    INFRARED_VALUE_S *pstData = NULL;
    CHAR szName[PLUG_NAME_MAX_LEN] = "";

    for ( uiLoopi = 0; uiLoopi < INFRARED_MAX; uiLoopi++ )
    {
        pstData = &g_astInfrared_Value[uiLoopi];

        pstData->uiNum        = uiLoopi+1;
        pstData->bEnable    = FALSE;
        pstData->uiOnValue    = 0;
        pstData->uiOffValue = 0;

        snprintf(szName, sizeof(szName), "infrared %d", uiLoopi+1);
        strcpy(pstData->szName, szName);
    }
}


INFRARED_VALUE_S* INFRARED_GetInfraredData( UINT8 ucNum )
{
    if ( ucNum >= INFRARED_MAX )
    {
        return &g_astInfrared_Value[0];
    }
    return &g_astInfrared_Value[ucNum];
}

UINT32 INFRARED_GetInfraredDataSize()
{
    return sizeof(g_astInfrared_Value);
}

UINT INFRARED_SaveInfraredData( INFRARED_VALUE_S* pstData )
{
    UINT uiRet = OK;

    if ( NULL == pstData )
    {
        LOG_OUT(LOGOUT_ERROR, "pstData = 0x%p", pstData);
        return FAIL;
    }

    uiRet = CONFIG_infraredDataCheck( pstData );
    if ( uiRet != OK )
    {
        LOG_OUT(LOGOUT_ERROR, "pstData check failed.");
        return FAIL;
    }

    memcpy(INFRARED_GetInfraredData(pstData->uiNum - 1), pstData, sizeof(INFRARED_VALUE_S));
    CONFIG_SaveConfig(PLUG_MOUDLE_INFRARED);
    return OK;
}



