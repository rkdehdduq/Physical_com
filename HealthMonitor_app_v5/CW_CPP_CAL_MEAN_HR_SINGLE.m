function [MEAN_HR]  =  CW_CPP_CAL_MEAN_HR_SINGLE(F_BCG_INT, TH)

if length(F_BCG_INT) >= TH
    MEAN_HR =    60./(mean(F_BCG_INT));
else
    MEAN_HR =    single(0);
end
