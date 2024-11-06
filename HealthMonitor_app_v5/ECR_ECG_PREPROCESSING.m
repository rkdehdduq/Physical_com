function [SMBCG, f_value, dX_h_1, dY_h_1, dX_l_30, dY_l_30, Window1, Window2, Window3, pre_value] =  ECR_ECG_PREPROCESSING...
    (INDATA, Window1, Window2, Window3, b_h_1, a_h_1, b_l_30, a_l_30, dX_h_1, dY_h_1, dX_l_30, dY_l_30, len1, len2, len3, pre_value)

[dX_h_1, dY_h_1]    =   CW_IIR_FILTERING(b_h_1, a_h_1, dX_h_1, dY_h_1, INDATA);
[dX_l_30, dY_l_30]  =   CW_IIR_FILTERING(b_l_30, a_l_30, dX_l_30, dY_l_30, dY_h_1(1));


absDATA             =  single(abs(dY_l_30(1) - pre_value));

pre_value = dY_l_30(1);

f_value = dY_l_30(1);

[Window1, tmp]      =  CW_MOV_WIN_AVG(absDATA, Window1, len1);
[Window2, tmp]      =  CW_MOV_WIN_AVG(tmp, Window2, len2);
[Window3, tmp]      =  CW_MOV_WIN_AVG(tmp, Window3, len3);

SMBCG               =   tmp;



