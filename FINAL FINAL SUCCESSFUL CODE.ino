


#include <M5Core2.h>

const int refresh = 3000; //read every 3 seconds

double SH, SW; //screenheight and screenwidth

double gender; //gender
double age; //age
double weight; //weight

unsigned int unit = 3;
//              0 = C (celsius)
//              1 = F (fahrenheit)
//              2 = Humidity
//              3 = C and Humidity
//              4 = F and Humidity
//              5 = C and F

char *title[] = {"Temperature", "Temperature" ,"Humidity"};
char *unitText[] = {"C", "F", "%"};

unsigned int backgroundColor = BLUE;

unsigned int temperatureTitleColor = WHITE;
unsigned int temperatureValueColor = RED;

unsigned int background2Color = GREEN;

unsigned int humidityTitleColor = WHITE;
unsigned int humidityValueColor = RED;

uint8_t titleV, unitV, degree;

uint8_t line1TitleY = 3;
uint8_t line1ValueY = 40;

uint8_t line2TitleY = 110;
uint8_t line2ValueY = 150;

bool t = true;
bool s1 = true;
bool s2 = false;
bool s3 = false;
bool s4 = false;
bool s5 = false;
bool s6 = false;

#include "DHT.h"
#define DHTPIN 27 
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);
float temperatureCValue, temperatureFValue, humidityValue, myValue1, myValue2;

void readTemperature();
void displayTemperature(uint8_t line);

void setup() {

  M5.begin();
  dht.begin(); 
  delay(1000);
  M5.IMU.Init();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);  
  M5.Lcd.setTextSize(2); 

  Serial.begin(9600); 
  Serial.println("M5Stack DHT Test");

  SH = M5.Lcd.height();
  SW = M5.Lcd.width();

  srand(time(0));  
    
}

//save temperature and humidity values
void readTemperature()
{
  temperatureCValue = dht.readTemperature(); //read temperature as Celsius (the default)
  humidityValue = dht.readHumidity(); //reading humidity 
  temperatureFValue = dht.readTemperature(true); //read temperature as Fahrenheit (true -> farenheit)

}

//display appropriate value depending on unit
void displayMyValue()
{
  
  switch (unit) {
    case 1:
      titleV = 1;
      unitV = 1; 
      myValue1 = temperatureFValue;    
      displayTemperature(1 , true);    
    break;

    case 2:
      titleV = 2;
      unitV = 2;   
      myValue1 = humidityValue;  
     displayTemperature(1, false );    
    break;

    case 3:
      titleV = 0;
      unitV =0;     
      myValue1= temperatureCValue;      
      displayTemperature(1, true);
 
      titleV = 2;
      unitV =2;  
      myValue2= humidityValue; 
      displayTemperature(2, false );               
    break;

    case 4:
      titleV = 1;
      unitV = 1;     
      myValue1 = temperatureFValue;      
      displayTemperature(1 , true);
 
      titleV = 2;
      unitV = 2;  
      myValue2= humidityValue; 
      displayTemperature( 2 , false);      
    break;

    case 5:
      titleV = 0;
      unitV = 0;     
      myValue1 = temperatureCValue;      
      displayTemperature(1, true);
 
      titleV = 1;
      unitV = 1;  
      myValue2 = temperatureFValue; 
      displayTemperature(2 , true);         
    break;

    default:
      titleV = 0;
      unitV = 0;    
      myValue1 = temperatureCValue;
      displayTemperature(1, true);    
    break;
    
  }

 
}

