%dhf pfm_residual.m
%dhf 	purpose:  plot up the results of the pfm_residual program.
%dhf 		plots the mean difference bounded by a 95% CL.
%dhf notes:	
%dhf - to use have to remove header and footer from pfm_residual file

filename = "";
filename = input("enter residual filename:  ", filename);

fid = fopen(filename, "r");
[a, count] = fscanf(fid, "%f %f %f %f %f %f %f %f %f", [10, Inf]);
fclose(fid);

bm = a(1,:);
rms = a(2,:);
mean_diff = a(3,:);
std = a(4,:);
std_percent = a(5,:);
neg_percent  = a(6,:);
pos_percent  = a(7,:);
max_depth  = a(8,:);
mean_depth  = a(9,:);
npts  = a(10,:);
%dhf whos

bm = bm + 1;
hold on
grid
gset key off
xlabel("beam number");
ylabel("meters");
titlestr = "";
titlestr = sprintf("residuals [x+-1.96s] - file <%s> - avg(z,n) <%.1f,%.0f>", filename, mean(mean_depth), mean(npts));
title(titlestr);
axis([-5 405 -.4 .4]);
[x1,y1] = bar(bm, mean_diff + 1.96*std);
[x2,y2] = bar(bm, mean_diff - 1.96*std);
plot(x1,y1,"3");
plot(x2,y2,"3");

plot(bm, mean_diff, "@23");
plot(bm, mean_diff, "1");

outfilename = sprintf("%s.png", filename);
gset terminal png color
eval( sprintf("gset output \"%s\"", outfilename) );
replot

gset terminal x11
