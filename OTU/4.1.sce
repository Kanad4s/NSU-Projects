clc;
V=[6 0 -18 0 -1];
p=poly(V,"x","c");
r=roots(p);
disp("Полином:"); disp(p);
disp("Корни:"); disp(r);

V=[1.3 0.94 -0.08 2];
p=poly(V,"x","c");
r=roots(p);
disp("Полином:"); disp(p);
disp("Корни:"); disp(r);
