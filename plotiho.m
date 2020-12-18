order = input('enter order in (0,1,2,3): ');
depth = input('enter deepest depth of interest: ');
hor = zeros(1,depth-1);
ver = zeros(1,depth-1);
cov = zeros(1,depth-1);
det = zeros(1,depth-1);
spa = zeros(1,depth-1);
for i=1:depth-1
   [hor(1,i) ver(1,i) cov(1,i) det(1,i) spa(1,i)]=iho(i+1,order);
endfor
title(sprintf("s-44,4 order=%d", order));
xlabel("depth (m)");
ylabel("max error (m)");
%dhf gset key left
grid
hold on
axis([0 40 0 2.1])
plot(hor, "*4");
plot(ver, "+3");
%dhf plot(cov, ";cov;");
%dhf plot(det, ";det;");
%dhf plot(spa, ";spa;");

filename = sprintf("order%d.png", order);
eval( sprintf("print -dpng '-S640,480' %s", filename) );
