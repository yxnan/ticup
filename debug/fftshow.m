function [] = fftshow(varargin)
narginchk(1,2);
filename = varargin{1};
type = 'mag';
if nargin == 2
    type = varargin{2};
end

fp = fopen(filename, 'r');
fft_received = fscanf(fp, '%d');
if strcmp(type, 'db')
    freq_base = fft_received(20);
    fft_received = 20*log10(fft_received/freq_base);
end

figure;
plot(fft_received);
end