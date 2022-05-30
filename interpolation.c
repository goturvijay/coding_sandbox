#include <stdio.h>

#define INPUT_LENGTH                    16

/*
    @brief sturture to depict a data signal
*/
typedef struct
{
    float* samples;   /* <  pointer to samples of audio signals as 32 bit floating point values */
    long length;    /* <  number of samples in the signal*/
} signal;

/* @brief this function interpolates an input signal by a factor of 4
    
    @param  input   pointer to an input signal struct with length INPUT_LENGTH 
    @param  output  pointer to an output signal struct with length of at least INPUT_LENGTH * 4
    
    @return         value is 1 if interpolation is successful, 0 otherwise

*/
int interpolate_signal_factor_4(signal* input, signal* output)
{
    //input checks
    assert(input != NULL);
    assert(output != NULL);
    
    const int INTERPOLATION_FACTOR_SHIFT = 2;  //number of bit shifts needed for indexing for interpolation factor of 4
                                           //using bit shifts for multiplication assuming interpolation
                                           //factors are multiples of 2
    
    //input length should match required value
    if(input->length != INPUT_LENGTH)
    {
        return 0;
    }
    
    //output buffer should be big enough to accomodate the interpolated
    //signal
    if(output->length < (INPUT_LENGTH << INTERPOLATION_FACTOR_SHIFT))
    {
        return 0;
    }
    
    //interpolation
    float slope;
    float *pin;
    float *p1, *p2, *p3, *p4;
    output->samples[0] = input->samples[0];
    for(long i = 1; i < INPUT_LENGTH; i++)
    {
        long  curr_out_idx = i << INTERPOLATION_FACTOR_SHIFT;
        pin = input->samples + i;
        float curr_in_val = *pin;
        float prev_in_val = *(pin - 1);
        slope = (curr_in_val - prev_in_val)/4;
        
        p1 = output->samples + curr_out_idx;
        p2 = p1 - 1;
        p3 = p2 - 1;
        p4 = p3 - 1;
        
        *p1 = curr_in_val;
        *p2 = *p1 - slope;
        *p3 = *p2 - slope;
        *p4 = *p3 - slope;
    }
    //retain the last slope for interpolating the last 3 samples
    long last_out_idx = INPUT_LENGTH << INTERPOLATION_FACTOR_SHIFT - 1;
    long last_in_idx = INPUT_LENGTH - 1;
    long last_in_val = input->samples[last_in_idx];
    p1 = output->samples + last_out_idx - 2;
    p2 = p1 + 1;
    p3 = p2 + 1;
    *p1 = last_in_val + slope;
    *p2 = *p1 + slope;
    *p3 = *p2 + slope;
    
    pin = NULL;
    p1 = NULL;
    p2 = NULL;
    p3 = NULL;
    p4 = NULL;
    
    return 1;
    
}