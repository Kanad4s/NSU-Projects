clc; clear;

t = linspace(0, 4*%pi, 80);
X = sin(t);
Y = sin(2*t);
Z = t/5;

scf();
param3d1(X, Y, Z);

title("Параметрическая 3D кривая", "fontsize", 3);
xlabel("X = cos(t)", "fontsize", 2);
ylabel("Y = cos(2t)", "fontsize", 2);
zlabel("Z = sin(t)", "fontsize", 2);

a = gca();
a.rotation_angles = [60, 45];  // Угол обзора [θ, φ]
a.grid = [1 1 1];             // Включение сетки
a.box = "on";                 // Рамка вокруг графика

gcf().color_map = jetcolormap(64);

xs2png(gcf(), '3d_parame_t_1.png');
