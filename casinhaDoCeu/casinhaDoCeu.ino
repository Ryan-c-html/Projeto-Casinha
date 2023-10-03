/*
____________Projeto Casinha so Céu Automatica___________________________

Autora: Vitória Viegas
Curso: edificações
Data: 26/09/2023
*/

//Incluindo Bibliotecas usadas
#include <WiFi.h> // Biblioteca WiFi para o esp 32
#include <DHT.h> 

// Dados para conexão Wi-Fi
const char* ssid = "VitoriaViegas"; // Inserir nome da rede, VitoriaViegas, WILL-2G
const char* password = "viegas23"; // Inserir senha da rede, viegas23, 27052004

//Definindo o server da conexão
WiFiServer server(80);

//Definição do objeto "motorgaragem" para o servo motor
//Servo motorgaragem;

DHT dht(21, DHT11);

float t; // Inicia Variáveld e temperatura
float h; // Inicia variável de umidade

// Define os pinos utilizados pelo ESP32
const int luzCozinha =  4;
const int luzSala1   =  2;
const int luzSala2   = 22;
// O dht está no pino 21
const int som        = 23;
const int pir        = 22;
// Leds que não vão ser usados
const int luzQuarto1 = 15;
const int luzQuarto2 = 16;
const int luzVaranda = 18;
const int servo      = 13;

//Variavel que printa pontinhos enquanto espera a conexão
int conect = 0;

//Variavel que armazena o valor que o sensor pir está enviando
int sinal = 0;

void setup() {
// Inicializa o monitor serial na frequencia 115200
Serial.begin(115200);
Serial.println("----------Iniciando a CASINHA----------");//Printa no monitor Serial que a casinha está sendo inicializada
Serial.println();

// Iniciando o sensor DHT
dht.begin();

// Definindo o pino usado pelo Servo Motor
//motorgaragem.attach(servo);
// Posição que o servo motor da garagem irpa iniciar 
//motorgaragem.write(angulo);

// Define os pinos como saída de sinal
pinMode(luzCozinha,     OUTPUT);
pinMode(luzSala1,       OUTPUT);
pinMode(luzSala2,       OUTPUT);
pinMode(luzQuarto1,     OUTPUT);
pinMode(luzQuarto2,     OUTPUT);
pinMode(luzVaranda,     OUTPUT);
pinMode(som,            OUTPUT);
pinMode(pir,             INPUT);

// Inicia os pinos em nível baixo (LOW)
digitalWrite(luzCozinha,     LOW);
digitalWrite(luzQuarto1,     LOW);
digitalWrite(luzQuarto2,     LOW);
digitalWrite(luzSala1,       LOW);
digitalWrite(luzSala2,       LOW);
digitalWrite(luzVaranda,     LOW);
digitalWrite(som,            LOW);

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
// Leitura do sensor Pir
sinal = digitalRead(pir);
if(sinal == HIGH){
  digitalWrite(som, HIGH);
}
else{
  digitalWrite(som, LOW);
}
//Inicia o WiFiClient e avalia se existe um cliente (pessoa) no server
WiFiClient client = server.available();
//Verifica se um novo cliente se conectou 
if (!client) {delay(250);return;}
Serial.println("Novo cliente se conectou!");

//le o pedido do cliente 
String header = client.readStringUntil('\r');
Serial.println(header);

client.println("<!DOCTYPE html><html lang=\"en\">");
client.println("<head>");
client.println("<meta charset=\"UTF-8\">");
client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
client.println("<title>Casa Inteligente</title>");
client.println("</head><body>");
client.println("<div class=\"caixa\">");
client.println("<h1>Casinha do Céu</h1>");
client.println("<h1>Controle para casa</h1>");
//Printa a Temperatura e a humidade no site html
client.println("<p><b>Temperatura: </b>""C</p>");
client.println(t);
client.println("C</p>");
client.println("<p><b>Humidade: </b>");
client.println(h);
client.println("%</p>");

client.println("</div>");
// Funções que irão verificar o estado do Led e mudar o HTML a partir disso - Luz cozinha

client.println("<div><p><b>Luz Cozinha</b></p>");
client.println("<a href='?cozinha'><button name=\"button\">Ligar</button></a></div>");

// Funções que irão verificar o estado do Led e mudar o HTML a partir disso - Luz Sala 1

client.println("<div><p><b>Luz Sala 1</b></p>");
client.println("<a href='?sala1'><button name=\"button\">Ligar</button></a></div>");

// Funções que irão verificar o estado do Led e mudar o HTML a partir disso - Luz Sala 2

client.println("<div><p><b>Luz Sala 2</b></p>");
client.println("<a href='?sala2'><button name=\"button\">Ligar</button></a></div>");

// Funções que irão verificar o estado do Led e mudar o HTML a partir disso - Luz Quarto 1

client.println("<div><p><b>Luz Quarto 1</b></p>");
client.println("<a href='?quarto1'><button name=\"button\">Ligar</button></a></div>");

// Funções que irão verificar o estado do Led e mudar o HTML a partir disso - Luz Quarto 2

client.println("<div><p><b>Luz Quarto 2</b></p>");
client.println("<a href='?quarto2'><button name=\"button\">Ligar</button></a></div>");

// Funções que irão verificar o estado do Led e mudar o HTML a partir disso - Luz Varanda

client.println("<div><p><b>Luz Varanda</b></p>");
client.println("<a href='?varanda'><button name=\"button\">Ligar</button></a></div>");
// Css da página
client.println("<style>");
client.println("body{background-color: rgb(242, 249, 255);}");
client.println("div{text-align: center;}");
client.println("button{display: inline-block;border-radius: 4px;border: none;text-align: center;font-size: 28px;font-family: fantasy, Impact;color:rgb(254, 254, 254);background-color: rgb(0, 75, 205);padding: 20px;width: 200px;transition: all 0.5s;cursor: pointer;margin: 5px;}");
client.println("button:hover{transform: scale(1.05);}");
client.println("</style>");
client.println("</body></html>");

client.flush();

// Funções que vão mudar o estado do Led
if(header.indexOf("cozinha") != -1){digitalWrite(luzCozinha, !digitalRead(luzCozinha));}
if(header.indexOf("sala1") != -1){digitalWrite(luzSala1, !digitalRead(luzSala1));}
if(header.indexOf("sala2") != -1){digitalWrite(luzSala2, !digitalRead(luzSala2));}
if(header.indexOf("quarto1") != -1){digitalWrite(luzQuarto1, !digitalRead(luzQuarto1));}
if(header.indexOf("quarto2") != -1){digitalWrite(luzQuarto2, !digitalRead(luzQuarto2));}
if(header.indexOf("varanda") != -1){digitalWrite(luzVaranda, !digitalRead(luzVaranda));}

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
