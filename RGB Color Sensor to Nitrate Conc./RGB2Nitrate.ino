#include <Wire.h>                                                 //include the I2C library to communicate with the sensor
#include "Adafruit_TCS34725.h" 
#include <math.h>   //include the sensor library
#include <DS3231_Simple.h>
#include <SD.h>

DS3231_Simple Clock;
DateTime MyDateAndTime;

#define redpin 3                                                  //pwm output for RED anode use 1K resistor
#define greenpin 5                                                //pwm output for GREEN anode use 2K resistor
#define bluepin 6                                                 //pwm output for BLUE anode use 1K resistor
 
 
 
#define commonAnode false                                         // set to false if using a common cathode LED                     
 
const int chipSelect = 4; 
 
byte gammatable[256];                                             // our RGB -> eye-recognized gamma color
 
                                                                  //Create an instance of the TCS34725 Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
 
void setup() {
  Serial.begin(9600);                                             //Sart serial comms @ 9600 (you can change this)
  Serial.println("Color View Test");                              //Title info             
 
  if (tcs.begin()) {                                              //if the sensor starts correctly
    Serial.println("Found sensor");                               //print the happy message
  } else {                                                        //if the sensor starts incorrectly
    Serial.println("No TCS34725 found ... check your connections");//print the not so happy message
    while (1); // halt!
  }

  //initialize SD Card
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  
  
  pinMode(redpin, OUTPUT);                                         //set redpin for output
  pinMode(greenpin, OUTPUT);                                       //set greenpin for output
  pinMode(bluepin, OUTPUT);                                        //set bluepin for output
  
                                                                   // thanks PhilB for this gamma table!
                                                                   // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float a = i;
    a /= 255;
    a = pow(a, 2.5);
    a *= 255;
      
    if (commonAnode) {
      gammatable[i] = 255 - a;
    } else {
      gammatable[i] = a;      
    }
                                                                   //Serial.println(gammatable[i]);
  }
  Clock.begin();//begin the clock - make sure you set the TIME!
}
 
 
void loop() {
  delay(1000);
  readTheTime();
  uint16_t clear, red, green, blue;                                 //declare variables for the colors
 
  tcs.setInterrupt(false);                                          // turn on LED
 
  delay(60);                                                        // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);                      //read the sensor
 
  tcs.setInterrupt(true);                                           // turn off LED
  
 
 
                                                                    // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  /*Serial.print("r");
  Serial.print(r);
  Serial.print(g);
  Serial.print(b);*/
 
                                                                    //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );
  RGBtoNitrate(r, g, b);
  analogWrite(redpin, gammatable[(int)r]);                          //light red led as per calculation
  analogWrite(greenpin, gammatable[(int)g]);                        //light green led as per calculation
  analogWrite(bluepin, gammatable[(int)b]);                         //light blue led as per calculation
  
} 
void RGBtoNitrate(float r, float g, float b){
  double h, s, v;
  double x;
  h = RGBtoHSV(r, g, b);
  //print (h, s, v)
  if (h > 350 and h < 360){
      h -= 360.0;
  }
      //print("h:", h)
  //h = round(h);
  //Serial.print("final h: ");
  //Serial.println(h);
  if (h < 55.0 and h >= 40.0){
      x = remapRange(h, 55.0, 40.0, 0.0, 1.0);
  }
  else if (h < 40.0 and h >= 23.0){
      x = remapRange(h, 40.0, 23.0, 1.0, 2.0);
  }
  else if (h < 23.0 and h >= 20.0){
      x = remapRange(h, 23.0, 20.0, 2.0, 3.0);
  }
  else if( h < 20.0 and h >= 1.0){
      x = remapRange(h, 20.0, 1.0, 3.0, 4.0);
  }
  else if (h < 1.0 and h >= -1.0){
      x = remapRange(h, 1.0, -1.0, 4.0, 5.0);
  }
  else if (h < -1.0 and h >= -2.0){
      x = remapRange(h, -1.0, -2.0, 5.0, 6.0);
  }
  /*else if (h >= 55){
      x = 7.0;
      Serial.println(' ');
      Serial.print("I DON'T KNOW MAYBE THERE'S 1 NITRATE");
  }*/
  else if (h < -2.0){
      x = 7.0;
      Serial.println(' ');
      Serial.print("JUST STOP, YOU HAVE WAY TOO MUCH NITRATE STOP KILLING THE FISHHHHH!!!!!");
      
  }
  //Serial.print(x);
  if (x != 7.0){
    double nitrates = 2.4667 * pow(x, 2.00467);
    Serial.print("nitrate ppm: ");
    Serial.println(nitrates);
    SDWrite(nitrates);
  }
}


