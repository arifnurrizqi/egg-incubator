/*---------------------Incubator Project--------------------------
-----------------------------------------------------------------*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 7 // define the pin used for the DHT11 sensor

// Determine the type of DHT used (choose one)
// Currently selected is DHT22
// #define DHTTYPE DHT11 // DHT 11 
#define DHTTYPE DHT22 // DHT 22 

DHT dht(DHTPIN, DHTTYPE);

#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  8   // pin used is pin 8
#define RELAY_2  9   // pin used is pin 9

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C untuk LCD 16x2

#define suhuon 11
#define suhuoff 12

unsigned long firsttime = 0;
unsigned long firsttime2 = 0;
int relayState = LOW;

void setPin() {
  // Set pin as output.
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(suhuon, OUTPUT);
  pinMode(suhuoff, OUTPUT);
}

void defaultPinValue() {
  // Initialize relay one as off so that on reset it would be off by default
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_2, RELAY_OFF);
}

void firstLcdDisplay() {
  lcd.setCursor(4,0);
  lcd.print("PROJECT");
  lcd.setCursor(3,1);
  lcd.print("INCUBATOR");
  delay(5000);
  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print("Made by");
  lcd.setCursor(1,1);
  lcd.print("ARIF NUR RIZQI");
  delay(5000);
  lcd.clear();
}

void setup() {
    
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  lcd.init();  // Tambahkan ini untuk versi library tertentu
  lcd.backlight();
  dht.begin();
  
  setPin(); // Set pin as output.
  
  defaultPinValue(); // Initialize relay one as off so that on reset it would be off by default

  firstLcdDisplay(); // LCD setup display
}

void loop(){
  int relaytime= 50;
  int Onrelaytime= 12500; // long time rotate eggs (depending on the rpm of the motor)
  unsigned long Offrelaytime= 10800000; // delay 3 hours rotate eggs
    
  unsigned long lasttime= millis();

  if(lasttime-firsttime>=relaytime){
    // Read humidity dan temperature
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check the reading results, and display if ok
    if (isnan(t) || isnan(h)) {
        Serial.println("Failed to read from DHT");
        return;
    }

    if (t<37.77)// ON
    {
        digitalWrite(RELAY_1, RELAY_ON);
        digitalWrite(suhuon, HIGH);
        digitalWrite(suhuoff, LOW);
    }
    else if (t>38.33)//OFF
    {
        digitalWrite(RELAY_1, RELAY_OFF);
        digitalWrite(suhuoff, HIGH); 
        digitalWrite(suhuon, LOW);
    }

    lcd.setCursor(0,0);
    lcd.print("Humi: "); 
    lcd.print(h);
    lcd.print(" %      ");
    lcd.setCursor(0,1);
    lcd.print("Temp: "); 
    lcd.print(t);
    lcd.print(" C      ");

    firsttime= millis();
}

  unsigned long lasttime2= millis();

  if((relayState == HIGH) && (lasttime2 - firsttime2 >= Offrelaytime)) {
      relayState = LOW; // Turn it off
      firsttime2 = lasttime2; // Remember the time
      digitalWrite(RELAY_2, relayState); // Update the actual RELAY
  }
  else if ((relayState == LOW) && (lasttime2 - firsttime2 >= Onrelaytime)) {
      relayState = HIGH; // turn it on
      firsttime2 = lasttime2; // Remember the time
      digitalWrite(RELAY_2, relayState);	 // Update the actual RELAY
  }
}
