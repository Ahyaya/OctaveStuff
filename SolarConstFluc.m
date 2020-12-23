Obj_waitbar=waitbar(0,'Initializing...');

fseg=(1e-6:1e-6:5e-4)';
f=(1e-6:1e-6:5e-2)';
t=(0:30:1e6);
df=f(2)-f(1);
fluc=zeros(size(t));

for pf=0:99
f=pf*fseg(end)+fseg;
fluc+=sqrt(2)*df*sqrt(t(end))*sum(0.175*f.^(-1/3).*cos(2*pi*(rand(length(f),1)+repmat(f,[1 length(t)]).*repmat(t,[length(f) 1]))));
waitbar(pf/100,Obj_waitbar,['Progress: ',num2str(floor(pf)),'%']);
end

f=(1e-6:1e-6:5e-2)';
refPSD=0.175*f.^(-1/3);

PSDplot(fluc,30);hold on;
plot(f,refPSD,'k.');

save fluc_out fluc;

close(Obj_waitbar);					%Close waitbar
disp('Computation Done, Ctrl-D to quit.');

