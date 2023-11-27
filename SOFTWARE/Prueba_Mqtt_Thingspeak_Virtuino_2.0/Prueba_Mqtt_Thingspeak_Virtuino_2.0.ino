#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de Wi-Fi
const char* ssid = "";
const char* password = "";

// Configuración de MQTT
const char* mqtt_server = "mqtt3.thingspeak.com";
const char* mqtt_username = "LiIKCCw4AScsNxU7FjwyLyw";
const char* mqtt_password = "Q/B4nlC9DJs5RiADnmeMXK7Z";

const char* api_key = "JGOXDZA0Y6U13CKQ";
const char* publish_topic = "channels/2268200/publish";
const char* subscribe_topic = "channels/2268200/subscribe/fields/field1"; // tópico MQTT de suscripción

/*const char* server = "api.thingspeak.com"; */

// Componentes
const int sensorPin = 26; 
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

  // Suscribirse a un tópico MQTT para recibir mensajes
  client.subscribe(subscribe_topic);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  sensorValue = digitalRead(sensorPin);

  if (sensorValue == HIGH) {
    if (!raining) {
      Led == HIGH;
      Buzzer == HIGH;
      raining = true;
      sendRainAlert();
    }
  } else {
    raining = false;
  }

  client.loop();
  delay(5000); // Espera 5 segundos antes de volver a leer el sensor
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Maneja mensajes MQTT recibidos en el tópico suscrito
  Serial.println("Mensaje MQTT recibido:");
  Serial.print("Tópico: ");
  Serial.println(topic);
  Serial.print("Contenido: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void sendRainAlert() {
  // Enviar un mensaje MQTT a ThingSpeak
  String payload = "field1=1"; // Puedes personalizar los campos según tu canal en ThingSpeak
  String fullTopic = String(publish_topic) + "/" + api_key;
  
  if (client.publish(fullTopic.c_str(), payload.c_str())) {
    Serial.println("Alerta de lluvia enviada a ThingSpeak");
  } else {
    Serial.println("Error al enviar la alerta de lluvia");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando al servidor MQTT...");
    if (client.connect("ESP32Client", "LiIKCCw4AScsNxU7FjwyLyw", "Q/B4nlC9DJs5RiADnmeMXK7Z")) {
      Serial.println("Conexión MQTT exitosa");
      // Resuscribirse al tópico MQTT después de la reconexión
      client.subscribe(subscribe_topic);
    } else {
      Serial.println("Fallo en la conexión MQTT. Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}
