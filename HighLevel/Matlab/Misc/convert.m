function CJ(in)

x = 0;
% open the file with write permission

     for i = in:in+100
                 

Y = [num2str(i),'.','t','x','t',];
R= [num2str(i),'.','j','p','g',];

 s = imread(R);
s = s(:,:,1);
s = transpose(s);
fid = fopen(Y, 'w');       
    fprintf(fid, '%d\n', s);
%     fprintf(fid, '%c', s);    
fclose(fid);
 end
% view the contents of the file
% type exp.txt
end
