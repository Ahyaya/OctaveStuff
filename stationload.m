function d=stationload(filename)

FID=fopen(filename);
fgetl(FID)
d=fscanf(FID,'%d-%d-%dT%d:%d:%dZ%f%f%f%f%f%f%f%f%f%f',[16,Inf]);
fclose(FID);
d=d([(7:12),(14:16)],:)';
end
