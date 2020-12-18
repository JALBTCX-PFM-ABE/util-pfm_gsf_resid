filename = "";
filename = input("enter filename:  ", filename);

fid = fopen(filename, "r");
[a, count] = fscanf(fid, "%f %f %f %f", [4, Inf]);
fclose(fid);

ifile = a(1,:);
bm = a(2,:);
xtr = a(3,:);
ang = a(4,:);
clear a;

grid on
title(filename);
xlabel("acrosstrack");
ylabel("absolute beam angle");

plot(xtr, abs(ang), ".");

outfilename = sprintf("%s.png", filename);
gset terminal png color
eval( sprintf("gset output \"%s\"", outfilename) );
replot

gset terminal x11
