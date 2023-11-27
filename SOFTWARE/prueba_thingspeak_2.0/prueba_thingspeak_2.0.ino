#include <WiFi.h>
#include <ThingSpeak.h>

// Definición de los pines
const int rainSensorPin = 32; // Pin digital al que está conectado el sensor de lluvia
const int buzzerPin = 33 // Pin digital al que está conectado el buzzer

// Configuración de WiFi
const char *ssid = "Estudiantes"; // Cambia esto a tu SSID de Wi-Fi
const char *password = "educar_2018"; // Cambia esto a tu contraseña de Wi-Fi

// ThingSpeak Channel ID y API Key
unsigned long channelID = 2268200; // Cambia esto al Channel ID de tu canal
const char *apiKey = "JGOXDZA0Y6U13CKQ"; // Cambia esto a tu API Key

// Variables globales
int rainSensorValue = 0;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  pinMode(rainSensorPin, INPUT); // Inicializa el sensor de lluvia
  pinMode(buzzerPin, OUTPUT); // Configura el pin del buzzer como salida

  // Conéctate a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }
  Serial.println("Conexión exitosa a Wi-Fi");

  // Inicializa ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Leer el valor del sensor de lluvia
  
  
  for(int x=0;x<10;x++){
  rainSensorValue = digitalRead(rainSensorPin);
 Serial.println(rainSensorValue);}
  // Si se detecta lluvia (valor HIGH), enviar una notificación a ThingSpeak
  if (rainSensorValue == HIGH) {
    digitalWrite(buzzerPin, HIGH);
    // Enviar datos a ThingSpeak
    
    ThingSpeak.writeField(channelID, 1, 1, apiKey);

    Serial.println("Lluvia detectada. ");
  }
  else {
    // Apagar el buzzer cuando no hay lluvia
    digitalWrite(buzzerPin, LOW);
    
    ThingSpeak.writeField(channelID, 1, 0, apiKey);
    Serial.println("No se detectó Lluvia.");
   

  delay(5000); // Esperar 1 minuto antes de volver a verificar
}
}
