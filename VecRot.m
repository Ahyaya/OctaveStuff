%Rodrigues rotation function 
%Usage:
%  new_position = VecRot ( position, axis_rotation)
%  new_position = VecRot ( position, axis_rotation, angle_rotation)
%
%  e.g.
%  position = [1 sqrt(3) 0];
%  axis_rotation = [0 0 1];
%  angle_rotation = pi/2;
%
%The angle_rotation is optional, it uses axis_rotation's module when unspecified.
% 

function new_position=VecRot(position,axis_rotation,angle_rotation)
axis_module=sqrt(sum(axis_rotation(1:3).^2));
if nargin<3
angle_rotation=axis_module;
fprintf("Rotation angle unspecified, rotation axis module is used\nAngle=%.2f (degree)\n",angle_rotation/pi*180);
end
axis_rotation=axis_rotation/axis_module;

C=cos(angle_rotation);S=sin(angle_rotation);x=axis_rotation(1);y=axis_rotation(2);z=axis_rotation(3);

new_position=[C+x*x*(1-C) x*y*(1-C)-z*S y*S+x*z*(1-C);z*S+x*y*(1-C) C+y*y*(1-C) -x*S+y*z*(1-C);-y*S+x*z*(1-C) x*S+y*z*(1-C) C+z*z*(1-C)]*[position(1);position(2);position(3)];

end
