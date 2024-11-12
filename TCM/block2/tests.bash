((((a+1)/(7*c))*((7+(6*c))/x)) / ((3*(a+1))/(x*x)))
((((x-6)/(6*y))/(((x*x)-(y*y))/y))*(((x*x)+((2*(x*y))+(y*y)))/x))
((((9*(x*x))-4)/((3*x)+2))-(3*x))
(((4*(a*a))-9)*((1/((2*a)-3))-(1/((2*a)+3))))


start:
    rpn:
flowcpp PEG.flow -- exp="(((11 3 -) (2 4 ^) /) ((0 (20 2 /) -) 5 +) *)" -- mode=rpn
flowcpp PEG.flow -- exp="((8 (2 4 ^) /) ((5 2000 *) (3 (2 (3 2 ^) ^) *) -) -)" -- mode=rpn 
    simplifyRational:
flowcpp PEG.flow -- exp="((1 / x) + ((2 / y) + (3 / z)))" -- mode=rat
flowcpp PEG.flow -- exp="((9-a)*(8+(a-b)))" -- mode=rat
flowcpp PEG.flow -- exp="((((a+1)/(7*c))*((7+(6*c))/x)) / ((3*(a+1))/(x*x)))" -- mode=rat
flowcpp PEG.flow -- exp="((1/1000000000)+(1/1000000000))" -- mode=rat
flowcpp PEG.flow -- exp="(((4*(a*a))-9)*((1/((2*a)-3))-(1/((2*a)+3))))" -- mode=rat
    dif:
flowcpp PEG.flow -- exp="((y / x) - (z + 1))" -- mode=dif
    sim:
flowcpp PEG.flow -- exp="(((3^1)-(2*0))+(6-0))" -- mode=sim


