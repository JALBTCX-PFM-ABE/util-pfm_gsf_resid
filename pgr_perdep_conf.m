%dhf pgr_perdep_conf.m
%dhf 	purpose:  plot up the results of the pfm_residual program.
%dhf 		plots the mean difference bounded by a 95% CL.
%dhf 		plotting as percentage of depth.
%dhf notes:	
%dhf - to use have to remove header and footer from pfm_gsf_resid file
%dhf - 		new errorbar plotting - 18 aug 2010 

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

conf = 1;
conf = input("enter desired vertical confidence level as a mult of stdev (1,1.96,2,3,...): ");

%dhf - for sue/clay paper only (i disagree with bars on a %depth plot)
order = 0;
order = input("enter desired iho5 order (0, 1.1, 1.2, 2): ");
[hor ver cvg det spa] = iho5(grand_mean, order);
ver
ihover = ver/1.96/grand_mean*100 % convert from 95 percent to 1-sigma
ihover *= conf

titlestr = "";
if ( bias == 'y' ) %dhf - to grand_mean application
	pd100 = ( (mean_diff-grand_mean_diff)./mean_depth)*100;
	titlestr = sprintf("%%depth residual: %%(d-e),(%.2f-sigma),(o%.0f),<%s>\navg(z,n,e)=<%.1f,%.0f,%.3f>", conf, order, filename, grand_mean, mean(npts),  grand_mean_diff);
	%dhf titlestr = sprintf("%%(depth residual-e) (1-sigma) - file <%s>\navg(z,n,e)=<%.1f,%.0f,%.3f>", filename, grand_mean, mean(npts),  grand_mean_diff);
else
	pd100 = (mean_diff./mean_depth)*100;
	titlestr = sprintf("%%depth residual: (%.2f-sigma),(o%.0f),<%s>\navg(z,n,e)=<%.1f,%.0f,%.3f>", conf, order, filename, grand_mean, mean(npts),  grand_mean_diff);
	%dhf titlestr = sprintf("%%depth (1-sigma) - file <%s>\navg(z,n,e)=<%.1f,%.0f,%.3f>", filename, grand_mean, mean(npts),  grand_mean_diff);
endif

title(titlestr);

hold on
grid
xlabel("-beam angle");
%dhf ylabel("meters");

std_percent *= conf;
%dhf - errorbar doesn't cover bars, so give it 15%
minx = 1.15*min(ang);
maxx = 1.15*max(ang);
miny = 1.15*min( [ min(pd100.-std_percent) -ihover ] );
maxy = 1.15*max( [ max(pd100.+std_percent) ihover ] );
axis([minx maxx miny maxy]); %dhf - set axes

%dhf - a simpler, more appropriate call? needed axes set above
errorbar(ang, pd100, std_percent, "#");

%dhf w = 1;
%dhf [x1,y1] = bar(ang, pd100 + std_percent, w, "stacked");
%dhf [x2,y2] = bar(ang, pd100 - std_percent, w, "stacked");
%dhf plot(x1,y1,"b");
%dhf plot(x2,y2,"b");

plot(ang, pd100, "*g");
plot(ang, pd100, "r");

newver = ones(size(ang))*ihover;
plot(ang, -newver, "r"); %dhf
plot(ang, newver, "r"); %dhf

outfilename = sprintf("%s_pd.gif", filename);
eval( sprintf("print -dgif -color '-FTimes-Roman' %s", outfilename) );

