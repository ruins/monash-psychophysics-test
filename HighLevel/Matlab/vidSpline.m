
function [vid,c] = vidSpline(I,xx,yy,Tmax) 

Ite = length(xx);

Sigma=1.2;

Fsize=9;

if(length(size(I))>2)
    
I = rgb2gray(I);

end

if (nargin<4)
    
Tmax = 500;
    
end

H = fspecial('gaussian',Fsize,Sigma);

counter=1;

X = zeros(size(I,1),size(I,2),2);

for i = 1:Ite
      J=I;
      if (yy(i)>Tmax)
          
          yy(i)= Tmax;
          
      end
for j = 1:yy(i)
    
     J = imfilter(J,H,'conv','replicate');    
    
end

    X(:,:,counter)=J;
    
    counter=counter+1;
   
end

vid = X;

c = counter;

end
