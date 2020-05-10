%%DHT11

dht11 = tf([0.0994252],[1 0.0994252]);
figure;
t = 0:0.01:20;
vout = 1 - exp(-0.0994252*t);
%plot(t,vout);
step(dht11);
