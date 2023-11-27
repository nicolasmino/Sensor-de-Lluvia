#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "hyper";
const char* password = "jofre";
const char* mqtt_server = "broker.emqx.io"; // Cambia esto por la dirección de tu servidor MQTT EMQX
const int mqtt_port = 1883; // Puerto MQTT predeterminado
const char* mqtt_user = "HYPERMQTT";
const char* mqtt_password = "JOFREMQTT";

const char* alarm_topic = "mi_alarma";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el topic: ");
  Serial.println(topic);

  if (strcmp(topic, alarm_topic) == 0) {
    // Analizar el payload para determinar la acción de la alarma
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i];
    }
    
    if (message == "activar") {
      // Aquí puedes activar la alarma
      Serial.println("Alarma activada");
    } else if (message == "desactivar") {
      // Aquí puedes desactivar la alarma
      Serial.println("Alarma desactivada");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando al servidor MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado");
      client.subscribe(alarm_topic);
    } else {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

