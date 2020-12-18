function [ hor, ver, cov, det, spa ] = iho( depth, order )

% function [ hor, ver, cov, det, spa ] = iho( depth, order )
% inputs:	depth - water depth
%		order - 0(=special),1,2,3
% outputs: 	hor - horizontal accuracy required at 95% ci 
%		ver - vertical accuracy required at 95% ci
%		cov - coverage requirement in 100*percent
%		det - minimum cube detection size required
%		spa - maximum line spacing
% reference:	iho special publication 44 (1998)

if ( order == 0 )
	hor = 2;
	a = 0.25; b = 0.0075; f = depth*b; ver = sqrt( a*a + f*f );
	cov = 100;
	det = 1;
	spa = 0;
end
if ( order == 1 )
	hor = 5 + 0.05*depth;
	a = 0.5; b = 0.013; f = depth*b; ver = sqrt( a*a + f*f );
	cov = 100;
	if ( depth <= 40 )
		det = 2;
	else
		det = 0.1*depth;
	end
	if ( 3*depth > 25 )
		spa = 3*depth;
	else
		spa = 25;
	end
end
if ( order == 2 )
	hor = 20 + 0.05*depth;
	a = 1.0; b = 0.023; f = depth*b; ver = sqrt( a*a + f*f );
	cov = 100;
	if ( depth <= 40 )
		det = 2;
	else
		det = 0.1*depth;
	end
	if ( 3*depth > 200 )
		spa = 3*depth;
	else
		spa = 200;
	end
end
if ( order == 3 )
	hor = 150 + 0.05*depth;
	a = 1.0; b = 0.023; f = depth*b; ver = sqrt( a*a + f*f );
	cov = 0;
	det = 0;
	spa = 4*depth;
end
