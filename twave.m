%Usage: 
%[h0, h1, amp_forward, amp_backward] = HeatBarrier (Effusity, ThermalDepth, Frequency, Effusity_ambient);
%
% h0 = extinct rate from surface Temperature to back side temperature
% h1 = extinct rate from surface flux to back side temperature
%
%Effusity = sqrt(C*rho*lambda), which is intrinsic property of the materials, e.g. Effusity = 22455 for Aluminum plate, more detail please refers https://thermaleffusivity.com/thermaleffusivityvsthermaleffusance/
%
%ThermalDepth = Thickness / sqrt(kappa), where kappa is the diffusion coeffiecent, e.g. kappa = 8.17e-5 for Aluminum plate.
%
%Frequency is the frequency of the input thermal signal, e.g. Frequency = 1e-4 for 0.1mHz disturbance.
%
%Effusity_ambient refers to the effusity of the backside media, e.g. Effusity_ambient = 0 for vaccum condition.
%
%Extinct as output represents the complex amplification attenuation from the input signal at frontside to the temperature fluctuation at backside.

function [extinct_surfTemperature,extinct_Irr2Temperature,amp_forward, amp_backward]=HeatBarrier(data_mu,data_TD,freq,mu_ambient)
if nargin<4
mu_ambient=0;
end
if length(data_mu)~=length(data_TD)
disp('incompatibale input');
return;
end
nlayers=length(data_mu);
amp_forward=zeros(nlayers+1,1);
amp_backward=zeros(nlayers+1,1);
SRF=sqrt(pi*freq);
An=1;
Bn=0;
data_mu(nlayers+1)=mu_ambient;
for pf=nlayers:-1:1
newAn=exp((1+i)*SRF*data_TD(pf))/2.*((1+data_mu(pf+1)/data_mu(pf))*An+(1-data_mu(pf+1)/data_mu(pf))*Bn);
newBn=exp(-(1+i)*SRF*data_TD(pf))/2.*((1-data_mu(pf+1)/data_mu(pf))*An+(1+data_mu(pf+1)/data_mu(pf))*Bn);
An=newAn;Bn=newBn;
amp_forward(pf)=An;amp_backward(pf)=Bn;
end
extinct_surfTemperature=1./(An+Bn);
extinct_Irr2Temperature=1./(data_mu(1)*exp(pi/4*i)*SRF.*(An-Bn));
amp_forward(nlayers+1)=1;amp_backward(nlayers+1)=0;
end
