%function setProb() by Meng Chen @ Monash University
%Updated 24/09/10
%P - input probability required
%x - output, position calculated
%sigma - output, std dev calculated
%Takes an input probability and works out the value which Prob(x<X)=P

function [x,sigma] = setProb(P)

Found = false;
decPlace=5;%accurate to the 5th decimal place

if (P<0)
    x=1/0;
    sigma=1/0;
elseif (P>1)
    x=1/0;
    sigma=1/0;%error input
else
    
  sig=1;
  
  
   position=double(0);
   inc_flag=true;
   
    if P == 0.5
       position = 0;
               
    elseif P<0.5
         increment=-1;
        while(Found==false)
            position=position+increment;
        Prob= normcdf(position,0,sig);
        
        if round(Prob*10^decPlace)==round(P*10^decPlace)
            
            
            Found=true;
            
        elseif(Prob<P)
            
           if(inc_flag)
               increment=-increment/2;
                
                inc_flag=false;
           else
               
               %do nothing
           end
           
            
        else
             if (inc_flag)
                             
                %do nothing
            else
               increment=-increment/2;
                
                inc_flag=true;
                                
            end
            
        end
        
        
        
        end
    else
         increment=double(1);
        while(Found==false)
            position=position+increment;
        Prob= normcdf(position,0,sig);
        
        if round(Prob*10^decPlace)==round(P*10^decPlace)
            
            
            Found=true;
            
        elseif(Prob<P)
            
            if (inc_flag)
                
        
            else
            
                increment=-increment/2;
                
                inc_flag=true;
                
            end
            
        else
            
            if (inc_flag)
                
                increment=-increment/2;
                
                inc_flag=false;
                
            else
               
                                
            end
            
        end
               
        
        end
                        
        
    end
     x=position;
     sigma=sig;
  
   
end
end





