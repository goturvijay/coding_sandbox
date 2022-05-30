#include<stdio.h>
#include<math.h>

#define SIGNAL_LENGTH       128
#define AMP_INCREASE_DB     12

/*
    @brief sturture to depict an audio signal
*/
typedef struct
{
    int* samples;   /* <  pointer to samples of audio signals */
    long length;    /* <  number of samples in the signal*/
} signal;


/*
    @brief soft_amp_increase accepts an input audio signal of 128 samples and performs a 
    soft amplitude increase of AMP_INCREASE_DB decibel over the midway point, based on softness factor
    
    @param  audio_in        structure that represents the input audio signal, of type signal
    @param  softness_factor A percentage value that decides the softness of the increase over
                            the input signal. Val 100 = increase happens over the entire input signal.
                            Val 1 = increase happens abruptly. Other values are currently not accepted.
                            
    @return                 value 1 signifies successful amplification
    
*/
int soft_amp_increase(signal* audio_in, int softness_factor)
{
    //-----------------------------------
    //input checks
    //-----------------------------------
    assert(audio_in != NULL);
    if(audio_in->length != SIGNAL_LENGTH)
    {
        return 0;   // TODO: define error codes
    }
    if((softness_factor <= 0)&&(softness_factor > 100))
    {
        return 0;   // TODO: define error codes
    }
    
    //-----------------------------------
    // soft increase factor characteristics
    //-----------------------------------

    long curve_start_idx, curve_end_idx;
    float curve_length;
    
    curve_length = (SIGNAL_LENGTH / 100) * softness_factor ;
    
    curve_start_idx = (SIGNAL_LENGTH - (long)curve_length) /  2;
    curve_end_idx = (SIGNAL_LENGTH + (long)curve_length) /  2;
    //TODO: check boundary conditions, and odd number/ fractional number behavior for extending softness factor
    
    float curve_start_val = 1.0; 
    // V_in at the start of curve is same as the input audio signal
    
    // assuming audio values as voltages and AMP_INCREASE_DB = 20 log (V_out/V_in)
    float curve_end_val = pow(10, (AMP_INCREASE_DB / 20)); 
    int curve_end_val_int;
    int curve_end_val_frac = modf(curve_end_val, &curve_end_val_int);
    if (curve_end_val_frac >= 0.5)
        curve_end_val_int++;
    //extract fractional part of curve_end_val and round off.
    //possible loss of precision here when converting float to int
    
    //using a linear curve for increase y = slope*x + offset;
    //note: other much softer non linear curves may be used possibly with some look-up tables
    float slope = (curve_end_val - curve_start_val)/((long)curve_length);
    int slope_int;
    int slope_frac = modf(slope, &slope_int);
    if (slope_frac >= 0.5)
        slope_int++;
    //extract fractional part of slope and round off.
    //possible loss of precision here when converting float to int
    
    //-----------------------------------
    // apply soft increase on input signal
    //-----------------------------------
    for (int i = curve_start_idx; i < curve_end_idx; i++)
    {
        int factor = (slope_int * (i - curve_start_idx) + (int)curve_start_val);
        audio_in->samples[i] = audio_in->samples[i] * factor; 
    }
    
    for (int i = curve_end_idx; i < SIGNAL_LENGTH; i++)
    {
        audio_in->samples[i] = audio_in->samples[i] * curve_end_val_int;
    }
    
    return 1;
    
}