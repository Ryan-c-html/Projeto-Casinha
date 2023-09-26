/*
____________Projeto Casinha so Céu Automatica___________________________

Autora: Vitória Viegas
Curso: edificações
Data: 26/09/2023
*/

//Incluindo Bibliotecas usadas
#include <WiFi.h> 
#include <ESP32Servo.h> 
#include <DHT.h> 

// Dados para conexão Wi-Fi
const char* ssid = "VitoriaViegas"; // Inserir nome da rede
const char* password = "viegas23"; // Inserir senha da rede

//Definindo o server da conexão
WiFiServer server(80);

//Definição do objeto "motorgaragem" para o servo motor
Servo motorgaragem;

DHT dht(5, DHT11);
 
float t; // Inicia Variáveld e temperatura
float h; // Inicia variável de umidade
 
// Define os pinos utilizados pelo ESP32
const int luzsala    =  4;
const int luzvaranda = 12;
const int luzgaragem = 14;
const int luzcozinha = 15;
const int luzquarto  = 16;
const int luzsotao   = 17;
const int servo      = 13;
const int som        = 23;
const int pir        = 22;

//Angulo do Servo Motor
int angulo = 60;
 
void setup() {
// Inicializa o monitor serial na frequencia 115200
Serial.begin(115200);
Serial.println("----------Iniciando a CASINHA----------");//Printa no monitor Serial que a casinha está sendo inicializada
Serial.println();

// Iniciando o sensor DHT
dht.begin();

// Definindo o pino usado pelo Servo Motor
motorgaragem.attach(servo);
// Posição que o servo motor da garagem irpa iniciar 
motorgaragem.write(angulo);

// Define os pinos como saída de sinal
pinMode(luzcozinha,    OUTPUT);
pinMode(luzsala,       OUTPUT);
pinMode(luzgaragem,    OUTPUT);
pinMode(luzvaranda,    OUTPUT);
pinMode(luzquarto,     OUTPUT);
pinMode(luzsotao,      OUTPUT);
pinMode(som,           OUTPUT);
pinMode(pir,            INPUT);
 
// Inicia os pinos em nível baixo (LOW)
digitalWrite(luzcozinha,  LOW);
digitalWrite(luzsala,     LOW);
digitalWrite(luzgaragem,  LOW);
digitalWrite(luzvaranda,  LOW);
digitalWrite(luzquarto,   LOW);
digitalWrite(luzsotao,    LOW);
digitalWrite(som,         LOW);
  
Serial.println();
Serial.println("Tentando conexão com WiFi");
initwifi(); // Função que inicia o WiFi
//Printa no monitor serial o nome da rede e seu ip local
Serial.println();
Serial.print("Nome da rede: ");
Serial.println(ssid);
Serial.print("O IP da rede é: ");
Serial.println(WiFi.localIP());
//Inicializa o server 
server.begin();
//Delay para o server ter um tempo para inicializar
delay(1 * 1000);
}
void loop() {
// Leitura do sensor DHT
h = dht.readHumidity();
t = dht.readTemperature();
//Inicia o WiFiClient e avalia se existe um cliente (pessoa) no server
WiFiClient client = server.available();
//Verifica se um novo cliente se conectou 
if (client) {delay(250);return;}
Serial.println("Novo cliente se conectou!");

//le o pedido do cliente 
String header = client.readStringUntil('\r');
Serial.println(header);
  
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".button { background-color: #B84F4F; border: none; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".button2 {background-color: #4CAF50;}</style></head>"); 
client.println("<body><h1>CONTROLE PARA CASA</h1>");
client.println("<hr>");
client.println("<hr>");
client.println("<h3>Temperatura: " + (String)t + "</h3>");
client.println("<h3>Umidade: " + (String)h + "</h3>");
client.println("<hr>");
client.println("<h3>Luz Cozinha - " + cozinha + "</h3>");
if (cozinha == "Ligado") {
client.println("<p><a href=\"/cozinha/desligado\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/cozinha/ligado\"><button class=\"button button2\">LIGAR</button></a></p>");
}
client.println("<hr>");
client.println("<h3>Luz Sala - " + sala + "</h3>");
if (sala == "Ligado") {
client.println("<p><a href=\"/sala/desligado\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/sala/ligado\"><button class=\"button button2\">LIGAR</button></a></p>");
}
client.println("<hr>");
client.println("<h3>Luz Garagem - " + garagem + "</h3>");
if (garagem == "Ligado") {
client.println("<p><a href=\"/garagem/desligado\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/garagem/ligado\"><button class=\"button button2\">LIGAR</button></a></p>");
}
client.println("<hr>");
client.println("<h3>Luz Varanda - " + varanda + "</h3>");
if (varanda == "Ligado") {
client.println("<p><a href=\"/varanda/desligado#varanda\" name=\"varanda\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/varanda/ligado#varanda\" name=\"varanda\"><button class=\"button button2\">LIGAR</button></a></p>");
} 
client.println("<hr>");
client.println("<h3>Luz Quarto - " + quarto + "</h3>");
if (quarto == "Ligado") {
client.println("<p><a href=\"/quarto/desligado#quarto\" name=\"quarto\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/quarto/ligado#quarto\" name=\"quarto\"><button class=\"button button2\">LIGAR</button></a></p>");
}
client.println("<hr>");
client.println("<h3>Luz Sotao - " + sotao + "</h3>");
if (sotao == "Ligado") {
client.println("<p><a href=\"/sotao/desligado#sotao\" name=\"sotao\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/sotao/ligado#sotao\" name=\"sotao\"><button class=\"button button2\">LIGAR</button></a></p>");
}
client.println("<hr>");
client.println("<h3>Porta Garagem - " + porta + "</h3>");
if (porta == "Aberta") {
client.println("<p><a href=\"/porta/fechada#porta\" name=\"porta\"><button class=\"button\">FECHAR</button></a></p>");
} else {
client.println("<p><a href=\"/porta/aberta#porta\" name=\"porta\"><button class=\"button button2\">ABRIR</button></a></p>");
}
client.println("<hr>");
client.println("<h3>Alarme - " + alarme + "</h3>");
if (alarme == "Ativado") {
client.println("<p><a href=\"/alarme/desativado#alarme\" name=\"alarme\"><button class=\"button\">DESATIVAR</button></a></p>");
} else {
client.println("<p><a href=\"/alarme/ativado#alarme\" name=\"alarme\"><button class=\"button button2\">ATIVAR</button></a></p>");
}
client.println("</body></html>"); 
client.println();

client.flush();


// Funções que vão mudar o estado do Led
if(header.indexOf() != -1){digitalWrite(, !digitalRead())}
if(header.indexOf() != -1){digitalWrite(, !digitalRead())}
if(header.indexOf() != -1){digitalWrite(, !digitalRead())}
if(header.indexOf() != -1){digitalWrite(, !digitalRead())}
if(header.indexOf() != -1){digitalWrite(, !digitalRead())}
if(header.indexOf() != -1){digitalWrite(, !digitalRead())}
//Função que vai mudar o angulo do servo
if(header.indexOf() != -1){
    if(angulo == ){
        for(angulo = ; angulo >  ; angulo--){
            servo.write(angulo);
            delay(5);
        }
    }
    if(angulo == ){
        for(angulo = ; angulo <  ; angulo++){
            servo.write(angulo);
            delay(5);
        }
    }
}


delay(200);

client.stop(); // Corta a conexão do cliente

Serial.println("Cliente desconectado");
Serial.println();
Serial.println();
}

// Função que irá iniciar o WiFi do Esp 32
void initwifi()
{
WiFi.begin(ssid, password);
WiFi.mode(WIFI_STA);
while(WiFi.status() != WL_CONNECTED){
 delay(500);
 Serial.print(".");  
 if(conect >= 100){Serial.println(".");}
 conect++;
}
Serial.println();
Serial.println("ESP32 se conectou ao WiFi");
delay(1000);
}
