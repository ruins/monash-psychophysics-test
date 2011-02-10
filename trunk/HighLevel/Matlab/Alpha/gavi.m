
Filename = 'cropped.avi';
FPS = 3;
Inputimage = 'le.png'; %filename here
Ite = 200;
Sigma=1.2;
Fsize=9;
Factor = 0.3;

aviobj = avifile(Filename,'fps',FPS,'compression','None');

I = imread(Inputimage);
if(length(size(I))>2)
I = rgb2gray(I);
end

%H = fspecial('gaussian',Fsize,Sigma);
H = fspecial('average',Fsize);


counter=1;
t_val=1;
array=zeros(1,2);

X = zeros(size(I,1),size(I,2),2);



for i = 1:0.3:Ite
    
    
I = imfilter(I,H,'conv','replicate');    
    
t_val = (sqrt(2))^counter+8;
if (round(Factor*(t_val))) == round(i)
    
    array(1,counter)=t_val;
    X(:,:,counter)=I;
    counter=counter+1;
    
   
end

end

fig=figure;

for i = 1:counter-1
    
imshow(uint8(X(:,:,counter-i)));

axis equal

frame = getframe(fig);

 aviobj = addframe(aviobj,frame);    
  aviobj = addframe(aviobj,frame);
  
end

aviobj = close(aviobj);

size(X)
figure
plot(array)
length(array)
