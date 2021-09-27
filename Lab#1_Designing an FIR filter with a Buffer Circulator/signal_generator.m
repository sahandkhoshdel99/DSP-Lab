function [sin_gen , t] = generator(freq,sampling_frequency)

    period = 1/sampling_frequency;
    Length = 2000;
    t = (0:Length - 1)*period;
    sin_gen = sin(2*pi*freq*t) ;
