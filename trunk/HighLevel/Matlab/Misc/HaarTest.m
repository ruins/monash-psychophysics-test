%Function HaarTest3() by Meng Chen @ Monash University
%Updated 08/10/10
%J - input signal
%P - reduce coefficients to P%. P value ignored if F = 0
%F - bitwise flag 0-15
    %Remove difference coefficients from
    %bit 4 set = high pass
    %bit 3 Diagonal matrix   (4)
    %bit 2 Vertical matrix   (2)
    %bit 1 Horizontal matrix  (1)
    % 0000 - all channels, low pass
    % 1000 (8) - all channels, highpass. These two modes overlap with 0111
    % and 1111, perhaps should develop into special modes of some sort
    %
    % default - without F specified, all channels affected, F = 0000
    
%Level of decomposition, default log2 of 1-D data length. If J is matrix N= number of columns

function [Ori_Mat,S_Mat,Rec_Mat]=HaarTest3(J,F,freq1,freq2)
 
if(length(size(J))>2)
    
J = double(rgb2gray(J));

elseif isa(J,'double')~=1 
    
    J = double(J);
end

N = ceil(log2(length(J)));

if (nargin <4) 

    freq2=1/0;
end

if nargin <3
    
    F=0;
       
else
    if F >2
        
        F=2;
        
    end
      
end


Flag = F;
  
[C, S] = wavedec2(J,N,'haar');

length_A=S(1,1)*S(1,2);

Freqs= zeros(N,2);

len=size(J,1);
wid=size(J,2);

for i = 1:N
    
Freqs(i,1)= round(len/S(i+1,1));
Freqs(i,2)=round(wid/S(i+1,2));

end





if Flag == 0
    
PERC = floor((length(C)-length_A)*P*0.01);

C(PERC+length_A:end)=0;

elseif Flag==8
    
    if P==100
    else
        
    PERC = floor((length(C)-length_A)*P*0.01);
    C(length_A+1:(length(C)-PERC))=0;
    
    end
    
else
    
    if P==100
        
    else
    
template=zeros(size(C));

counter = length_A;

N_coef=0;

for i = 2:1:(N+1)

    N_coef=N_coef+S(i,1)*S(i,2);  
      
    for j = 0:2
        
       template(counter+1:counter+S(i,1)*S(i,2))=2^j;
    
       counter = counter+S(i,1)*S(i,2);

    end
    
end

weight=bitand(1,Flag)+bitshift(bitand(2,Flag),-1)+bitshift(bitand(4,Flag),-2);

counter=0;

PERC = ceil(weight*N_coef*0.01*P);


for i = 2:length(template)

    if (bitand(template(i),Flag))~=0
        
        counter=counter+1;
        
        if (bitand(8,Flag))==0
            
        if counter>=PERC
                
        C(i)=0;
        
        end
        
        else
          
        if counter<=(N_coef*weight-PERC)
         
        C(i)=0;
               
        end    
            
        end
        
            
    end
    
    
    
end

    end

end

RC=waverec2(C,S,'haar');

figure; imshow(uint8(RC));

title(['P=',num2str(P),'%','  F=',num2str(F),'  N=',num2str(N)]);

S_Mat=S; Ori_Mat=C; Rec_Mat=RC;

end
