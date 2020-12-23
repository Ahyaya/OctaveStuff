#Linear interpolation for time series of a 2D map--M(1:pixel_x, 1:pixel_y, 1:pixel_t) with time grid t(1:pixel_t)
#M=interpts(t,M,ts) will return a map in size of M(1:pixel_x, 1:pixel_y).
#Time vector must have monotonously increasing value.

function Mout=interpts(t,M,ti)

bnd_0=find(t<ti)(end);
if (bnd_0+1>length(t))
Mout=M(:,:,bnd_0+1);
return;
else
Mout=((t(bnd_0+1)-ti)*M(:,:,bnd_0)+(ti-t(bnd_0))*M(:,:,bnd_0+1))/(t(bnd_0+1)-t(bnd_0));
endif

endfunction