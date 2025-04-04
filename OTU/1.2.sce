clc,clear;

A = [3 2 -5; 4 2 0; 1 1 2];
B = [-1 2 4; 0 3 2; -1 -3 4];

D = (A - B)*(A * A) + 3 * B;
disp("матрица D:"); disp(D);
disp("det(D):"); disp(det(D));

D_inv = inv(D);
disp("Обратная матриц D^-1:"); disp(D_inv);

check = D*D_inv
disp("D*D_inv:"); disp(check);
