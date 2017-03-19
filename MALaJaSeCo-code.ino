#include <Servo.h>
#include <PID_v1.h>

//Variaveis do PID#######################################
double SetPoint = 500, Input = 0, Output = 0;
double kp = 0.011, ki = 0.23, kd = 0.0002;
//double kp = 0.006336, ki = 0.1, kd = 0.0025; //Testando esses valores
byte pinoLDR = A7;
byte led = 13;
String InSetPointPortSerial = "";
boolean flag = false;
unsigned int error = 0;
PID myPID(&Input, &Output, &SetPoint, kp, ki, kd, DIRECT);
//#########################################################

int btnAutManLed = 22;  // botão Manual/Automático do LED
int valInstantaneoBtnAutManLed = 0;       // Guarda o valor do botao a cada loop
int valGuardadoBtnAutManLed = 0;     // Variavel que vai ser usada para verificar se o estado do botao alterou
int estadoBtnAutManLed = 0;  // guarda o valor 0 ou 1 (HIGH ou LOW)

int btnAutManJanela = 23;  // botão Manual/Automático da janela
int valInstantaneoBtnAutManJanela = 0;       // Guarda o valor do botao a cada loop
int valGuardadoBtnAutManJanela = 0;     // Variavel que vai ser usada para verificar se o estado do botao alterou
int estadoBtnAutManJanela = 0;  // guarda o valor 0 ou 1 (HIGH ou LOW)

int btnLigaDesligaLed = 24;  // botão Liga/desliga do LED
int valInstantaneoBtnLigaDesligaLed = 0;       // Guarda o valor do botao a cada loop
int valGuardadoBtnLigaDesligaLed = 0;     // Variavel que vai ser usada para verificar se o estado do botao alterou
int estadoBtnLigaDesligaLed = 0;  // guarda o valor 0 ou 1 (HIGH ou LOW)

int btnAbreFechaJanela = 25;  // botão Abre/Fecha janela
int valInstantaneoBtnAbreFechaJanela = 0;       // Guarda o valor do botao a cada loop
int valGuardadoBtnAbreFechaJanela = 0;     // Variavel que vai ser usada para verificar se o estado do botao alterou
int estadoBtnAbreFechaJanela = 0;  // guarda o valor 0 ou 1 (HIGH ou LOW)

int pinLed = 13;
int pinSensorChuva = 52;
int pinSensorChuvaAnalogico = A0;
int valorSensorChuva = 0;
int valorSensorChuvaAnalogico = 1023;
Servo myservo;
  
void setup() {
  pinMode(btnAutManLed, INPUT);
  pinMode(btnAutManJanela, INPUT);
  pinMode(btnLigaDesligaLed, INPUT);
  pinMode(btnAbreFechaJanela, INPUT);

  pinMode(pinSensorChuva, INPUT);

  pinMode(pinLed, OUTPUT);
  myservo.attach(2);

  myservo.write(179);

  pinMode(led, OUTPUT);
  analogWrite(led, 0);
  myPID.SetMode(AUTOMATIC);
  
  Serial.begin(9600);
}

void loop() {
/*
      ##Verifica botao Manual/Automatico LED
     */
    valInstantaneoBtnAutManLed = digitalRead(btnAutManLed); // ler o valor enviado pelo botão: "HIGH" ou "LOW"
    if ((valInstantaneoBtnAutManLed == HIGH) && (valGuardadoBtnAutManLed == LOW)) {
        estadoBtnAutManLed = 1 - estadoBtnAutManLed;
    }
    valGuardadoBtnAutManLed = valInstantaneoBtnAutManLed;
    if (estadoBtnAutManLed == 1) {

        while (Serial.available() > 0) //verifica se tem dados diponível para leitura
        {
            char c = Serial.read();
            InSetPointPortSerial += c;
            flag = true;
        }
        if (flag) {
            SetPoint = (double) InSetPointPortSerial.toInt();
            InSetPointPortSerial = "";
            flag = false;
        }
        Input = analogRead(pinoLDR);
        myPID.Compute();
        error = (unsigned int) Input - SetPoint;
        if( error > 30 ){//IF OPCIONAL
          analogWrite(led, Output);
        }
        Serial.print((int) SetPoint);Serial.print("    ");Serial.print((int) Input);Serial.print("    ");Serial.println((int) Output);
        delay(100);
    } else {
        /*
    ##Verifica botao Liga/Desliga LED
         */
        valInstantaneoBtnLigaDesligaLed = digitalRead(btnLigaDesligaLed); // ler o valor enviado pelo botão: "HIGH" ou "LOW"
        if ((valInstantaneoBtnLigaDesligaLed == HIGH) && (valGuardadoBtnLigaDesligaLed == LOW)) {
            estadoBtnLigaDesligaLed = 1 - estadoBtnLigaDesligaLed;
        }
        valGuardadoBtnLigaDesligaLed = valInstantaneoBtnLigaDesligaLed;
        if (estadoBtnLigaDesligaLed == 1) {
            digitalWrite(pinLed, HIGH); // liga o led
        } else {
            digitalWrite(pinLed, LOW); // desliga o led
        }
    }


    /*
    ##Verifica botao Manual/Automatico Janela
     */
    valInstantaneoBtnAutManJanela = digitalRead(btnAutManJanela); // ler o valor enviado pelo botão: "HIGH" ou "LOW"
    if ((valInstantaneoBtnAutManJanela == HIGH) && (valGuardadoBtnAutManJanela == LOW)) {
        estadoBtnAutManJanela = 1 - estadoBtnAutManJanela;
    }
    valGuardadoBtnAutManJanela = valInstantaneoBtnAutManJanela;
    if (estadoBtnAutManJanela == 1) {
        //valorSensorChuva = digitalRead(pinSensorChuva);
        valorSensorChuvaAnalogico = analogRead(pinSensorChuvaAnalogico);
        if (valorSensorChuvaAnalogico < 800) {
            myservo.write(179); //Fecha Janela
        } else {
            //if (estadoBtnAbreFechaJanela == 1) {
                myservo.write(0); // Abre janela
            //} else {
            //    myservo.write(179); //Fecha janela
            //}
        }

    } else {
        /*
   ##Verifica botao Abre/Fecha Janela
         */
        valInstantaneoBtnAbreFechaJanela = digitalRead(btnAbreFechaJanela); // ler o valor enviado pelo botão: "HIGH" ou "LOW"
        if ((valInstantaneoBtnAbreFechaJanela == HIGH) && (valGuardadoBtnAbreFechaJanela == LOW)) {
            estadoBtnAbreFechaJanela = 1 - estadoBtnAbreFechaJanela;
        }
        valGuardadoBtnAbreFechaJanela = valInstantaneoBtnAbreFechaJanela;
        if (estadoBtnAbreFechaJanela == 1) {
            myservo.write(0); // Abre janela
        } else {
            myservo.write(179); //Fecha janela
        }
    }

    /*Serial.print("  btn manual/automatico LED: ");
    Serial.print(estadoBtnAutManLed);
    Serial.print("  btn manual/automatico Janela: ");
    Serial.print(estadoBtnAutManJanela);
    Serial.print("  btn Liga/desliga LED: ");
    Serial.print(estadoBtnLigaDesligaLed);
    Serial.print("  btn Abre/Fecha Janela: ");
    Serial.print(estadoBtnAbreFechaJanela);
    Serial.println();*/

    //Serial.print(valorSensorChuva);Serial.print("     ");Serial.print(valorSensorChuvaAnalogico);
    //Serial.println();
  
}
