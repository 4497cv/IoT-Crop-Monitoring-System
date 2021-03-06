close all; clc;

%% Sensor de temperatura LM35 
% Caracter�sticas Est�ticas
k = 10e-3;                                           % sensibilidad  
T = 2:1:150;                                          % temperatura
Vout = k*T;
%Acondicionamiento I: amplificador diferencial
Vout_ad = 2.229729*(Vout-0.02);
% Funci�n de transferencia
lm35 = tf([0.02049],[1 0.02049]);
% Respuesta en el tiempo
t = 0:0.1:500;
Vo = 1 - 0.8*exp(-0.02049*t);
% Gr�ficas de la respuesta al escal�n y respuesta en frecuencia
figure; 
subplot(2,1,1);
plot(T,Vout,'-r');
title('Gr�fica de la recta del sensor de temperatura con rango [0.02, 1.5] V');
xlabel('Temperatura (�C)');
ylabel('Voltaje de salida (Vout)');
axis([2 150 0.02 1.5]);
subplot(2,1,2);
plot(T,Vout_ad,'-r');
title('Gr�fica de la recta del sensor con rango [0, 3.3] V');
xlabel('Temperatura (�C)');
ylabel('Voltaje de salida (Vout)');
axis([2 150 0 3.3]);
figure;
subplot(2,1,1);
step(lm35,'-black');
title('Respuesta al escal�n del sensor LM35');
ylabel('Amplitud');
xlabel('Tiempo (segundos)');
subplot(2,1,2);
bodemag(lm35,'-b');
title('Respuesta en frecuencia del sensor LM35');
xlabel('Frecuencia (rad/s)');
ylabel('Magnitud (dB)');


%% Sensor de humedad de suelo

%% Sensor de humedad relativa DHT11

% Funci�n de transferencia
dht11 = tf([0.0994252],[1 0.0994252]);

% Gr�ficas de la respuesta al escal�n y respuesta en frecuencia
figure; 
subplot(2,1,1);
step(dht11);
title('Respuesta al escal�n del sensor DHT11');
xlabel('Amplitud');
ylabel('Tiempo (segundos)');
subplot(2,1,2);
bodemag(dht11);
title('Respuesta en frecuencia del sensor DHT11');
xlabel('Frecuencia (rad/s)');
ylabel('Magnitud (dB)');

%% Sensor de luminosidad
R10 = 2.072e3;
R100 = 670;
gamma = log(R10/R100);
Vin = 5;
RTD = [2.072e3: -1: 670];
lux = [0:1/14.02:100];
R = 1e3;
% Puente de wheatstone linealizado
Vo = (-Vin*(RTD/(2*R)));
Vo = Vo+2.59;
% Amplificador diferencial
k = (3.3-0)/(-2.59-0.9125)
Vo_2 = k*(Vo-0.9125);
figure;
subplot(2,1,1);
plot(RTD,Vo,'-black');
title('Gr�fica Vo vs R');
ylabel('Voltaje de salida');
xlabel('Resistencia (ohm)');
xlim([670 2.072e3])
subplot(2,1,2);
plot(RTD,Vo_2,'-r');
title('Gr�fica Vo vs R');
ylabel('Voltaje de salida');
xlabel('Resistencia (ohm)');
xlim([670 2.072e3])
ylim([0 3.3])


%plot(t,vout);

