%function HaarTest() by Meng Chen @ Monash University
%Updated 21/09/10
%J - input signal
%P - reduce coefficients to P%. P value ignored if F = 0
%F - bitwise flag 0-15
    %Remove difference coefficients from
    %bit 4 set = high pass
    %bit 3 Diagonal matrix   (4)
    %bit 2 Vertical matrix   (2)
    %bit 1 Horizontal matrix  (1)
    % 0000 - all channels, low pass
    % 1000 - all channels, highpass
    %default - without F specified, all channels affected, F = 0000
%L - level of decomposition, default log2 of 1-D data length

function [Ori_Mat,S_Mat,Rec_Mat]=HaarTest(J,P,F,L)
 
if(length(size(J))>2)
    
J = double(rgb2gray(J));

elseif isa(J,'double')~=1 
    
    J = double(J);
end

if (nargin <4 )||(L<0)||(L>20) %upper and lower limit for levels of decomposition, upper can be altered as needed
    
    N = ceil(log2(length(J)));
        
else
    
    N=ceil(L);
    
end

if nargin <3
    
    F=7;
       
else
    if F >7
        
        F=7;
        
    end
      
end

if (P<0)
    
    P=0;
    
elseif (P>100)
    
    P=100;

end

Flag = F;
  
[C, S] = wavedec2(J,N,'haar');

length_A=S(1,1)*S(1,2);

if Flag == 7
    
PERC = floor((length(C)-length_A)*P*0.01);

C(PERC+length_A:end)=0;

elseif Flag==0
    
    %do nothing
    
else
    
template=zeros(size(C));

counter = length_A;

N_coef=0;

for i = 2:1:N+1

    N_coef=N_coef+S(i,1)*S(i,2);  
      
    for j = 0:2
        
       template(counter+1:counter+S(i,1)*S(i,2))=2^j;
    
       counter = counter+S(i,1)*S(i,2);

    end
    
end

weight=bitand(1,Flag)+bitshift(bitand(2,Flag),-1)+bitshift(bitand(4,Flag),-2);

counter=0;

PERC = ceil(weight*N_coef*0.01*P);

for i = 1:length(template)

    if (bitand(template(i),Flag))~=0
        
        counter=counter+1;
        
        if counter>=PERC
                
        C(i)=0;
        
        end
            
    end
    
end

end

RC=waverec2(C,S,'haar');

figure; imshow(uint8(RC));

title(['P=',num2str(P),'%','  F=',num2str(F),'  N=',num2str(N)]);

S_Mat=S; Ori_Mat=C; Rec_Mat=RC;

end
