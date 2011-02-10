function [out]= fr2vid(X,counter)

Filename = 'fr_vid.avi';
FPS = 3;


aviobj = avifile(Filename,'fps',FPS,'compression','None');

fig=figure;

for i = 1:counter-1
    
imshow(uint8(X(:,:,counter-i)));

axis equal

frame = getframe(fig);

 aviobj = addframe(aviobj,frame);    
 
end

aviobj = close(aviobj);

out=X;

end