//display temperature value
void displayTemperature(uint8_t line, bool degree)
{
  uint8_t Y_lineTitle,Y_lineValue;
  float valueToDisplay;
  //M5.Lcd.fillScreen(backgroundColor);
  M5.Lcd.setTextColor(temperatureTitleColor);  
 
  M5.Lcd.setTextSize(4);
  if(line ==1){
    Y_lineTitle =line1TitleY;
    Y_lineValue =line1ValueY;
    valueToDisplay=myValue1;
    //M5.Lcd.fillRect(0, 0, 340, line2TitleY, backgroundColor);            
   M5.Lcd.fillScreen(backgroundColor);     
  }else{
    //M5.Lcd.drawRect(0, line2TitleY, 340, 240-line2TitleY, background2Color);
    M5.Lcd.fillRect(0, line2TitleY, 340, 240-line2TitleY, background2Color);      
    Y_lineTitle =line2TitleY;
    Y_lineValue =line2ValueY; 
    valueToDisplay=myValue2;
  }

  M5.Lcd.setCursor(3,Y_lineTitle); 
 
  M5.Lcd.print(title[titleV]);
  M5.Lcd.setTextColor(temperatureValueColor);    
  M5.Lcd.setTextSize(7);  
  M5.Lcd.setCursor(3,Y_lineValue);  
  M5.Lcd.printf("%.1f", valueToDisplay);  
  if(degree){
   printDegree();//print degree symbol if needed
  }
  M5.Lcd.print(unitText[unitV]);    
}

//display humidity value
void displayHumidity()
{

  M5.Lcd.setTextColor(humidityTitleColor);      
  M5.Lcd.setTextSize(4);    
  M5.Lcd.setCursor(3,110); 
  M5.Lcd.print(title[2]); //humidity title   
  M5.Lcd.setTextColor(humidityValueColor);    
  M5.Lcd.setTextSize(7);  
  M5.Lcd.setCursor(3,150);   
  M5.Lcd.printf("%.1f", humidityValue);       
  M5.Lcd.print(unitText[2]);    

}


//print degree
void printDegree()
{

  M5.Lcd.setTextSize(3);  
  
  M5.Lcd.print("o");  
  M5.Lcd.setTextSize(7);    
  M5.Lcd.setCursor(M5.Lcd.getCursorX()+15,M5.Lcd.getCursorY());   
}

//print title and unit
void printOnSerial()
{
      Serial.print(title[unit]); 
      Serial.print(" "); 
      Serial.print(myValue1);  
             
      Serial.print(unitText[unit]);      
      Serial.println();  
}

//detect press on screen
bool isTouched(float x, float y, float r){

TouchPoint_t coordinate = M5.Touch.getPressPoint();
float d = sqrt(pow((x-(coordinate.x)),2) + pow((y-(coordinate.y)),2));

if (coordinate.x < 0 || coordinate.y < 0){
  t = true;
  return false;
}
else{
  if (d < r && t){
    t = false;
    return true;
  }

else{
  return false;
}  
}
}

//display screen 1
void screen1(){
  
    M5.Lcd.setCursor(0.1 * SW, 0.03 * SH);
    M5.Lcd.setTextSize(4);
    M5.Lcd.printf("TEMPERATURE    HUMIDITY");

    M5.Lcd.setCursor(0.2 * SW, 0.6 * SH);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("PRESS TO MEASURE");

    M5.Lcd.fillCircle(0.5 * SW, 0.85 * SH, 0.05 * SW, RED);

}

//display screen 3
void screen3(){
  
    M5.Lcd.setCursor(0.275 * SW, 0.03 * SH);
    M5.Lcd.setTextSize(5);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.printf("GENDER");

    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(BLUE, BLACK);

    M5.Lcd.setCursor(0.38 * SW, 0.6 * SH);
    M5.Lcd.printf("MALE");

    M5.Lcd.setCursor(0.75 * SW, 0.6 * SH);
    M5.Lcd.printf("FEMALE");

    M5.Lcd.fillCircle(0.28 * SW, 0.6 * SH, 0.05 * SW, WHITE);
    M5.Lcd.fillCircle(0.67 * SW, 0.6 * SH, 0.05 * SW, WHITE);
  
}


