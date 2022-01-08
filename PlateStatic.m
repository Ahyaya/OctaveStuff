I0=1200,
epsilon0=0.79,
alpha=0.64,
epsilon1=0.03,
Rtotal=7.41,

qin=linspace(0,alpha*I0,256);
T0=((alpha*I0-qin)/epsilon0/(5.67e-8)).^(0.25);
T1=T0-Rtotal*qin;
T1(find(T1)<0)=0;
func=qin-(5.67e-8)*epsilon1*T1.^4;
pivot=min(find(func>0));
if(pivot<2)
pivot=2;
end
Q1=qin(pivot);
Q0=qin(pivot-1);

qin=linspace(Q0,Q1,256);
T0=((alpha*I0-qin)/epsilon0/(5.67e-8)).^(0.25);
T1=T0-Rtotal*qin;
T1(find(T1)<0)=0;
func=qin-(5.67e-8)*epsilon1*T1.^4;
pivot=min(find(func>0));
if(pivot<2)
pivot=2;
end
Q1=qin(pivot);
Q0=qin(pivot-1);

fqin=interp1(func(pivot-1:pivot),qin(pivot-1:pivot),0);
fT0=((alpha*I0-fqin)/epsilon0/(5.67e-8))^(0.25);
fT1=fT0-Rtotal*fqin;
fT1(find(fT1)<0)=0;

fprintf("T0=%f (%f deg C)\nT1=%f (%f deg C)\nqin=%f\n",fT0,fT0-273.15,fT1,fT1-273.15,fqin)
cos_func=fqin-(5.67e-8)*epsilon1*fT1^4

