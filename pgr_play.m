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
whos

i0 = find( npts > std(npts)/4 );

ang0 = ang(i0);
mean_diff0 = mean_diff(i0);
stdev0 = stdev(i0);

hold on
plot(-ang0, mean_diff0, "@12");
plot(-ang0, mean_diff0+1.96*stdev0, "@23");
plot(-ang0, mean_diff0-1.96*stdev0, "@23");





