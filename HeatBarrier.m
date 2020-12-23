%Usage: Extinct = HeatBarrier(Effusity,ThermalDepth,Frequency,Effusity_ambient);
%
%Effusity = sqrt(C*rho*lambda), which is intrinsic property of the materials, e.g. Effusity = 22455 for Aluminum plate, more detail please refers https://thermaleffusivity.com/thermaleffusivityvsthermaleffusance/
%
%ThermalDepth = Thickness / sqrt(2*kappa), where kappa is the diffusion coeffiecent, e.g. kappa = 8.17e-5 for Aluminum plate.
%
%Frequency is the frequency of the input thermal signal, e.g. Frequency = 1e-4 for 0.1mHz disturbance.
%
%Effusity_ambient refers to the effusity of the backside media, e.g. Effusity_ambient = 0 for vaccum condition.
%
%Extinct as output represents the complex amplification attenuation from the input signal at frontside to the temperature fluctuation at backside.

function extinct_Irr2Temperature=HeatBarrier(data_mu,data_TD,freq,mu_ambient);
if nargin<4
mu_ambient=0;
end
if length(data_mu)~=length(data_TD)
disp('incompatibale input');
return;
end
SRF=sqrt(2*pi*freq);
An=(1+mu_ambient/data_mu(end))/2.*exp((1+i)*SRF*data_TD(end));
Bn=(1-mu_ambient/data_mu(end))/2.*exp(-(1+i)*SRF*data_TD(end));
for pf=length(data_mu)-1:-1:1
newAn=exp((1+i)*SRF*data_TD(pf))/2.*((1+data_mu(pf+1)/data_mu(pf))*An+(1-data_mu(pf+1)/data_mu(pf))*Bn);
newBn=exp(-(1+i)*SRF*data_TD(pf))/2.*((1-data_mu(pf+1)/data_mu(pf))*An+(1+data_mu(pf+1)/data_mu(pf))*Bn);
An=newAn;Bn=newBn;
end
%extinct_surfTemperature=1./(An+Bn);
extinct_Irr2Temperature=1./(data_mu(1)*exp(pi/4*i)*SRF.*(An-Bn));
end
