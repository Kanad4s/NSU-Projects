clc; clear; 

exec("mSolution.sci")

A = [20 5 0 5; 1 -3 4 0; 0 3 -2 -4; 1 2 -1 3];
b = [-9; -7; 12; 10];
[x, status, residual] = system4x4(A, b);

if status == 0 then
    disp("Решение:"); disp(x);
    disp("Невязка:"); disp(residual);
elseif status == 1 then
    disp("Система имеет бесконечно много решений");
    disp("Частное решение:"); disp(x);
else
    disp("Система несовместна");
end
