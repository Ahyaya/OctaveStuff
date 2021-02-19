//Qe=4.15465;
Qe=2.64798;
pkg load optim
f= @(p,x) Qe*(1-exp(-((p(1)*x).^p(2))))
init=[1;1];
format long
[p,mv,cvg,outp]=nonlin_curvefit (f,init,t,Q);
printf("\tQe=%f\n\tka=%f\n\tna=%f\n",Qe,p(1),p(2));
plot((0:.1:30),Qe*(1-exp(-((p(1)*(0:.1:30)).^p(2)))),"m","LineWidth",3);hold on;
plot(t,Q,'ko');
set(gca,'Box','on','xminortick','on','yminortick','on','TickDir','in','TickLength',[.02 0]);set(gca,'LineWidth',3,'fontsize',18,'fontweight','bold');xlabel('t [min]');ylabel('Qt');
