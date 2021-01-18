/*
 * Created by Robert Fraczkiewicz, 01/2018
 * New signal processing methodology for obtaining heart rate and SpO2 data
 * from the MAX30102 sensor manufactured by MAXIM Integrated Products, Inc.
 */
/*******************************************************************************
* Copyright (C) 2017 Robert Fraczkiewicz, All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL ROBERT FRACZKIEWICZ BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Robert Fraczkiewicz retains all
* ownership rights.
*******************************************************************************
*/

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdbool.h>
#include <stdint.h>

/*
 * Settable parameters
 * Leave these alone if your circuit and hardware setup match the defaults
 * described in this code's Instructable. Typically, different sampling rate
 * and/or sample length would require these paramteres to be adjusted.
 */
#define ST 4      // Sampling time in s. WARNING: if you change ST, then you MUST recalcuate the sum_X2 parameter below!
#define FS 25     // Sampling frequency in Hz. WARNING: if you change FS, then you MUST recalcuate the sum_X2 parameter below!


// Sum of squares of ST*FS numbers from -mean_X (see below) to +mean_X incremented be one. For example, given ST=4 and FS=25,
// the sum consists of 100 terms: (-49.5)^2 + (-48.5)^2 + (-47.5)^2 + ... + (47.5)^2 + (48.5)^2 + (49.5)^2
// The sum is symmetrc, so you can evaluate it by multiplying its positive half by 2. It is precalcuated here for enhanced
// performance.



// WARNING: The two parameters below are CRUCIAL! Proper HR evaluation depends on these.
#define MAX_HR 180  // Maximal heart rate. To eliminate erroneous signals, calculated HR should never be greater than this number.
#define MIN_HR 40   // Minimal heart rate. To eliminate erroneous signals, calculated HR should never be lower than this number.



/**
* \brief        Calculate the heart rate and SpO2 level, Robert Fraczkiewicz version
* \par          Details
*               By detecting  peaks of PPG cycle and corresponding AC/DC of red/infra-red signal, the xy_ratio for the SPO2 is computed.
*
* \param[in]    *pun_ir_buffer           - IR sensor data buffer
* \param[in]    n_ir_buffer_length      - IR sensor data buffer length
* \param[in]    *pun_red_buffer          - Red sensor data buffer
* \param[out]    *pn_spo2                - Calculated SpO2 value
* \param[out]    *pch_spo2_valid         - 1 if the calculated SpO2 value is valid
* \param[out]    *pn_heart_rate          - Calculated heart rate value
* \param[out]    *pch_hr_valid           - 1 if the calculated heart rate value is valid
*
* \retval       None
*/
void rf_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer, int32_t n_ir_buffer_length, uint32_t *pun_red_buffer, float *pn_spo2, int8_t *pch_spo2_valid, int32_t *pn_heart_rate,
                                        int8_t *pch_hr_valid, float *ratio, float *correl);


/**
* \brief        Coefficient beta of linear regression
* \par          Details
*               Compute directional coefficient, beta, of a linear regression of pn_x against mean-centered
*               point index values (0 to BUFFER_SIZE-1). xmean must equal to (BUFFER_SIZE-1)/2! sum_x2 is
*               the sum of squares of the mean-centered index values.
*               Robert Fraczkiewicz, 12/22/2017
* \retval       Beta
*/
float rf_linear_regression_beta(float *pn_x, float xmean, float sum_x2);

/**
* \brief        Autocorrelation function
* \par          Details
*               Compute autocorrelation sequence's n_lag's element for a given series pn_x
*               Robert Fraczkiewicz, 12/21/2017
* \retval       Autocorrelation sum
*/
float rf_autocorrelation(float *pn_x, int32_t n_size, int32_t n_lag);

/**
* \brief        Root-mean-square variation
* \par          Details
*               Compute root-mean-square variation for a given series pn_x
*               Robert Fraczkiewicz, 12/25/2017
* \retval       RMS value and raw sum of squares
*/
float rf_rms(float *pn_x, int32_t n_size, float *sumsq);

/**
* \brief        Correlation product
* \par          Details
*               Compute scalar product between *pn_x and *pn_y vectors
*               Robert Fraczkiewicz, 12/25/2017
* \retval       Correlation product
*/
float rf_Pcorrelation(float *pn_x, float *pn_y, int32_t n_size);

/**
* \brief        Search the range of true signal periodicity
* \par          Details
*               Determine the range of current heart rate by locating neighborhood of
*               the _first_ peak of the autocorrelation function. If at all lags until
*               n_max_distance the autocorrelation is less than min_aut_ratio fraction
*               of the autocorrelation at lag=0, then the input signal is insufficiently
*               periodic and probably indicates motion artifacts.
*               Robert Fraczkiewicz, 04/25/2020
* \retval       Average distance between peaks
*/
void rf_initialize_periodicity_search(float *pn_x, int32_t n_size, int32_t *p_last_periodicity, int32_t n_max_distance, float min_aut_ratio, float aut_lag0);

/**
* \brief        Signal periodicity
* \par          Details
*               Finds periodicity of the IR signal which can be used to calculate heart rate.
*               Makes use of the autocorrelation function. If peak autocorrelation is less
*               than min_aut_ratio fraction of the autocorrelation at lag=0, then the input
*               signal is insufficiently periodic and probably indicates motion artifacts.
*               Robert Fraczkiewicz, 01/07/2018
* \retval       Average distance between peaks
*/
void rf_signal_periodicity(float *pn_x, int32_t n_size, int32_t *p_last_periodicity, int32_t n_min_distance, int32_t n_max_distance, float min_aut_ratio, float aut_lag0, float *ratio);

#endif /* ALGORITHM_H_ */
