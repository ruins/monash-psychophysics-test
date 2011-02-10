function [a,b,c] = getSpline(Tmax,Nmax,n,p)

if (nargin<3)
    
    n=5;

end

if (nargin<4)
    
    p=200;
    
end

figure

plot(0:1,0:1);

[x,y]=ginput(n);

xx=linspace(0,Nmax,p);

yy=spline(x*Nmax,y*Tmax,xx);

plot(xx,yy)

axis([0,Nmax,0,Tmax]);

a=xx;

b=yy;   

c=Tmax;

end
