clc; clear;

function r = f(fi)
    r = 2 ./ sin(fi) + 3;
endfunction
fi = linspace(0.01, 2*%pi-0.01, 100);
ro=f(fi)

polarplot(fi,ro,style=color("red"));
