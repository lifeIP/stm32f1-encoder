// для удобства переименовываем пины
#define A PB12
#define B PB13
#define C PB14
#define D PB15
#define E PB11
#define F PB10
#define G PB1

#define D4 PB6
#define D3 PB5
#define D2 PB4

#define S1 PB9
#define S2 PB8
#define KEY PB7
// для удобства переименовываем пины

// подключаем библиотеку для работы с EEPROM
#include <EEPROM.h>


// определение символа (цифры)
void pickNumber(int x) {
  switch (x) {
    default: zero();    break;
    case 1: one();      break;
    case 2: two();      break;
    case 3: three();    break;
    case 4: four();     break;
    case 5: five();     break;
    case 6: six();      break;
    case 7: seven();    break;
    case 8: eight();    break;
    case 9: nine();     break;
  }
}

// очистка индикатора
void clearLEDs() {
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void zero() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, LOW);
}

void one() {
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void two() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
}

void three() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
}

void four() {
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void five() {
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void six() {
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void seven() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void eight() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void nine() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

// задаём участкам памяти EEPROM свои ссылки с именами
EERef signal_1 = EEPROM[0];
EERef signal_2 = EEPROM[1];
EERef signal_3 = EEPROM[2];
// задаём участкам памяти EEPROM свои ссылки с именами


#define ENC_TYPE 0    // тип энкодера, 0 или 1
volatile int encCounter;
volatile boolean state0, lastState;
volatile int flag;

// объявляем функции, которые будут описаны ниже в коде
void print(int num);
void setSignal(int8_t signalNum, int16_t value);
uint16_t getSignal(int8_t signalNum);
void int0(); // это программное прерывание
void button(); 
// объявляем функции, которые будут описаны ниже в коде

int statemax100;
int statemax400;
int statemax280;

void setup() {
  
  // настраиваем порты ввода - вывода
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);

  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(KEY, INPUT_PULLUP);
  // настраиваем порты ввода - вывода

  flag = 1; // флаг значения. Он отвечает за то, какой диапазон будет на циферблате

  statemax100 = getSignal(1); // загружаем с EEPROM предыдущее значение
  statemax400 = getSignal(2); // загружаем с EEPROM предыдущее значение
  statemax280 = getSignal(3); // загружаем с EEPROM предыдущее значение

  attachInterrupt(0, int0, CHANGE); // настраиваем программное прерывание
}



void int0() {
  state0 = digitalRead(S1);
  encCounter += (digitalRead(S2) != lastState) ? -1 : 1;
  if(flag == 1) setSignal(1, statemax100 + encCounter);
  else if(flag == 2) setSignal(2, statemax400 + encCounter);
  else setSignal(3, statemax280 + encCounter);
  lastState = state0;
}

void loop() {
  button();
  delay(12);
  if(flag == 1) print(1);
  else if(flag == 2) print(2);
  else print(3);
}

void button(){
  if(digitalRead(KEY)) return;
  encCounter = 0;
  if(flag > 2) flag = 1;
  else flag++;
}

void print(int num){
  String data = String(getSignal(num));
  for(int i = 0; i < data.length(); i++){
    char ch = data.c_str()[i];
    if(i == 0) {
      digitalWrite(D4, 0);
      digitalWrite(D3, 1);
      digitalWrite(D2, 1);
    }else if(i == 1) {
      digitalWrite(D4, 1);
      digitalWrite(D3, 0);
      digitalWrite(D2, 1);
    }else if(i == 2) {
      digitalWrite(D4, 1);
      digitalWrite(D3, 1);
      digitalWrite(D2, 0);
    }
    if(ch == '0'){
      pickNumber(0);
    }else if(ch == '1'){
      pickNumber(1);
    }else if(ch == '2'){
      pickNumber(2);
    }else if(ch == '3'){
      pickNumber(3);
    }else if(ch == '4'){
      pickNumber(4);
    }else if(ch == '5'){
      pickNumber(5);
    }else if(ch == '6'){
      pickNumber(6);
    }else if(ch == '7'){
      pickNumber(7);
    }else if(ch == '8'){
      pickNumber(8);
    }else if(ch == '9'){
      pickNumber(9);
    }
    delay(2);

  }
}

void setSignal(int8_t signalNum, int16_t value){
  if(signalNum == 1 && (value >= 50 && value <= 100)){
    signal_1 = char(value);
  }
  else if(signalNum == 2 && (value >= 300 && value <= 400)){
    signal_2 = char(value - 255);
  }
  else if(signalNum == 3 && (value >= 200 && value <= 280)){
    signal_3 = char(value - 128);
  }
}

uint16_t getSignal(int8_t signalNum){
  if(signalNum == 1){
    return signal_1;
  }
  else if(signalNum == 2){
    return signal_2 + 255;
  }
  else if(signalNum == 3){
    return signal_3 + 128;
  }
  return 0;
}