function [filename] = lookup(varargin)
narginchk(0,1);
if nargin == 1
    filename = ['data_receive/' num2str(varargin{1})];
else
    fp = fopen('data_receive/index', 'r');
    filename = ['data_receive/' num2str(fscanf(fp, '%d'))];
end

