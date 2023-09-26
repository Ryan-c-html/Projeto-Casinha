/*
____________Projeto CASINHA Automatica___________________________

Autores: Ryan Sales e Vitoria Viegas
Cursos: Mecânica e Edificações
Data: 05/12/22
*/

//Incluindo Bibliotecas usadas
#include <WiFi.h>
#include <ESP32Servo.h>
//Variaveis que vão armazenar o nome e a senha do WiFi
const char* ssid = "Oi"; //Robotica -- Oi -- Ryan_5G
const char* password = "tudobom?";  //arduino2015 -- tudobom? --
//Definindo o server da conexão
WiFiServer server(80);
//Definição das portas usadas
const int led1 = 23;   //Porta do led do ldr
const int ldr = 15;    //porta do ldr
const int sensorM = 2; //Porta do Sensor de presença
const int buzzer = 27; //porta do buzzer
const int servo1 = 4;  //Porta do ServoMotor
//Portas dos leds RGB
const int rgb1R = 34;
const int rgb1G = 35;
const int rgb1B = 32;
const int rgb2R = 33;
const int rgb2G = 25;
const int rgb2B = 26;
const int rgb3R = 14;
const int rgb3G = 12;
const int rgb3B = 13;

//Configuração do Servo
Servo servo; //Inicializa o ServoMotor
/*
//Led RGB 1
const int 1R_canal = 2;  
const int 1G_canal = 3;  
const int 1B_canal = 4;
//Led RGB2
const int 2R_canal = 5;  
const int 2G_canal = 6;  
const int 2B_canal = 7;
//Led RGB3
const int 3R_canal = 8;  
const int 3G_canal = 9;  
const int 3B_canal = 10;
*/
//frequencia dos leds RGB
const int Freq_Led = 5000; // pwm frequencia 
//Variaveis para o buzzer PWM
const int freq = 2000;
const int Bu_canal = 11;
const int resolucao = 8;
//variavel de conexão
int conect = 0;
//VAriavel para posição do Servo
int pos;
//Variavel Bool que vai interromper o alarme
//bool interru = LOW;

