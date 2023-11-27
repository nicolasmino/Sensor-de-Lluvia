#include <WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "Estudiantes";     // Cambia esto con tu SSID de Wi-Fi
const char* password = "educar_2018"; // Cambia esto con tu contraseña de Wi-Fi
const char* thingSpeakApiKey = "JGOXDZA0Y6U13CKQ"; // Cambia esto con tu clave API de ThingSpeak
unsigned long myChannelNumber = 2268200;

const int sensorPin = 26; // Pin analógico donde está conectado el sensor HW-038
const int alarmPin = 25;  // Pin digital donde está conectada la alarma

int sensorThreshold = 2000; // Ajusta este valor según tu entorno

WiFiClient client;

void setup() {
  pinMode(alarmPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  Serial.begin(115200);

  // Conexión a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Inicialización de ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  if (sensorValue > sensorThreshold) {
    // Activar la alarma y enviar datos a ThingSpeak
    digitalWrite(alarmPin, HIGH);
    Serial.println("¡Alerta! Se detectó lluvia.");
    ThingSpeak.setField(1, "1"); // Campo 1 de ThingSpeak (puede ser cualquier campo)
  } else {
    // Desactivar la alarma
    digitalWrite(alarmPin, LOW);
    Serial.println("No se detectó agua.");
    ThingSpeak.setField(1, "0"); // Campo 1 de ThingSpeak (puede ser cualquier campo)
  }

  // Enviar datos a ThingSpeak cada 15 segundos (puedes ajustar el intervalo)
  ThingSpeak.writeFields(myChannelNumber, thingSpeakApiKey);

  delay(15000); // Esperar 15 segundos antes de la próxima lectura
}
