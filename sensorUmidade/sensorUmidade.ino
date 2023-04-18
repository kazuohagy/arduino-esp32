const int pinoSensorUmidade = A0;
void setup() {
  Serial.begin(9600);
  pinMode(pinoSensorUmidade,INPUT);
  // put your setup code here, to run once:

}

void loop() {
  Serial.println(analogRead(pinoSensorUmidade));
  if(analogRead(pinoSensorUmidade) > 200){
    Serial.println("Precisa de agua");
  }
  delay(1000);
  // put your main code here, to run repeatedly:

}