void setup() {
Serial.begin(115200);//Inicializa o Monitor Serial
Serial.println("----------Iniciando a CASINHA----------");//Printa no monitor Serial que a casinha está sendo inicializada
Serial.println();

pinMode(ldr, INPUT);    //Defne o pino do ldr como uma entrada

pinMode(led1, OUTPUT);
digitalWrite(led1, LOW);//Inicia o Led de fora da casa desligado
//Configuração do PWM(Leds RGB, Servo Motor, buzzer)
configPWM(); 
delay(500);

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
//Inicia o WiFiClient e avalia se existe um cliente (pessoa) no server
WiFiClient client = server.available();
//Variaveis que vão armazenar os valores dos sensores da casa
int leitura = analogRead(ldr);
int l = digitalRead(sensorM);
//Liga luz externa a partir do sensor de luminosidade
if(leitura > 1050){digitalWrite(led1, HIGH);}
if(1050 < leitura < 900){digitalWrite(led1, digitalRead(led1));}
if(leitura < 900){digitalWrite(led1, LOW);}
//Verifica se um novo cliente se conectou 
if(!client){delay(250); return;}
Serial.println("Novo cliente se conectou!");
//le o pedido do cliente 
String req = client.readStringUntil('\r');
Serial.println(req);
//Criação do site
client.println("<!DOCTYPE html>");
client.println("<html lang=\"en\">");
client.println("<head>");
client.println("<meta charset=\"UTF-8\">");
client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
client.println("<title>CASINHA</title>");
client.println("</head>");
client.println("<body><h1>Automação Casinha</h1><br>");
client.println("<h2>Porta Casinha</h2>");
client.println("<a href='?abrir'><button name=\"porta1\">Abrir</button></a>");
client.println("<a href='?fechar'><button name=\"porta2\">Fechar</button></a><br>");

client.println("<h2>Led RGB-Quarto-1</h2>");
client.println("<a href='?desligar1'><button name=\"1RGB\">Desligar</button></a>");
client.println("<a href='?branco1'><button name=\"W1RGB\">Ligado</button></a><br>");
client.println("<a href='?vermelho1'><button name=\"R1RGB\">Vermelho</button></a>");
client.println("<a href='?azul1'><button name=\"B1RGB\">Azul</button></a>");
client.println("<a href='?verde1'><button name=\"G1RGB\">Verde</button></a>");
client.println("<h2>Led RGB-Quarto-2</h2>");
client.println("<a href='?desligar2'><button name=\"2RGB\">Desligar</button></a>");
client.println("<a href='?branco2'><button name=\"W2RGB\">Ligado</button></a><br>");
client.println("<a href='?vermelho2'><button name=\"R2RGB\">Vermelho</button></a>");
client.println("<a href='?azul2'><button name=\"B2RGB\">Azul</button></a>");
client.println("<a href='?verde2'><button name=\"G2RGB\">Verde</button></a>");
client.println("<h2>Led RGB-Sala</h2>");
client.println("<a href='?desligar3'><button name=\"3RGB\">Desligar</button></a>");
client.println("<a href='?branco3'><button name=\"W3RGB\">Ligado</button></a><br>");
client.println("<a href='?vermelho3'><button name=\"R3RGB\">Vermelho</button></a>");
client.println("<a href='?azul3'><button name=\"B3RGB\">Azul</button></a>");
client.println("<a href='?verde3'><button name=\"G3RGB\">Verde</button></a>");
client.println("<style>");
client.println("html{font-family: 'Open Sans', sans-serif;display: block;margin: 0px auto;}");
client.println("body{text-align:center;background-color: rgb(242, 249, 255);; margin-top: 0%;}");
client.println("button{background-color: rgba(131, 131, 131, 0.925); border-radius:18px ;border-color: rgb(114, 114, 134); margin-top: 4px; margin-bottom: 3px;}");
client.println("</style>");
client.println("</body></html>");

client.flush();

//Função do Servo
if(req.indexOf("abrir") != -1){
  for(pos = 0; pos < 90; pos++)
  {
    servo.write(pos);
    delay(2);
  }
}
if(req.indexOf("fechar") != -1){
  for(pos = 90; pos > 0; pos--)
  {
    servo.write(pos);
    delay(5);
  }
}

//Funções que vão ligar o Led RGB 1
if(req.indexOf("desligar1") != -1){RGB_color(1, 0, 0, 0);}
if(req.indexOf("branco1") != -1){RGB_color(1, 255, 255, 255);}
if(req.indexOf("vermelho1") != -1){RGB_color(1, 255, 0, 0);}
if(req.indexOf("verde1") != -1){RGB_color(1, 0, 255, 0);}
if(req.indexOf("azul1") != -1){RGB_color(1, 0, 0, 255);}
//Funções que vão ligar o Led RGB 2
if(req.indexOf("desligar2") != -1){RGB_color(2, 0, 0, 0);}
if(req.indexOf("branco2") != -1){RGB_color(2, 255, 255, 255);}
if(req.indexOf("vermelho2") != -1){RGB_color(2, 255, 0, 0);}
if(req.indexOf("verde2") != -1){RGB_color(2, 0, 255, 0);}
if(req.indexOf("azul2") != -1){RGB_color(2, 0, 0, 255);}
//Funções que vão ligar o Led RGB 3
if(req.indexOf("desligar3") != -1){RGB_color(3, 0, 0, 0);}
if(req.indexOf("branco3") != -1){RGB_color(3, 255, 255, 255);}
if(req.indexOf("vermelho3") != -1){RGB_color(3, 255, 0, 0);}
if(req.indexOf("verde3") != -1){RGB_color(3, 0, 255, 0);}
if(req.indexOf("azul3") != -1){RGB_color(3, 0, 0, 255);}

delay(200);

client.stop();

Serial.println("Cliente desconectado");
Serial.println();
Serial.println();
}

void configPWM() {
//Configurações da porta do Servo
servo.attach(servo1);
//RGB1
ledcAttachPin(rgb1R , 2);
ledcAttachPin(rgb1G , 3);
ledcAttachPin(rgb1B , 4);
//RGB2
ledcAttachPin(rgb2R , 5);
ledcAttachPin(rgb2G , 6);
ledcAttachPin(rgb2B , 7);
//RGB3
ledcAttachPin(rgb3R , 8);
ledcAttachPin(rgb3G , 9);
ledcAttachPin(rgb3B , 10);
//Setup dos leds RGB
ledcSetup(2 , Freq_Led, resolucao);
ledcSetup(3 , Freq_Led, resolucao);
ledcSetup(4 , Freq_Led, resolucao);
ledcSetup(5 , Freq_Led, resolucao);
ledcSetup(6 , Freq_Led, resolucao);
ledcSetup(7 , Freq_Led, resolucao);
ledcSetup(8 , Freq_Led, resolucao);
ledcSetup(9 , Freq_Led, resolucao);
ledcSetup(10 , Freq_Led, resolucao);
//Configura o PWM do Buzzer
ledcSetup(Bu_canal, freq, resolucao);
ledcAttachPin(buzzer, Bu_canal);
}
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
void RGB_color(int n, int i, int j, int k){
int canal1;int canal2;int canal3;
if(n == 1){canal1 = 2;canal2 = 3;canal3 = 4;}
if(n == 2){canal1 = 5;canal2 = 6;canal3 = 7;}
if(n == 3){canal1 = 8;canal2 = 9;canal3 = 10;}
ledcWrite(canal1, i);   
ledcWrite(canal2, j);  
ledcWrite(canal3, k);}
void Buzzer_som(int t,int so){
ledcWrite(Bu_canal, so);
ledcWriteTone(Bu_canal, t);
/*
delay(te * 1000);
ledcWrite(Bu_canal, 0);*/
}
