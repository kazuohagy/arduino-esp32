#include "Firebase_ESP_Client.h"
#include "addons/TokenHelper.h"
#include "time.h"

//info do Firestore
#define FIREBASE_HOST "https://gaia-25c62-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "G5HN59JZValyPF813J4zc6kzI5Zit6zOiN6C1o71"

#define API_KEY "AIzaSyCjgPe0vmY7thIgBubGp45BflEzsq98CYA"
#define FIREBASE_PROJECT_ID "gaia-25c62"
#define USER_EMAIL "mario@gmail.com"
#define USER_PASSWORD "ajudar123"

//Info da rede Wifi
#define WIFI_SSID "Kazuo_EXT"
#define WIFI_PASSWORD "12345678w"

//Info do Rele
#define RELAY_PIN 15

FirebaseData fbdo; // Cria um objeto para armazenar os dados do Firebase
FirebaseAuth auth;
FirebaseConfig config;

const int pinoSensorUmidade = A0;
float percentHumidity;
float humidity;
long tempoInicial = 0;
long tempoFinal = 0;
unsigned long tempoDecorrido;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -5400;
const int   daylightOffset_sec = -5400;

String dataNow;
String dateMonth;
String dateYear;
String dateDay;
char buffer[50];  // Array de caracteres para armazenar a string formatada
char bufferMes[20];
char ano[2];
char dia[2];

void retornaMes(char* mes){
 if (strcmp("January", mes) == 0) {
    dateMonth = "01";
  }
  else if (strcmp("February", mes) == 0) {
    dateMonth = "02";
  }
  else if (strcmp("March", mes) == 0) {
    dateMonth = "03";
  }
  else if (strcmp("April", mes) == 0) {
    dateMonth = "04";
  }
  else if (strcmp("May", mes) == 0) {
    dateMonth = "05";
  }
  else if (strcmp("June", mes) == 0) {
    dateMonth = "06";
  }
  else if (strcmp("July", mes) == 0) {
    dateMonth = "07";
  }
  else if (strcmp("August", mes) == 0) {
    dateMonth = "08";
  }
  else if (strcmp("September", mes) == 0) {
    dateMonth = "09";
  }
  else if (strcmp("October", mes) == 0) {
    dateMonth = "10";
  }
  else if (strcmp("November", mes) == 0) {
    dateMonth = "11";
  }
  else if (strcmp("December", mes) == 0) {
    dateMonth = "12";
  }
  else {
    dateMonth = ""; // Handle invalid month input
  }
}
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  strftime(bufferMes,20,"%B",&timeinfo);

  retornaMes(bufferMes);
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(pinoSensorUmidade,INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Conecta o ESP32 à sua rede Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

    /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth); // Inicia a conexão do Firebase
}

// the loop function runs over and over again forever
void loop() {
  //delay(1000); // Aguarda 1 segundos antes de recuperar os dados do Firebase
  // Recupera a umidade do Firebase
  humidity = analogRead(pinoSensorUmidade);
  // Lendo dados
  percentHumidity = map(humidity, 0,4095, 100, 0);
  //int humidity = Firebase.get(firebaseData, "/planta/umidade");
  FirebaseJson content;
  if(percentHumidity < 50){
    content.set("fields/ligado/booleanValue", "true"); // passando para o banco do firebase para depois
    digitalWrite(RELAY_PIN, HIGH); // Ligar o relé

    if(millis() - tempoInicial >= 1000){
      Serial.println(millis());
      Serial.println(tempoInicial);
      Serial.println(millis() - tempoInicial);
      tempoDecorrido = tempoInicial/1000; //converter para segundos
      tempoInicial = millis();
    }
    //tempoInicial = millis();
    //content.set("fields/temp/doubleValue", String(tempoInicial).c_str());
      
  }else{
    content.set("fields/ligado/booleanValue", "false");
    digitalWrite(RELAY_PIN, LOW); // Desligar o relé
    //tempoFinal = millis() - tempoInicial;
    
    //unsigned long tempoDecorrido = tempoFinal - tempoInicial;
    //content.set("fields/temp/doubleValue", String(tempoDecorrido).c_str());
  }
  
  content.set("fields/temp/doubleValue", String(tempoDecorrido).c_str());
  //Firebase.setString(firebaseData,"/umidade", percentHumidity);
  String documentPath = "House/Room_1";
  //String testPath = "House/Room_1/data/2023-06-01";
  
  content.set("fields/humidity/doubleValue", String(percentHumidity).c_str());
  Serial.print("Create a document... ");
  // para criar o banco
  //if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, testPath.c_str(), content.raw()))
  //    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  //else
  //    Serial.println(fbdo.errorReason());

  //para atualizar o banco
  if(Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "ligado,humidity,temp")){
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    return;
  }else{
    Serial.println(fbdo.errorReason());
  }

  
  String mask = "ligado";

  Serial.print("Get a document... ");
  // funcao para receber o campo escolhido
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str())){
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      // Create a FirebaseJson object and set content with received payload
      FirebaseJson payload;
      payload.setJsonData(fbdo.payload().c_str());
      // Get the data from FirebaseJson object 
      FirebaseJsonData jsonData;
      payload.get(jsonData, "fields/ligado/booleanValue", true);
      Serial.println(jsonData.stringValue);

      if(jsonData.stringValue == "true"){
        Serial.println("E verdadeiro");
      }
      else{
        Serial.println("E falso");
      }
  }
  else
      Serial.println(fbdo.errorReason());
  Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  Serial.println(dateMonth);
  printLocalTime();
}