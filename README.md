# symafpv_rcswitch
RC controlled switch to start/stop video recording on Syma X5C 5.8G FPV kit using any RC transmitter/receiver

Using an Arduino Nano or Pro Mini(5v) and a NPN transistor as switch.

Arduino reads PWM values from a RC channel, starting the video recording when PWM>1900 and stoping when PWM<1100. 
You can assign a transmitter switch to a channel to start or stop recording.
