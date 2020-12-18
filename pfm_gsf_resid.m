%dhf pfm_gsf_resid.m
%dhf 	purpose:  plot up the results of the pfm_residual program.
%dhf 		plots the mean difference bounded by a 95% CL.
%dhf notes:	
%dhf - to use have to remove header and footer from pfm_gsf_resid file

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

order = 0;
order = input("enter desired iho order: ");
[hor ver cvg det spa] = iho(grand_mean, order);

hold on
grid
%dhf gset key off
xlabel("-beam_angle");
ylabel("meters");
titlestr = "";
if ( bias == "y" )
	titlestr = sprintf("depth residuals [(z-e)+-1.96s] <o%.0f> - file <%s>\navg(z,n,e) <%.1f,%.0f,%.3f>",
		order, filename, grand_mean, mean(npts), grand_mean_diff);
	chunk = mean_diff - grand_mean_diff;
else
	titlestr = sprintf("depth residuals [z+-1.96s] <o%.0f> - file <%s>\navg(z,n,e) <%.1f,%.0f,%.3f>",
		order, filename, grand_mean, mean(npts), grand_mean_diff);
	chunk = mean_diff;
endif
title(titlestr);

%dhf axis([-5 405 -.4 .4]);
w = 1;
[x1,y1] = bar(ang, chunk + 1.96*stdev, w, "stacked");
[x2,y2] = bar(ang, chunk - 1.96*stdev, w, "stacked");
plot(x1,y1,"3");
plot(x2,y2,"3");

plot(ang, chunk, "*2"); %dhf star,green
plot(ang, chunk, "r");	%dhf red line

newver = ones(size(ang))*ver;
plot(ang, newver, "1");
plot(ang, -newver, "1");

%dhf outfilename = sprintf("%s.png", filename);
%dhf eval( sprintf("print -dpng -color '-S640,480' %s", outfilename) );

outfilename = sprintf("%s.gif", filename);
eval( sprintf("print -dgif -color '-FTimes-Roman' %s", outfilename) );

