//u = linspace(-%pi/2,%pi/2,40);
//v = linspace(0,2*%pi,20);
u = linspace(0,2*%pi/2,40);
v = linspace(0.01,5*%pi,40);
[U,V] = ndgrid(u,v);
X = cos(U) .* U .* (1 + cos(V)/2);
Y = (U/2) .* sin(V);
Z = (sin(U) .* U) .* (1 + cos(V/2));
plot3d2(X,Y,Z);

xs2png(gcf(), '3d2_param_u_v.png');
