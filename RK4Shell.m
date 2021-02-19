X=zeros(4096,1);Z=zeros(4096,1);V=zeros(4096,1);dt=0.01;g=9.8;var_drag=5e-5;v_0=815;theta_degree=15;
theta=theta_degree/180*pi;
x=0;z=0;
vx=v_0*cos(theta);
vz=v_0*sin(theta);
v=v_0;
S=[x,z,vx,vz];

function fK=Dfunc(fS)
g=9.8;var_drag=5e-5;fv=sqrt(fS(3)*fS(3)+fS(4)*fS(4));
dx=fS(3);
dz=fS(4);
ddx=-var_drag*fv*fS(3);
ddz=-var_drag*fv*fS(4)-g;
fK=[dx,dz,ddx,ddz];
end

for pf=1:4096
K1=Dfunc(S);
K2=Dfunc(S+0.5*dt*K1);
K3=Dfunc(S+0.5*dt*K2);
K4=Dfunc(S+dt*K3);
S=S+(K1+2*K2+2*K3+K4)*dt/6;
x=S(1);z=S(2);vx=S(3);vz=S(4);

v=sqrt(vx*vx+vz*vz);
X(pf)=x;Z(pf)=z;V(pf)=v;

if(z<1e-3)
break;
end
end
if pf<length(X)
X(pf+1:end)=[];Z(pf+1:end)=[];V(pf+1:end)=[];
end
plot(X,Z);
