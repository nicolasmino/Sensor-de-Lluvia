#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de Wi-Fi
const char* ssid = "Estudiantes";
const char* password = "educar_2018";

// Configuración de MQTT
const char* mqtt_server = "mqtt3.thingspeak.com";
const char* mqtt_username = "LiIKCCw4AScsNxU7FjwyLyw";
const char* mqtt_password = "Q/B4nlC9DJs5RiADnmeMXK7Z";

const char* api_key = "JGOXDZA0Y6U13CKQ";
const char* topic = "channels/2268200/publish";

const char* server = "api.thingspeak.com";

// Componentes
const int sensorPin = 26; // Cambia esto al pin que estés utilizando
int Led;
int Buzzer;

// Variables para almacenar el estado de la lluvia
int sensorValue = 0;
boolean raining = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);

  // Conexión a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión WiFi exitosa");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  sensorValue = digitalRead(sensorPin);

  if (sensorValue == HIGH) {
    if (!raining) {
      raining = true;
      Led == HIGH;
      Buzzer == HIGH;
      

      sendRainAlert();
    }
  } else {
    raining = false;
  }

  client.loop();
  delay(5000); // Espera 5 segundos antes de volver a leer el sensor
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Maneja mensajes de retorno de MQTT si es necesario
}

void sendRainAlert() {
  // Enviar un mensaje MQTT a ThingSpeak
  String payload = "field1=1"; // Puedes personalizar los campos según tu canal en ThingSpeak
  String fullTopic = String(topic) + "/" + api_key;
  
  if (client.publish(fullTopic.c_str(), payload.c_str())) {
    Serial.println("Alerta de lluvia enviada a ThingSpeak");
  } else {
    Serial.println("Error al enviar la alerta de lluvia");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando al servidor MQTT...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("Conexión MQTT exitosa");
    } else {
      Serial.println("Fallo en la conexión MQTT. Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}
