#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

/* Uncomment this block to use hardware SPI
// If using software SPI (the default case):
#define OLED_MOSI   D0
#define OLED_CLK    D1
#define OLED_DC     D2
#define OLED_CS     D3
#define OLED_RESET  D4
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
*/

#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//-----------------------------------------//
//THIS IS THE START OF MY CODE FOR THIS BIT//
//-----------------------------------------//

long readInterval = 30000;
unsigned long previousMillis = 0;
unsigned long currentMillis;

int currentTime;
int currentHour;
int previousHour;

int outputPin = D7;
int vibeInterval = 100;

int vibe(String command);


//Cities can be altered to whatever you desire
String cities[] = {"Pacific Ocean", "Sydney", "Brisbane", "Tokyo", "Hong Kong", "Bangkok",
                  "Bangladesh", "Islamabad", "Dubai", "Moscow", "Athens", "Stockholm",
                  "Greenwich", "Ittoqqortoormiit", "Rio de Janeiro", "Buenos Aires", "Halifax", "Toronto",
                  "Winnipeg", "Albuquerque", "Las Vegas", "Anchorage", "Honolulu", "Aukland"}; //do another array of random cities in europe to replace Stockholm (Venice, Berlin, Prague, Paris, Bimbo, etc...)

void setup()   {
  Serial.begin(9600);

  //Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC);

  //show some starting text. Not sure if this bit is working right now
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("5 O'Clock");
  display.println("Somwhere");
  display.display();
  delay(2000);
  display.clearDisplay();

  //This isn't used right now, but you can add something to this pin to make it go off when it is 5 in your zone
  pinMode(D7, OUTPUT);

  //Initialize time
  Time.zone(-5);

  currentTime = Time.now();

  currentHour = Time.hour(currentTime);

  //debugging for the current tiem
  Particle.publish("InitializeHour", String(currentHour), PRIVATE);
  Particle.publish("InitializeTime", String(currentTime), PRIVATE);

  //makes the function that is called when it is 5 actionable from the terminal
  Particle.function("vibe", vibe);
}


void loop() {

    //reset the timezone, just in case
    Time.zone(-5);

    //set current time
    currentTime = Time.now();

    //set the current hour
    currentHour = Time.hour(currentTime);

    //debugging
    Particle.publish("currentHour", String(currentHour), PRIVATE);
    Particle.publish("currentTime", String(currentTime), PRIVATE);

    //this is the function that gets called when its 5
    if(currentHour == 19 && previousHour == 18){
      vibeOne();
    }

  //Display the text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("It's 5 o'clock in");
  display.println(" ");

  //checks the length of the city, so that it can be reduced to fit on the screen
  int currentCityLength = cities[currentHour].length();

  if(currentCityLength > 10){
    display.setTextSize(1);
  } else {
    display.setTextSize(2);
  }

  display.println(cities[currentHour]);

  //display the current time underneath the city
  showTime();

  display.display();
  display.clearDisplay();

  //for the 5 oclock function
  previousHour = currentHour;

}

int vibe(String command){
  if(command == "on"){
      vibeOne();

      return 1;
  }

  else {

    return 0;

  }
}

void vibeOne(){
  digitalWrite(outputPin, HIGH);
  delay(vibeInterval);
  digitalWrite(outputPin, LOW);
  delay(vibeInterval);

  digitalWrite(outputPin, HIGH);
  delay(vibeInterval);
  digitalWrite(outputPin, LOW);
  delay(vibeInterval);

  digitalWrite(outputPin, HIGH);
  delay(vibeInterval);
  digitalWrite(outputPin, LOW);
  delay(vibeInterval);
}

void showTime(){
  int currentHour = Time.hour();
  int currentMinute = Time.minute();
  bool isItAm = Time.isAM();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,45);
  if(currentHour > 12){
    currentHour = (currentHour - 12);
  }
  display.print(currentHour);
  display.print(":");
  if(currentMinute < 10){
    display.print("0");
    display.print(currentMinute);
  } else{
  display.print(currentMinute);
}
    if(isItAm){
      display.print("am");
    } else {
      display.print("pm");
    }
}
