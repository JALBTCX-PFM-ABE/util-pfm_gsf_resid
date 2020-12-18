%dhf pfm_gsf_resid_conf.m
%dhf 	purpose:  plot up the results of the pfm_residual program.
%dhf 		plots the mean difference bounded by a 95% CL.
%dhf  added confidence option
%dhf notes:	
%dhf - to use have to remove header and footer from pfm_gsf_resid file
%dhf		new plotting using errorbar - 18 aug 2010

filename = "";
filename = input("enter residual filename:  ", filename);

fid = fopen(filename, "r");
[a, count] = fscanf(fid, "%f %f %f %f %f %f %f %f %f", [10, Inf]);
fclose(fid);

ang = a(1,:);
rms = a(2,:);
mean_diff = a(3,:);
stdev = a(4,:);
std_percent = a(5,:);
neg_percent  = a(6,:);
pos_percent  = a(7,:);
max_depth  = a(8,:);
mean_depth  = a(9,:);
npts  = a(10,:);
%dhf whos

%dhf - don't use undersampled angles
i0 = find( npts > std(npts)/4 );
ang = -ang(i0);
mean_diff = mean_diff(i0);
stdev = stdev(i0);

grand_mean = mean(mean_depth);
grand_mean_diff = mean(mean_diff)
bias = "";
bias = input("should the grand mean bias be applied?  (y/n):  ", bias);

conf = 1;
conf = input("enter desired confidence level as a mult of stdev (1, 1.96, 2): ");

order = 0;
order = input("enter desired iho5 order (0, 1.1, 1.2, 2): ");
[hor ver cvg det spa] = iho5(grand_mean, order);
ihover = ver/1.96; %convert to unity, 1-sigma
ihover *= conf; %convert to input cl

hold on
grid
%dhf gset key off
xlabel("-beam angle");
ylabel("meters");
titlestr = "";
if ( bias == "y" )
	titlestr = sprintf("depth residual: (d-e),(%.2f-sigma),<o%.0f>,<%s>\navg(z,n,e)=<%.1f,%.0f,%.3f>",
		conf, order, filename, grand_mean, mean(npts), grand_mean_diff);
	chunk = mean_diff - grand_mean_diff;
else
	titlestr = sprintf("depth residual: (%.2f-sigma),<o%.0f>,<%s>\navg(z,n,e)=<%.1f,%.0f,%.3f>",
		conf, order, filename, grand_mean, mean(npts), grand_mean_diff);
	chunk = mean_diff;
endif
title(titlestr);

%dhf axis([-5 405 -.4 .4]);
%dhf w = 1;
%dhf [x1,y1] = bar(ang, chunk + 1.96*stdev, w, "stacked");
%dhf [x2,y2] = bar(ang, chunk - 1.96*stdev, w, "stacked");
%dhf plot(x1,y1,"3");
%dhf plot(x2,y2,"3");

error =  conf*stdev;
minx = 1.15*min(ang);
maxx = 1.15*max(ang);
miny = 1.15*min( [ chunk.-error -ihover ] );
maxy = 1.15*max( [ chunk.+error ihover ] );

axis([minx maxx miny maxy]); %dhf - set axes

errorbar(ang, chunk, error); %dhf - a cool replacement for bar

plot(ang, chunk, "*g"); %dhf star,green
plot(ang, chunk, "r");	%dhf cyan

newver = ones(size(ang))*ihover;
plot(ang, newver, "r");
plot(ang, -newver, "r");

outfilename = sprintf("%s.gif", filename);
eval( sprintf("print -dgif -color '-FTimes-Roman' %s", outfilename) );

