clc; clear;

function y = f(x)
    y = (x.^2 .* (x + 35).^2).^(1/3);
endfunction

x = linspace(-50, 50, 1000);

y = f(x);

scf();
plot(x, y, 'b-', 'LineWidth', 2); 

title('f(x) = 3∛[x²(x+35)²]', 'fontsize', 3);
xlabel('x', 'fontsize', 3);
ylabel('f(x)', 'fontsize', 3);
xgrid();

x_roots = [-35, 0];  // Корни функции
y_roots = f(x_roots);
plot(x_roots, y_roots, 'ro', 'MarkerSize', 8);

legend(['f(x) = ∛[x²(x+35)²]'; 'Корни функции'], 2);

