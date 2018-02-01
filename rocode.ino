/*ROCODE
 *Codigo arduino para um veiculo controlado por voz, que desvia de obstaculos
 *Trabalho realizado para a disciplina de Sistemas Embutidos da Universeidade Federal de Ouro Preto
 *
 *Fillipe Souza
 *Romario Da Silva
 *Tiago Estevao
 */


#include <SoftwareSerial.h>    //inclui a biblioteca para fazer a comunicacao serial

#define echopin  7 // pino echo do sensor HC-SR04 frontal
#define trigpin 8 // pino trigger do sensor HC-SR04 frontal
#define echopin_d 9  // pino echo do sensor HC-SR04 lateral
#define trigpin_d 12  // pino trigger do sensor HC-SR04 lateral

//------------------------------------------------------------------------------


SoftwareSerial RC(10, 11); //TX, RX respectivamente
String comando;     //string onde o comando de voz sera armazenado
long duracao, distancia, distancia_d, duracao_d;   // variaveis auxiliares para o calculo da distancia pelo HC-SR04 


//---------------------------------------------------------------------------------------
void setup() {  // funcao que seta as configuracoes do sistema
 RC.begin(9600); // inicia comunicacao serial
 Serial.begin(9600);
  pinMode (trigpin, OUTPUT);  // seta o pino trigger como output
  pinMode (echopin, INPUT );   // seta o pino echo como input
  pinMode (trigpin_d, OUTPUT);  // seta o pino trigger como output
  pinMode (echopin_d, INPUT );  // seta o pino echo como output
  pinMode(3, OUTPUT);  // seta os pinos que contralarao o motor atraves da ponte H
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

//--------------------------------------------------------------------------------------



void frente(){ // funcao que faz com que o veiculo ande para frente
      digitalWrite (3, HIGH);
      digitalWrite (4, HIGH);
      digitalWrite (5,LOW);
      digitalWrite (6,LOW);
      delay(200);      
}


void volta(){  // funcao que faz com que o veiculo ande para tras
    digitalWrite (3, LOW);                                                                                                                                                                                                                                                         
    digitalWrite (4, LOW);
    digitalWrite (5, HIGH);
    digitalWrite (6, HIGH);
    delay(2000);  
}


void esquerda(){  // funcao que faz com que o veiculo ande para esquerda
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(1000);
   digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(450);
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(1500);
   digitalWrite (3, HIGH);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
}



void direita(){ // funcao que faz com que o veiculo ande para direita
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(1000);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(450);
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(1500);
   digitalWrite (3, HIGH);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
}

 
 
void para(){ // funcao que faz com que o veiculo pare
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(500);
}

void gira_direita(){  // funcao que faz com que o veiculo ande para direita
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(1000);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(450);
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(500);
  
}


void gira_esquerda(){  // funcao que faz com que o veiculo gire para esquerda
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(1000);
   digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(450);
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay(500);
  
}
//--------------------------------------------------------------------------

void verifica(){  // funcao que verifica distancia em relacao ao HC-SR04 frontal
    digitalWrite(trigpin,LOW);  //seta o pino trigger frontal para low
    delayMicroseconds(2);  //delay necessario para estabilidade
    
    digitalWrite(trigpin,HIGH);  //seta o pino trigger frontal para high
    delayMicroseconds(10);  //delay necessario para estabilidade
    
    duracao=pulseIn (echopin,HIGH);  // armazena o tempo que o sinal demorou para voltar para o pino echo
    
    distancia= duracao/58.2; // calcula a distancia detectada em cm
    delay(50);
    Serial.println(distancia);  // efetua um print serial da distancia lida
}

void verifica_d(){  // funcao que verifica distancia em relacao ao HC-SR04 frontal
    digitalWrite(trigpin_d,LOW);  //seta o pino trigger frontal para low
    delayMicroseconds(2);  //delay necessario para estabilidade
    
    digitalWrite(trigpin_d,HIGH);  //seta o pino trigger frontal para high
    delayMicroseconds(10);  //delay necessario para estabilidade
    
    duracao_d = pulseIn (echopin_d,HIGH);   // armazena o tempo que o sinal demorou para voltar para o pino echo
    
    distancia_d= duracao_d/58.2;  // calcula a distancia detectada em cm
    delay(50);
    Serial.println(distancia_d);  // efetua um print serial da distancia lida
}


//---------------------------------------------------------------

void rotina_obstaculo(){  // funcao a ser executada caso o sensor frontal encontre algum obstaculo
      if (distancia < 15){  // se a distancia detectada formenor que 15 cm
        para();  //o veiculo para
        gira_direita();// gira para direita
        verifica_d(); //verifica a distancia lateral
        while(distancia_d < 15){  // enquanto a distancia lateral for menor que 15 cm 
          verifica_d();  // verifica a distacia lateral
          frente(); //o veiculo segue em frente
        }
        
        delay(200); // delay necessario para que o carrinho ultrapasse completamente o obstaculo
        verifica_d(); // verifca novamente a distancia lateral
        if(distancia_d > 15){ // se a distancia lateral for maior do que 15 cm
          para();  // o veiculo para
          gira_esquerda();  // gira para esquerda
          frente();  // segue em frente
        }
        
        comando = "interrup";  // mudancade variavel necessaria para que o comando enviado via bluetooth prevaleca
      }
     
      }  
      


//-------------------------------------------------------------------
void loop() {  // loop principal
  
  while (RC.available()){  //verifica se existe um byte disponivel para leitura
  delay(10); //delay para estabilidade
  char c = RC.read(); //conduz uma leitura serial
  comando += c; //armazena o resultado da leiturana variavel comando
  }
  
//---------------------------------------------------------------------------------
  rotina_obstaculo(); // executa a funcao para desvio de obstaculo , caso exista
  
  if (comando.length() >= 0) {  //se houver algum comando de voz
    Serial.println(comando);  // efetua um serial print do comando

      if(comando == "frente"){  // se a string for 'frente'
        frente();}    // executa a funcao frente()
        
      else if(comando == "volta"){  // se a string for 'volta'
        volta();}  // executa a funcao volta()

      else if (comando == "direita"){  // se a string for 'direita'
        direita();}  // executa a funcao direita()

      else if (comando == "esquerda"){ // se a string for 'esquerda'
        esquerda();}  // executa a funcao esquerda()

      else if (comando == "para"){  // se a string for 'para'
        para();}  // executa a funcao para()

      comando=""; //Reseta a variavel
  }
}
