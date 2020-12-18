%dhf pgr_perdep.m
%dhf 	purpose:  plot up the results of the pfm_residual program.
%dhf 		plots the mean difference bounded by a 95% CL.
%dhf 		plotting as percentage of depth.
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
ang = -ang(i0); %dhf - invert ang for plotting
mean_diff = mean_diff(i0);
stdev = stdev(i0);
std_percent = std_percent(i0);
mean_depth = mean_depth(i0);
grand_mean = mean(mean_depth);
grand_mean_diff = mean(mean_diff)
bias='';
bias = input("should grand mean bias be applied (y/n):  ", bias);
titlestr = "";
if ( bias == 'y' )
	pd100 = ( (mean_diff-grand_mean_diff)./mean_depth)*100;
	titlestr = sprintf("%%depth-e (1-sigma) - file <%s>\navg(z,n,e) <%.1f,%.0f,%.3f>", filename, grand_mean, mean(npts),  grand_mean_diff);
else
	pd100 = (mean_diff./mean_depth)*100;
	titlestr = sprintf("%%depth (1-sigma) - file <%s>\navg(z,n,e) <%.1f,%.0f,%.3f>", filename, grand_mean, mean(npts),  grand_mean_diff);
endif

title(titlestr);

hold on
grid
%dhf gset key off
xlabel("-beam_angle");
%dhf ylabel("meters");
%dhf axis([-80 80 -.7 .7]); %dhf - for sequences at same scale

w = 1;
[x1,y1] = bar(ang, pd100 + std_percent, w, "stacked");
[x2,y2] = bar(ang, pd100 - std_percent, w, "stacked");
plot(x1,y1,"3");
plot(x2,y2,"3");

plot(ang, pd100, "*2"); %dhf star, green
plot(ang, pd100, "r"); %dhf red line

%dhf outfilename = sprintf("%s_pd.png", filename);
%dhf eval( sprintf("print -dpng -color '-S640,480' %s", outfilename) );

outfilename = sprintf("%s_pd.gif", filename);
eval( sprintf("print -dgif -color '-FTimes-Roman' %s", outfilename) );
