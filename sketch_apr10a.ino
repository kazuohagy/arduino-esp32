#include "FirebaseESP32.h"


#define FIREBASE_HOST "https://gaia-25c62-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "G5HN59JZValyPF813J4zc6kzI5Zit6zOiN6C1o71"
#define WIFI_SSID "Kazuo_EXT"
#define WIFI_PASSWORD "12345678w"


FirebaseData firebaseData; // Cria um objeto para armazenar os dados do Firebase
const int pinoSensorUmidade = A0;
void setup() {
  Serial.begin(9600);
  pinMode(pinoSensorUmidade,INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Conecta o ESP32 à sua rede Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Inicia a conexão do Firebase
}

// the loop function runs over and over again forever
void loop() {
  delay(2000); // Aguarda 2 segundos antes de recuperar os dados do Firebase
  // Recupera a temperatura e a umidade do Firebase

  int temperature = Firebase.get(firebaseData, "/planta/temperatura");
  int humidity = Firebase.get(firebaseData, "/planta/umidade");
  Firebase.setString(firebaseData,"/mario", analogRead(pinoSensorUmidade));
  // Faz um get da string a partir do nó /mario no Firebase
 // if (Firebase.getString(firebaseData, "/mario")) {
 //   String myString = firebaseData.jsonString();
 //   Serial.print("String from Firebase: ");
 //   Serial.println(myString);
 //   //Firebase.setString(firebaseData,"/mario","tomanocu");
//
 // } else {
 //   Serial.println("Failed to get string from Firebase!");
 // }
 Serial.println(analogRead(pinoSensorUmidade));
  
    if(Firebase.getInt(firebaseData,"/planta/umidade")){
      if(firebaseData.dataType()=="int"){
        //intValue = firebaseData.intData();
        Serial.print("Umidade: ");
        //Serial.println(intValue);
      }else{
        Serial.println(firebaseData.errorReason());
      }
    }
    if (Firebase.get(firebaseData, "/mario")) {
    // Verifica o tipo de dados retornados
    if (firebaseData.dataType() == "string") {
      String myString = firebaseData.stringData();
      Serial.print("String from Firebase: ");
      Serial.println(myString);
    } else {
      Serial.println("Data from Firebase is not a string!");
    }
  } else {
    Serial.println("Failed to get data from Firebase!");
  }

  firebaseData.clear(); // Limpa os dados armazenados no objeto FirebaseData
}
