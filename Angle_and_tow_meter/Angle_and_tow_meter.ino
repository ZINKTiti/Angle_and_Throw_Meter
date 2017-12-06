#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

float xg,yg,zg;
float soh;
float tilt;
float angle;
String dir;

const int PB_PLUS = 2;
const int PB_MINUS = 3;
const int PB_INIT = 4;

int CHORD = 50;
float DISPLAYANGLE, DISTANCE, RADIAN, STARTANGLE;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  pinMode(PB_PLUS,INPUT);
  pinMode(PB_MINUS, INPUT);
  pinMode(PB_INIT, INPUT);
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_2_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);
  //accel.setDataRate(ADXL345_DATARATE_3200_HZ);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C
  display.clearDisplay();   // clears the screen and buffer   // Efface l'écran
}

void loop(void) 
{
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);
  xg = event.acceleration.x*0.0039;
  yg = event.acceleration.y*0.0039;
  zg = event.acceleration.z*0.0039;
  // Output x,y,z values - Commented out
  //Serial.print("X Value: "); Serial.print(xg);
  //Serial.print(", Y Value: "); Serial.print(yg);
  //Serial.print(", Z Value: "); Serial.println(zg);
  delay(500);

  
  //soh = yg/zg;
  angle = abs(atan(yg/zg)*57.296);
  DISPLAYANGLE = abs(angle-STARTANGLE);
  RADIAN = (abs(angle-STARTANGLE)* 71) / 4068;
  DISTANCE = sin(RADIAN) * CHORD;
  
  if (digitalRead(PB_INIT) == LOW)
  {
    STARTANGLE = angle;
  }
  
  if (digitalRead(PB_PLUS) == LOW)
  {
    CHORD++;
  }
  if (digitalRead(PB_MINUS) == LOW) 
  {
    CHORD--; 
  }
  
  Serial.print("Tilt Angle is: "); Serial.print(angle); Serial.print(";");
  Serial.print(" Display Angle is: "); Serial.print(DISPLAYANGLE); Serial.print(";");
  Serial.print(" Start angle :"); Serial.print(STARTANGLE);Serial.print(";");
  Serial.print(" Chord :"); Serial.print(CHORD);Serial.print(";");
  Serial.print(" Radian :"); Serial.print(RADIAN); Serial.print(";");
  Serial.print(" Distance :");Serial.print(DISTANCE); Serial.println(";");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("CHORD "); display.println(CHORD);
  display.print("ANGLE "); display.print(DISPLAYANGLE); display.println(" deg");
  display.print("DIST "); display.print(DISTANCE); display.println(" mm"); 
  display.display();
  delay(5);
  display.clearDisplay();
}



void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}
