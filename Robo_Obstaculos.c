//------------------------------------------ Sensor Ultrasonico
//incluindo a biblioteca Ultrasonic.h para deixar o controle dele mais simplificado 
#include "Ultrasonic.h"

//Usando a função Ultrasonic se declara o objeto ultrasonico (ou seja o sensor)
//Trigger ligado ao pino 9 e o Eccho ao pino 10
Ultrasonic SensorUltrassonico1(9, 10);
//se quiser colocar outro sensor ultrasonico basta somente:
//Ultrasonic SensorUltrassonico2( , ); (e escolher os pinos);

//variavel que armazena o valor do tempo na qual o som demora pra ir e voltar mediante o sensor
long Microsegundos = 0;
//variavel tipo float para armazzenar a distancia em centimetros fornecida para o sensor 
float DistanciaemCM = 0;

//------------------------------------------ Ferramenta de Hardware
//que motor ta ligado em qual pino do arduino
//#define da nome as contantes (isso não ocupa espaço na memoria do compilador - pq o compilador substitui o numero 7 onde tem o nome --> pode ser int tambem mas ocupa memoria no microcomputador)
#define MotorLadoEsquerdo1 7 //entrada ou saida digital --> se o pino 7 tiver HIGH e o 8 LOW ele gira por um lado ao contrario ele gira pro outro
#define MotorLadoEsquerdo2  8 

#define MotorLadoDireito1  4 //entrada ou saida digital --> se o pino 4 tiver HIGH e o 5 LOW ele gira por um lado ao contrario ele gira pro outro
#define MotorLadoDireito2 5

//define o motor do lado esquedo e direito olhando por baixo do robo
#define VelocidadeMotorLadoEsquerdo 6 //pino 6 é o pulso controlado via PWM 
#define VelocidadeMotorLadoDireito 3

//------------------------------------------ Duas variaveis do tipo inteiro para ajustar a velocidade do motor via ponte H --> Caso o robo esteja pendendo pra um lado isso nevela 
int ValorVelocidadeMotorLadoEsquerdo = 120; //para ele andar o maximo possivel reto --> ir testando (inicio os dois em 120) --> valor varia de 0(min) e 255(max)
int ValorVelocidadeMotorLadoDireito = 80;

void setup() {
//------------------------------------------ Declara os pinos --> saida - sai do arduino e vai pra ponte H
  pinMode(MotorLadoEsquerdo1, OUTPUT);
  pinMode(MotorLadoEsquerdo2, OUTPUT);
  pinMode(MotorLadoDireito1, OUTPUT);
  pinMode(MotorLadoDireito2, OUTPUT);
  
//inicio da comunicação serial de 115200 (normal é de 9600)
  Serial.begin(115200); //muito alto --> fato de ter iniciado a comunicação serial do seu arduino interfere no tempo de leitura (para ter uma resposta mais rapida do sensor em relação aos motores precisa de uma velocidade alta)  
  delay(3000); //aqui é a parte que delimita o tempo (quando for fazer MiniSumo colocar um botão e inserir delay (5000) pelas regras da robocore o robo tem que esperar 5 segundo pra começar a luta
  // tem 3 segundos pra arrumar o robo / botar ele no chão e etc --> ai ele ja começa luta
}
void loop() {
//------------------------------------------ Convertendo a distancia em centimetros e lendo o sensor 
  DistanciaemCM = SensorUltrassonico1.convert(SensorUltrassonico1.timing(), Ultrasonic::CM); //biblioteca Ultrasonic.h tem a função para converter (SensorUltrassonico1.convert) ela converte o tempo do sensor (SensorUltrassonico1.timing()) para centimetros ou em inches
  //converte o tempo em centimetros e joga dentro da variavel DistanciaemCM
  
  Serial.print(DistanciaemCM); //printa essa informação
  Serial.println(" cm"); //printa o texto
  
  //------------------------------------------ PARTE QUE O ROBO SE MOVE REALMENTE
  if (DistanciaemCM <= 40) { //se o sensor detectar uma distancia em centimetro menor ou igual a 40 (numero a nossa escolha)
    
    //primeiro: seta a velocidade dos motores 
    //analogWrite --> função que escreve valores analogicos utilizando a modulação por largura de pulso PWM
    analogWrite(VelocidadeMotorLadoEsquerdo, ValorVelocidadeMotorLadoEsquerdo); //(pino,120) --> (linha 26,linha 30)
    analogWrite(VelocidadeMotorLadoDireito, ValorVelocidadeMotorLadoDireito); //(pino,80) --> (linha 27,linha 31)
    
  //------------------------------------------ FORMAS DE DESVIO SE FOR MENOR OU IGUAL A 40 CENTIMETROS
    //motor do lado esquerdo vai para tras -- como?
    digitalWrite(MotorLadoEsquerdo1, HIGH);
    digitalWrite(MotorLadoEsquerdo2, LOW);
    //motor do lado direito vai para tras -- como?
    digitalWrite(MotorLadoDireito1, HIGH); //digitalWrite --> escreve na digital valor que pode ser 0(low) ou 1(high)
    digitalWrite(MotorLadoDireito2, LOW);
    delay(700); //tempo que ele vai ficar indo pra tras (7 segundos)
    
    // ----- fica 7 segundos indo para tras, depois faz a volta indo pela direita por 200 milisegundos
    //motor do lado esquerdo para frente 
    digitalWrite(MotorLadoEsquerdo1, LOW);
    digitalWrite(MotorLadoEsquerdo2, HIGH);
    //motor do lado direito para tras 
    digitalWrite(MotorLadoDireito1, HIGH);
    digitalWrite(MotorLadoDireito2, LOW);
   
    delay(200); //por 200 milisegundos 
  }
   //------------------------------------------ COMO AGIR SE FOR MAIOR QUE 40 CENTIMETROS 
  else {
    //seta a velocidade dos motores novamente
    analogWrite( VelocidadeMotorLadoEsquerdo, ValorVelocidadeMotorLadoEsquerdo);
    analogWrite( VelocidadeMotorLadoDireito, ValorVelocidadeMotorLadoDireito);
    //motor do lado esquerdo vai para frente -- como?
    digitalWrite(MotorLadoEsquerdo1, LOW);
    digitalWrite(MotorLadoEsquerdo2, HIGH);
    //motor do lado direito vai para frente -- como?
    digitalWrite(MotorLadoDireito1, LOW);
    digitalWrite(MotorLadoDireito2, HIGH);
  }
}
//------------------------------------------ Resumindo
//Robo fica andando em linha reta, quando tem um obstaculo a distancia diminui (menor que 40), o robo vai para tras por 700 milisegundos vira para o lado esquerdo por 200 milisegundos
//Assim o obstaculo ja saiu do sensor, se não saiu vai continuar indo para tras virado para o lado ate sair, quando a distancia ficar maior que 40 ele continua seu caminho em linha reta  
