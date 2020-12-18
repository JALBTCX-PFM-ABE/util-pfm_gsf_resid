order = input('enter 5th ed order in (0,1.1,1.2,2): ');
depth = input('enter deepest depth of interest: ');
hor = zeros(1,depth-1);
ver = zeros(1,depth-1);
cov = zeros(1,depth-1);
det = zeros(1,depth-1);
spa = zeros(1,depth-1);
for i=1:depth-1
   [hor(1,i) ver(1,i) cov(1,i) det(1,i) spa(1,i)]=iho5(i+1,order);
endfor
title(sprintf("s-44,5 order=%.1f, spa=%.0f, det=%.0f", order, spa(1,1), det(1,1)));
xlabel("depth (m)");
ylabel("max error (m)");
%dhf gset key left
grid
hold on
maxh = 1.1*max(hor);
maxv = 1.1*max(ver);
maxhv = max([maxh maxv]);
maxz = 1.1*depth;
axis([0 maxz 0 maxhv])
plot(hor, "2;hor;");
plot(ver, "3;ver;");
%dhf plot(hor, "*4");
%dhf plot(ver, "+3");
%dhf plot(cov, ";cov;");
%dhf plot(det, ";det;");
%dhf plot(spa, ";spa;");

filename = sprintf("order%.1f.gif", order);
eval( sprintf("print -dgif -color '-FTimes-Roman' %s", filename) );
