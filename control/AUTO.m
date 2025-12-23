
Ts=0.001;
C= pid(1,0,0)
[num , den] = tfdata(C);
[num , den]= c2dm(num,den,Ts,'tustin')
Control_z= filt(num,den,Ts)
[num , den] = tfdata(Control_z)