//display screen 4
void screen4(){
  
    M5.Lcd.setCursor(0.30 * SW, 0.03 * SH);
    M5.Lcd.setTextSize(5);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.printf("AGE");

    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(BLUE, BLACK);

    M5.Lcd.setCursor(0.10 * SW, 0.6 * SH);
    M5.Lcd.printf("16-24");

    M5.Lcd.setCursor(0.40 * SW, 0.6 * SH);
    M5.Lcd.printf("25-39");

    M5.Lcd.setCursor(0.72 * SW, 0.6 * SH);
    M5.Lcd.printf("40+");

    M5.Lcd.fillCircle(0.05 * SW, 0.6 * SH, 0.05 * SW, WHITE);
    M5.Lcd.fillCircle(0.33 * SW, 0.6 * SH, 0.05 * SW, WHITE);
    M5.Lcd.fillCircle(0.66 * SW, 0.6 * SH, 0.05 * SW, WHITE);

}


//display screen 5
void screen5(){
  
    M5.Lcd.setCursor(0.28 * SW, 0.03 * SH);
    M5.Lcd.setTextSize(5);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.printf("WEIGHT");

    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(BLUE, BLACK);

    M5.Lcd.setCursor(0.10 * SW, 0.6 * SH);
    M5.Lcd.printf("40-60");

    M5.Lcd.setCursor(0.40 * SW, 0.6 * SH);
    M5.Lcd.printf("61-80");

    M5.Lcd.setCursor(0.72 * SW, 0.6 * SH);
    M5.Lcd.printf("80+");

    M5.Lcd.fillCircle(0.05 * SW, 0.6 * SH, 0.05 * SW, WHITE);
    M5.Lcd.fillCircle(0.33 * SW, 0.6 * SH, 0.05 * SW, WHITE);
    M5.Lcd.fillCircle(0.66 * SW, 0.6 * SH, 0.05 * SW, WHITE);

}


//display screen 6
void screen6(){

if (temperatureCValue > 15 && temperatureCValue <= 41 && humidityValue > 25 && humidityValue < 70){

    M5.Lcd.setCursor(0.01 * SW, 0.03 * SH);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("Perform yoga poses: mountain - child's pose - downward dog - warrior - corpse - cat/cow\n\n");
}
else {
    M5.Lcd.printf("Unsafe to do yoga in these conditions");
}

if (temperatureCValue > 15 && temperatureCValue <= 41 && humidityValue > 25 && humidityValue < 70){

  M5.Lcd.setCursor(0.01 * SW, 0.03 * SH);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);

  if (gender == 1){
    if (age == 1){
      if (weight == 1){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 4 sets of shoulder press, push-ups, pull-ups, sit-ups for 12 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }
      else if (weight == 2){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 5 sets of shoulder press, push-ups, pull-ups, sit-ups for 12 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 5 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 25 reps each\n");
      }
      else if (weight == 3){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 10 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 6 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }        
      }
    }
    else if (age == 2){
      if (weight == 1){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 10 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 25 reps each\n");
      }
      else if (weight == 2){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 8 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 5 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 30 reps each\n");
      }
      else if (weight == 3){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 6 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 6 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }        

    }
    else if (age == 3){
      if (weight == 1){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 8 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 3 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }
      else if (weight == 2){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 6 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 25 reps each\n");
      }
      else if (weight == 3){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 2 sets of shoulder press, push-ups, pull-ups, sit-ups for 6 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 5 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }        

    }
  }
  else if (gender == 2){
    if (age == 1){
      if (weight == 1){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 4 sets of shoulder press, push-ups, pull-ups, sit-ups for 12 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 5 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 15 reps each\n");
      }
      else if (weight == 2){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 4 sets of shoulder press, push-ups, pull-ups, sit-ups for 10 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 5 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }
      else if (weight == 3){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 10 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 15 reps each\n");
      }        
      }
    }
    else if (age == 2){
      if (weight == 1){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 10 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }
      else if (weight == 2){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 8 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 5 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 25 reps each\n");
      }
      else if (weight == 3){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 6 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 15 reps each\n");
      }        

    }
    else if (age == 3){
      if (weight == 1){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 6 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 15 reps each\n");
      }
      else if (weight == 2){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 3 sets of shoulder press, push-ups, pull-ups, sit-ups for 4 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 4 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 20 reps each\n");
      }
      else if (weight == 3){
        M5.Lcd.setCursor(0.01 * SW, 0.35 * SH);
        M5.Lcd.printf("Workout: Perform 2 sets of shoulder press, push-ups, pull-ups, sit-ups for 4 reps each\n\n");

        M5.Lcd.setCursor(0.01 * SW, 0.72 * SH);
        M5.Lcd.printf("Cardio: Perform 3 sets of jumping jacks, mountain climbers, skater, squats and shadow boxing for 15 reps each\n");
      }        

    }

