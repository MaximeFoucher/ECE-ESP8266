#include <ESP8266WiFi.h>
#include<Servo.h>

Servo myservo;

const char* ssid = "Oui";
const char* password = "groschibrax";

// Broches pour contrôler le moteur DC (A+ et A-)
const int motorAspeed = 5; // A+
const int motorAdir = 0; // A-
const int motorBspeed = 4; // A+
const int motorBdir = 2; // A-

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(100);

  myservo.attach(D7, 500, 2400);
  
  pinMode(motorAspeed, OUTPUT);
  pinMode(motorAdir, OUTPUT);
  pinMode(motorBspeed, OUTPUT);
  pinMode(motorBdir, OUTPUT);
  
  Serial.println();
  Serial.print("Connexion au réseau WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }
  
  Serial.println("Nouveau client");

  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  // Contrôle du moteur DC
  if (request.indexOf("/motor=avant") != -1) {
    analogWrite(motorAspeed, 1023); // Sens horaire
    digitalWrite(motorAdir, HIGH);
    analogWrite(motorBspeed, 1023); // Sens horaire
    digitalWrite(motorBdir, HIGH);
  } 
  else if (request.indexOf("/motor=off") != -1) {
    analogWrite(motorAspeed, 0); // Sens horaire
    digitalWrite(motorAdir, LOW);
    analogWrite(motorBspeed, 0); // Sens horaire
    digitalWrite(motorBdir, LOW);
  }
  else if (request.indexOf("/motor=arriere") != -1) {
    analogWrite(motorAspeed, 1023); // Sens horaire
    digitalWrite(motorAdir, LOW);
    analogWrite(motorBspeed, 1023); // Sens horaire
    digitalWrite(motorBdir, LOW);
  }
  else if (request.indexOf("/motor=droite") != -1) {
    analogWrite(motorAspeed, 1023); // Sens horaire
    digitalWrite(motorAdir, LOW);
    analogWrite(motorBspeed, 1023); // Sens horaire
    digitalWrite(motorBdir, HIGH);
  }
  else if (request.indexOf("/motor=gauche") != -1) {
    analogWrite(motorAspeed, 1023); // Sens horaire
    digitalWrite(motorAdir, HIGH);
    analogWrite(motorBspeed, 1023); // Sens horaire
    digitalWrite(motorBdir, LOW);
  }
  if (request.indexOf("/servo=tourne") != -1) {
    myservo.write(0);
    delay(500);
    myservo.write(180);
    delay(500);
  } 
  
  // Réponse HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>Controle du moteur</title></head>");
  client.println("<body>");
  client.println("<h1>Controle du moteur</h1>");
  client.println("<a href=\"/motor=gauche\"><button> gauche </button></a>");
  client.println("<a href=\"/motor=avant\"><button>avant</button></a>");
  client.println("<a href=\"/motor=droite\"><button> droite </button></a>");
  client.println("<br>");
  client.println("<br>");
  client.println("<a href=\"/motor=arriere\"><button>arriere</button></a>");
  client.println("<a href=\"/motor=off\"><button>eteindre</button></a>");
  client.println("<a href=\"/servo=tourne\"><button>tourner servo</button></a>");
  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client déconnecté");
  Serial.println("");
}
