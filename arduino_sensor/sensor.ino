#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN     13

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int perc = 50;
const int sampleWindow = 100; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample = 0;
int lastperc = 0;

void setup()
{
  Serial.begin(9600);
  
  // Switch on the backlight
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  
  lcd.begin(16,2);               // initialize the lcd 

  lcd.home ();                   // go home
  lcd.print("Library Noise ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("Monitoring ");
  
  int perc = random(0,100);
  delay ( 1000 );
  lcd.setCursor ( 0, 1 );
  lcd.print ("                ");
}

void loop()
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int numSamples = 0;
   unsigned int total = 0;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         total = total + sample;
         numSamples++;
      }
   }

   Serial.println("");
   Serial.print("total: ");
   Serial.print(total);
   Serial.print(" average: ");
   long average = total/numSamples;
   Serial.print(average);

   float div = (float)(average-17.0) / (float)(200.0-17.0);
   perc = (int)(div * 100);

   Serial.print(" perc: ");
   Serial.print(perc);   

   if(perc > 100) {
    perc = 100;
   }
   if(perc < 0) {
    perc = 0;
   }

   lastperc = perc;

   perc = (lastperc + perc) / 2;
  
   lcd.setCursor ( 0, 1 );

   int hashes = (int) (perc / 10);

   lcd.print ("[");

   int length = 0;
   for(int i = 0; i < hashes; i++) {
      lcd.print ("#");
      length++;
   }

   for(int pad = length; pad < 10; pad++) {
      lcd.print (" ");
   }
   
   lcd.setCursor ( 11, 1 );
   lcd.print("] ");
   
   // pad with space
   if(perc < 10) {
     lcd.print(" ");
   }
   lcd.print(perc);
   lcd.print("%");
  
   
   delay (800);
}
