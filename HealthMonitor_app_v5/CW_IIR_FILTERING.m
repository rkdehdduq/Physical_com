function [dX, dY]  =   CW_IIR_FILTERING(b_i, a_i, dX, dY, INDATA)

for p=6:-1:2
    dX(p)       =  dX(p-1);
    dY(p)       =  dY(p-1);
end

dX(1)           =  INDATA;
dY(1)           =  (b_i(1)*dX(1)+b_i(2)*dX(2)+b_i(3)*dX(3)+b_i(4)*dX(4)+b_i(5)*dX(5)+b_i(6)*dX(6)-...
                    a_i(2)*dY(2)-a_i(3)*dY(3)-a_i(4)*dY(4)-a_i(5)*dY(5)-a_i(6)*dY(6))/a_i(1);