float remapRange(float val, float oldMin, float oldMax, float newMin, float newMax){
  /*
    Remaps a value from one range to another range.

    Args:
        val: A number form the old range to be rescaled.
        old_min: The inclusive 'lower' bound of the old range.
        old_max: The inclusive 'upper' bound of the old range.
        new_min: The inclusive 'lower' bound of the new range.
        new_max: The inclusive 'upper' bound of the new range.

    Note:
        min need not be less than max; flipping the direction will cause the sign of
        the mapping to flip.  val does not have to be between old_min and old_max.
    */
  float oldRange;
  float newValue;
  float newRange;
  oldRange = (oldMax - oldMin);
  if (oldRange == 0){
    newValue = newMin;
  }
  else{
    newRange = (newMax - newMin);
    newValue = (((val - oldMin) * newRange) / oldRange) + newMin;
  }
  return newValue;
}

double RGBtoHSV(float r, float g, float b){
    r, g, b = r/255.0, g/255.0, b/255.0;
  // R, G, B values are divided by 255
    // to change the range from 0..255 to 0..1
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;
 
    // h, s, v = hue, saturation, value
    double cmax = max(r, max(g, b)); // maximum of r, g, b
    double cmin = min(r, min(g, b)); // minimum of r, g, b
    double diff = cmax - cmin; // diff of cmax and cmin.
    double h = -1, s = -1;
 
    // if cmax and cmax are equal then h = 0
    if (cmax == cmin){
        h = 0.0;
        //Serial.print('hi');
    }
 
    // if cmax equal r then compute h
    else if (cmax == r){
        h = fmod((60 * ((g - b) / (diff)) + 360), 360);
        
    }
 
    // if cmax equal g then compute h
    else if (cmax == g){
        h = fmod((60 * ((b - r) / (diff)) + 120), 360);
        
    }
 
    // if cmax equal b then compute h
    else if (cmax == b){
        h = fmod((60 * ((r - g) / (diff)) + 240), 360);
        
    }
    //Serial.print("h: ");
    //Serial.print(h);
 
    // if cmax equal zero
    if (cmax == 0)
        s = 0;
    else
        s = (diff / cmax) * 100;
 
    // compute v
    double v = cmax * 100;
    return h;
}


void readTheTime(){
  // Ask the clock for the data.
  MyDateAndTime = Clock.read();
  
  // And use it
  Serial.println("");
  Serial.print("Time is: ");   Serial.print(MyDateAndTime.Hour);
  Serial.print(":"); Serial.print(MyDateAndTime.Minute);
  Serial.print(":"); Serial.println(MyDateAndTime.Second);
  Serial.print("Date is: ");   Serial.print(MyDateAndTime.Month);
  Serial.print("/");  Serial.print(MyDateAndTime.Day); Serial.print("/");
  Serial.print("20");  Serial.println(MyDateAndTime.Year);
}

void SDWrite(double nitrates){
  // make a string for assembling the data to log:
  String dataString;
  dataString += "nitrates: ";
  dataString += String(nitrates);
  dataString += " | time: ";
  dataString += String(MyDateAndTime.Hour); dataString += ":"; dataString += String(MyDateAndTime.Minute);
  dataString += " | date: ";
  dataString += String(MyDateAndTime.Month); dataString += "/"; dataString += String(MyDateAndTime.Day); dataString += "/"; dataString += String(MyDateAndTime.Year);
  Serial.println(" ");
  Serial.print(dataString);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("NatureCenterNitrates.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println("wrote to SD Card");
  }

  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file!");
  }
}
