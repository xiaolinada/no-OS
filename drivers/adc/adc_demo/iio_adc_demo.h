/***************************************************************************//**
 *   @file   iio_adc_demo.h
 *   @brief  Header file of ADC Demo iio.
 *   @author RNechita (ramona.nechita@analog.com)
********************************************************************************
 * Copyright 2021(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef IIO_DEMO_ADC
#define IIO_DEMO_ADC

#include <stdlib.h>
#include "adc_demo.h"
#include "iio_types.h"

#define DEFAULT_CHANNEL_NO 2
#define ADC_CHANNEL_NO changeable_channel_number
#define MAX_SAMPLES_PER_CHANNEL 800

static int changeable_channel_number = 2;

#define ADC_DEMO_ATTR(_name, _priv) {\
	.name = _name,\
	.priv = _priv,\
	.show = get_adc_demo_attr,\
	.store = set_adc_demo_attr \
}

static struct scan_type adc_scan_type = {
	.sign = 's',
	.realbits = 16,
	.storagebits = 16,
	.shift = 0,
	.is_big_endian = false
};

static struct iio_attribute adc_channel_attributes[] = {
	ADC_DEMO_ATTR("adc_channel_attr", ADC_CHANNEL_ATTR),
	END_ATTRIBUTES_ARRAY,
};

static struct iio_attribute iio_adc_global_attributes[] = {
	ADC_DEMO_ATTR("adc_global_attr", ADC_GLOBAL_ATTR),
	END_ATTRIBUTES_ARRAY,
};

#define IIO_DEMO_ADC_CHANNEL(_idx) {\
	.name = "adc_in_ch",\
	.ch_type = IIO_VOLTAGE,\
	.channel = _idx,\
	.scan_index = _idx,\
	.indexed = true,\
	.scan_type = &adc_scan_type,\
	.attributes = adc_channel_attributes,\
	.ch_out = false,\
}

static struct iio_channel iio_adc_channels[] = {
	IIO_DEMO_ADC_CHANNEL(0),
	IIO_DEMO_ADC_CHANNEL(1),
#ifdef MODIFIED_CHANNEL_VALUE
	IIO_DEMO_ADC_CHANNEL(2),
	IIO_DEMO_ADC_CHANNEL(3),
	IIO_DEMO_ADC_CHANNEL(4),
	IIO_DEMO_ADC_CHANNEL(5),
	IIO_DEMO_ADC_CHANNEL(6),
	IIO_DEMO_ADC_CHANNEL(7),
	IIO_DEMO_ADC_CHANNEL(8),
	IIO_DEMO_ADC_CHANNEL(9),
	IIO_DEMO_ADC_CHANNEL(10),
	IIO_DEMO_ADC_CHANNEL(11),
	IIO_DEMO_ADC_CHANNEL(12),
	IIO_DEMO_ADC_CHANNEL(13),
	IIO_DEMO_ADC_CHANNEL(14),
	IIO_DEMO_ADC_CHANNEL(15),
#endif
};

#define ADC_DEMO_DEV(_numch) {\
	.num_ch = _numch, \
	.channels = iio_adc_channels, \
	.attributes = iio_adc_global_attributes,	\
	.debug_attributes = NULL,	\
	.buffer_attributes = NULL,	\
	.prepare_transfer = update_adc_channels,	\
	.end_transfer = close_adc_channels,	\
	.read_dev = (int32_t (*)())adc_read_samples,	\
	.debug_reg_read = (int32_t (*)()) adc_demo_reg_read,	\
	.debug_reg_write = (int32_t (*)()) adc_demo_reg_write	\
}

//static struct iio_device adc_demo_iio_descriptor = ADC_DEMO_DEV(MODIFIED_CHANNEL_VALUE);

//int32_t init_adc_channels(struct adc_demo_desc* dev, uint32_t mask);

#endif /* IIO_DEMO_ADC */
