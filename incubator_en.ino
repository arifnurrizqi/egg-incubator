/*------------------------------------------------Incubator Project
-----------------------------------------------------------------*/

#include "DHT.h"

#define DHTPIN 2 // define the pin used for the DHT11 sensor

// Determine the type of DHT used (choose one)
// Currently selected is DHT11
#define DHTTYPE DHT11 // DHT 11 


DHT dht(DHTPIN, DHTTYPE);

#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  8   // pin used is pin 8
#define RELAY_2  9   // pin used is pin 9


#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define suhuon 11
#define suhuoff 12

unsigned long firsttime = 0;
unsigned long firsttime2 = 0;
int relayState = LOW;

void setup() {
    
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  dht.begin();
  
 // Set pin as output.
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(suhuon, OUTPUT);
  pinMode(suhuoff, OUTPUT);
   
 // Initialize relay one as off so that on reset it would be off by default
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_2, RELAY_OFF);

  lcd.begin(16, 2);
  lcd.setCursor(4,0);
  lcd.print("PROJECT");
  lcd.setCursor(3,1);
  lcd.print("INCUBATOR");
  delay(5000);
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("Di Buat Oleh");
  lcd.setCursor(1,1);
  lcd.print("Arif Nur Rizqi");
  delay(5000);
  lcd.clear();

  pinMode(13, OUTPUT);  // turn on the fan using pin 13
}
void loop(){
  
  int relaytime= 10;
  int Onrelaytime= 10000;
  int Offrelaytime= 20000;
    
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
  if (h<55)//Fan ON
{
  digitalWrite(13, HIGH);
}
  else if (h>60)//Fan OFF
{ 
  digitalWrite(13, LOW);
}
  
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %t");
    
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");

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

    if((relayState == HIGH) && (lasttime2 - 
    firsttime2 >= Offrelaytime))

        {
        relayState = LOW; // Turn it off

        firsttime2 = lasttime2; // Remember the time

        digitalWrite(RELAY_2, relayState); // Update the actual RELAY

        }
    else if ((relayState == LOW) && (lasttime2 - 
    firsttime2 >= Onrelaytime))

        {
        relayState = HIGH; // turn it on
        firsttime2 = lasttime2; // Remember the time
        digitalWrite(RELAY_2, relayState);	 // Update the actual RELAY
        }
}