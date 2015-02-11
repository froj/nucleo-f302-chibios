#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#define ADC_TO_VOLTS   0.001611328f

BaseSequentialStream* stdout;


static THD_WORKING_AREA(adc_task_wa, 256);
static THD_FUNCTION(adc_task, arg)
{
    (void)arg;
    chRegSetThreadName("adc read");
    static adcsample_t adc_samples[1];
    static const ADCConversionGroup adcgrpcfg1 = {
        FALSE,                  // circular?
        1,                      // nb channels
        NULL,                   // callback fn
        NULL,                   // error callback fn
        0,                      // CFGR
        0,                      // TR1
        ADC_CCR_VBATEN,         // CCR : enable Vbat
        {ADC_SMPR1_SMP_AN1(0), 0},  // SMPRx : sample time minimum
        {ADC_SQR1_NUM_CH(1) | ADC_SQR1_SQ1_N(17), 0, 0, 0}, // SQRx : ADC1_CH17
    };

    while (1) {
        adcConvert(&ADCD1, &adcgrpcfg1, adc_samples, 1);

        chprintf(stdout, "%f V\n", adc_samples[0]*ADC_TO_VOLTS);
        chThdSleepMilliseconds(500);
    }
    return 0;
}

int main(void)
{
    halInit();
    chSysInit();

    adcStart(&ADCD1, NULL);
    sdStart(&SD2, NULL);
    stdout = (BaseSequentialStream*)&SD2;

    chprintf(stdout, "Boot\n");

    chThdCreateStatic(adc_task_wa, sizeof(adc_task_wa), LOWPRIO, adc_task, NULL);

    while(42){
        palSetPad(GPIOB, GPIOB_LED);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOB, GPIOB_LED);
        chThdSleepMilliseconds(500);
    }
}
