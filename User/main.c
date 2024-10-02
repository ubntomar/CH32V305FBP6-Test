/********************************** (C) MIT License *******************************
* File Name          : main.c
* Author             : Omar Hernandez
* Version            : V1.0.0
* Date               : 2024/10/01
* Description        : Main program body with ADC reading and USART transmission.
***********************************************************************************

/*
 *@Note
 This example demonstrates how to read ADC values and send them periodically
 over USART using the printf function provided by the debug library.

 - The ADC reads values from pin PA1 (ADC Channel 1).
 - The values are transmitted over USART3 using pin PB10 (Tx) at 115200 baudrate.
 - PB11 (Rx) is configured but not used in this example.

 Connections:
 - Connect your analog input signal to pin PA1 for ADC reading.
 - Connect PB10 (Tx) to the Rx pin of the device you wish to send data to (e.g., a PC with a USB-to-Serial adapter).
 - PB11 (Rx) is optional if you want to use it for receiving data.

 Configuration Note:
 - In the file `debug.h`, you can select the USART to be used by modifying the `DEBUG` macro.
 - For example, `#define DEBUG DEBUG_UART3` configures the system to use USART3. The corresponding pins for USART3 on the CH32V305FBP6 are PB10 for Tx and PB11 for Rx.
 - Make sure to connect the appropriate pins based on the selected USART.

 - The Watchdog timer is configured to reset the system if it is not fed within 1.6 seconds.
 - A function (`Long_Delay_With_IWDG`) ensures that during long delays, the Watchdog is fed periodically to prevent an unintended reset.
*/

#include "debug.h"

/* Global typedef */

/* Global define */

/* Global Variable */
uint16_t adc_value = 0;  // Variable to store ADC value

/*********************************************************************
 * @fn      IWDG_Init
 *
 * @brief   Initializes the Independent Watchdog (IWDG) to prevent system lock.
 *
 * @return  none
 */
void IWDG_Init(void)
{
    /* Enable the Watchdog with a timeout of approximately 1.6 seconds */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64);   // Prescaler 64
    IWDG_SetReload(2000);                   // Reload value for ~1.6 seconds timeout
    IWDG_ReloadCounter();                   // Reload the watchdog timer
    IWDG_Enable();                          // Enable the watchdog timer
}

/*********************************************************************
 * @fn      Feed_IWDG
 *
 * @brief   Feeds (refreshes) the Watchdog timer to prevent system reset.
 *
 * @return  none
 */
void Feed_IWDG(void)
{
    IWDG_ReloadCounter();   // Reload the watchdog timer to prevent timeout
}

/*********************************************************************
 * @fn      Long_Delay_With_IWDG
 *
 * @brief   Delays for the specified time in milliseconds, while periodically
 *          feeding the Watchdog to prevent system reset.
 *
 * @param   total_delay_ms - Total delay time in milliseconds.
 *
 * @return  none
 */
void Long_Delay_With_IWDG(uint32_t total_delay_ms)
{
    uint32_t elapsed = 0;

    /* Split the total delay into smaller intervals, feeding the Watchdog each time */
    while (elapsed < total_delay_ms)
    {
        Delay_Ms(500);    // Delay for 500 ms
        Feed_IWDG();      // Feed the Watchdog
        elapsed += 500;   // Update elapsed time
    }
}

/*********************************************************************
 * @fn      ADC_Function_Init
 *
 * @brief   Initializes the ADC to read values from PA1.
 *
 * @return  none
 */
void ADC_Function_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable clocks for GPIOA and ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    /* Configure PA1 (ADC Channel 1) as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure the ADC */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Configure the ADC channel */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

    /* Enable the ADC */
    ADC_Cmd(ADC1, ENABLE);

    /* Calibrate the ADC */
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    /* Start ADC conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    /* Initialize system settings .. */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);  // Initialize USART3 for debugging

    /* Initialize Watchdog */
    IWDG_Init();  // Watchdog with a timeout of ~1.6 seconds

    /* Print system information */
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());
    printf("Starting ADC read example with Watchdog enabled\r\n");

    /* Initialize the ADC */
    ADC_Function_Init();

    /* Main loop */
    while(1)
    {
        /* Read the ADC value */
        adc_value = ADC_GetConversionValue(ADC1);

        /* Print the ADC value over UART */
        printf("ADC Value: %d\r\n", adc_value);

        /* Perform a long delay (5 seconds), feeding the Watchdog during the delay */
        Long_Delay_With_IWDG(5000);  // 5 seconds delay
    }
}