else {
  M5.Lcd.printf("Unsafe to workout in these conditions\n\n");
  M5.Lcd.printf("Unsafe to do cardio in these conditions\n");
}

}


//loop that keeps repeating
void loop() {

  M5.update();
  while (s1) {
    screen1();
    if (isTouched(0.5 * SW, 0.85 * SH, 0.05 * SW)){      
    s1 = false;
    s2 = true;
    if (s2){
      M5.Lcd.clearDisplay();
      M5.Lcd.fillScreen(BLACK);
      readTemperature(); //reads the temperature and humidity
      displayMyValue(); //displays temperature or humidity
      delay(5000); //refresh
      M5.Lcd.setCursor(0.80 * SW, 0.58 * SH);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.printf("NEXT");
      M5.Lcd.fillCircle(0.85 * SW, 0.45 * SH, 0.05 * SW, BLACK);
      
      }
    }
  }

  if (isTouched(0.8 * SW, 0.5 * SH, 0.05 * SW)){
    s1 = false;
    s2 = false;
    s3 = true;
    M5.Lcd.fillScreen(BLACK);
    delay(1000);
    if (s3){
      screen3();
  }
  }
  
   if (isTouched(0.28 * SW, 0.6 * SH, 0.05 * SW)){
    gender = 1;
    s3 = false;
    s4 = true;
    M5.Lcd.fillScreen(BLACK);
    delay(1000);
    if (s4){
      screen4();
  }
  }
  
  if (isTouched(0.67 * SW, 0.6 * SH, 0.05 * SW)){
    gender = 2;
    s3 = false;
    s4 = true;
    M5.Lcd.fillScreen(BLACK);
    delay(1000);
    if (s4){
      screen4();
  }
  }
  
  if (s4) {
    if (isTouched(0.05 * SW, 0.6 * SH, 0.05 * SW)){
    age = 1;
    s4 = false;
    s5 = true;
    M5.Lcd.fillScreen(BLACK);
    delay(1000);
    if (s5){
      screen5();
  }
  }

  else if (isTouched(0.33 * SW, 0.6 * SH, 0.05 * SW)){
    age = 2;
    s4 = false;
    s5 = true;
    M5.Lcd.fillScreen(BLACK);
    delay(1000);
    if (s5){
      screen5();
  }
  }

  else if (isTouched(0.66 * SW, 0.6 * SH, 0.05 * SW)){
    age = 3;
    s4 = false;
    s5 = true;
    M5.Lcd.fillScreen(BLACK);
    delay(1000);
    if (s5){
      screen5();
    }
  }
  }

  if (s5){
    if (isTouched(0.05 * SW, 0.6 * SH, 0.05 * SW)){
      weight = 1;
      s5 = false;
      s6 = true;
      M5.Lcd.fillScreen(BLACK);
      delay(1000);
      if (s6){
        screen6();
      }
    }
    else if (isTouched(0.33 * SW, 0.6 * SH, 0.05 * SW)){
      weight = 2;
      s5 = false;
      s6 = true;
      M5.Lcd.fillScreen(BLACK);
      delay(1000);
      if (s6){
        screen6();
      }
    }
    else if(isTouched(0.66 * SW, 0.6 * SH, 0.05 * SW)){
      weight = 3;
      s5 = false;
      s6 = true;
      M5.Lcd.fillScreen(BLACK);
      delay(1000);
      if (s6){
        screen6();
      }
    }
  }

}

