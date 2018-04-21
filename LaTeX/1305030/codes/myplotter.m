
a = imread('frame1.png');
figure;
imshow(a);

hold on

[x,y] = ginput();
a = insertMarker(a,[x y]);
b = [x y];
%disp(b);

len = length(b);
distance = 0;

for i = 1:len
    d = sprintf('%.3f  %.3f',b(i,1),b(i,2));
    disp(d);
end

for i = 2:len
   a = insertShape(a,'Line',[b(i-1,1) b(i-1,2) b(i,1) b(i,2)],'LineWidth',2,'Color','blue'); 
   % imagesc(a);
   % p = line([b(i,2),b(i-1,2)],[b(i,1),b(i-1,1)],'Color','r','LineWidth',2,'MarkerSize', 100);
   % imshow(p);
   % disp(n);
    xAxis = ((b((i-1),1) - b(i,1)))^2;
    yAxis = ((b((i-1),2) - b(i,2)))^2;
    value = sqrt(xAxis + yAxis);
    distance = distance + value;
end
e = sprintf('Total distance %.3f pixels',distance);
disp(e);

prompt ='Enter the velocity :  ';
velocity = input(prompt);
time = distance / velocity;
f = sprintf('Total approximate time is %.3f seconds',time);
disp(f);
imshow(a);

% Get the pixel values
%data = a(sub2ind(size(a), round(y), round(x)));
%disp(data);