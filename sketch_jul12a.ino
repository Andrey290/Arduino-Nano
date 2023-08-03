#include <avr/io.h>
#include <avr/interrupt.h>

#include <OLED_I2C.h>                               // Подключаем библиотеку OLED_I2C для работы со шрифтами и графикой
OLED  myOLED(A4, A5, A4);                           // Определяем пины I2C интерфейса: UNO и NANO -- SDA - пин A4, SCL - пин A5; MEGA -- SDA - пин 20, SCL - пин 21
extern uint8_t RusFont[];                           // Подключаем русский шрифт
extern uint8_t SmallFont[];    
char strin[30];
char stru[30];
int integer_part = 0;
int fravtional_part = 0;

// код для выключателя (не включателя)
const int button_pin = 2;
int button_state = HIGH;
int button_prestate = HIGH;
int button_count = 0;

//код для D6 оптопара
const int optopara_pin = 3;

void setup() {
  pinMode(button_pin, INPUT);
  pinMode(optopara_pin, OUTPUT);
  Serial.begin(9600); 

// Пины D3 и D11 - 4 кГц
TCCR2B = 0b00000010;  // x8
TCCR2A = 0b00000001;  // phase correct

  // инициализация Timer1
  cli();  // отключить глобальные прерывания
  TCCR1A = 0;   // установить регистры в 0
  TCCR1B = 0;

  OCR1A = 15624; // установка регистра совпадения

  TCCR1B |= (1 << WGM12);  // включить CTC режим 
  TCCR1B |= (1 << CS10); // Установить биты на коэффициент деления 1024
  TCCR1B |= (1 << CS12);

  TIMSK1 |= (1 << OCIE1A);  // включить прерывание по совпадению таймера 
  sei(); // включить глобальные прерывания

  // ДИСПЛЕЙ //
  myOLED.begin();
}

boolean debounse(boolean last) {
	boolean current = digitalRead(button_pin);
	if(last != current) {
		//delay(5);
		current = digitalRead(button_pin);
	}
	return current;
}

void loop() {
  button_state = debounse(button_prestate);
  if (button_state == LOW && button_prestate == HIGH) {
    button_count++;
  }
  button_prestate = button_state;
  
  int sensorValue = analogRead(A1);
  float value = sensorValue * (255 / 1023.0);
  analogWrite(optopara_pin, value);
  integer_part = (int)value;
  //fravtional_part = (int)(value * 10) % 100;
  Serial.println(integer_part);
  sprintf(stru, "%s%i", "IBV ", integer_part);
  
  //ДИСПЛЕЙ//
  myOLED.clrScr();                                    // Стираем все с экрана
  myOLED.setFont(RusFont);                            // Инициализируем русский шрифт
  myOLED.print("GHBDTN VBH!", LEFT, 0);
  myOLED.print(strin, LEFT, 24);          
  myOLED.print(stru, LEFT, 36);  
  myOLED.update();                                    // Обновляем информацию на дисплее
}

ISR(TIMER1_COMPA_vect)
{
  Serial.println(button_count);
  sprintf(strin,"%s%i","Xfcnjnf(j,/vby) ", button_count * 60);

  button_count = 0;

  //ДИСПЛЕЙ//
  myOLED.clrScr();                                    // Стираем все с экрана
  myOLED.setFont(RusFont);                            // Инициализируем русский шрифт
  myOLED.print("GHBDTN VBH!", LEFT, 0);
  myOLED.print(strin, LEFT, 24);
  myOLED.print(stru, LEFT, 36);
  myOLED.update();                                    // Обновляем информацию на дисплее
}