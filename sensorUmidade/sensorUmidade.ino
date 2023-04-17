const int pinoSensorUmidade = A0;
void setup() {
  Serial.begin(9600);
  pinMode(pinoSensorUmidade,INPUT);
  // put your setup code here, to run once:

}

void loop() {
  Serial.println(analogRead(pinoSensorUmidade));
  delay(1000);
  // put your main code here, to run repeatedly:

}
