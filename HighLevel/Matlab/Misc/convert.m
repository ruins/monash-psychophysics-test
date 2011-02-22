x = 0;
% open the file with write permission

s = imread('GG.bmp');
s = s(:,:,1);%reduce matrix to two dimensional 
fid = fopen('thesis.txt', 'w');       
    fprintf(fid, '%d\n', s);
fclose(fid);

% view the contents of the file
% type exp.txt
