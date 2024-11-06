function [WINDOW, OUTDATA]  =  CW_MOV_WIN_AVG(INDATA, WINDOW, WISNSIZE)

for p=WISNSIZE:-1:2
    WINDOW(p)    =  WINDOW(p-1);
end
WINDOW(1)         =  INDATA;
OUTDATA           =  mean(WINDOW);