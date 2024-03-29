function Hd = FIR_LP
%FIR_LP Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.8 and DSP System Toolbox 9.10.
% Generated on: 06-Nov-2020 22:06:51

% Equiripple Lowpass filter designed using the FIRPM function.

% All frequency values are in Hz.
Fs = 24000;  % Sampling Frequency

Fpass = 3000;             % Passband Frequency
Fstop = 4200;             % Stopband Frequency
Dpass = 0.0057563991496;  % Passband Ripple
Dstop = 0.001;            % Stopband Attenuation
dens  = 20;               % Density Factor

% Calculate the order from the parameters using FIRPMORD.
[N, Fo, Ao, W] = firpmord([Fpass, Fstop]/(Fs/2), [1 0], [Dpass, Dstop]);

% Calculate the coefficients using the FIRPM function.
b  = firpm(N, Fo, Ao, W, {dens});
Hd = dfilt.dffir(b);

% [EOF]
