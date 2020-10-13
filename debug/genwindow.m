function [] = genwindow(windowname, size)
if strcmp(windowname, 'hanning')
    wnd = hanning(size);
elseif strcmp(windowname, 'flattop')
    wnd = flattopwin(size);
elseif strcmp(windowname, 'blackman')
    wnd = blackman(size);
else
    error('Invalid window');
end

fp = fopen('window_gen.c', 'w');
fprintf(fp, ['const float ' windowname '_' num2str(size) '[] =\n{']);
for i = 1:1:size
    if mod(i, 16) == 1
        fprintf(fp, '\n    ');
    end
    fprintf(fp, '%.6f, ', wnd(i));
end
fprintf(fp, '\n};');

fclose(fp);
end

