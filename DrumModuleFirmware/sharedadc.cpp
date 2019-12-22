#include "sharedadc.h"
#include "hardware.h"

ADC* SharedADC::adc = new ADC();

void SharedADC::setup()
{
	pinMode(ANALOG_IN0, INPUT);
	pinMode(ANALOG_IN1, INPUT);

	adc->setAveraging(4, ADC_0);
	adc->setResolution(10, ADC_0);
	adc->setConversionSpeed(ADC_CONVERSION_SPEED::LOW_SPEED, ADC_0);
	adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED, ADC_0);

	adc->setAveraging(4, ADC_1);
	adc->setResolution(10, ADC_1);
	adc->setConversionSpeed(ADC_CONVERSION_SPEED::LOW_SPEED, ADC_1);
	adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED, ADC_1);
}