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

const int PB_PLUS = 2;
const int PB_MINUS = 3;
const int PB_INIT = 4;

// Variables 
int corde_gouverne = 50;     // Corde de la gouverne

// Variables d'initialisation

boolean INIT = false;
boolean INVERT = false;

//  Vecteurs acceleration (3 axe)
//              x,y,z
double v_neutre [3] = {0,0,0};
double v_courant [3] = {0,0,0};
// Variables de calcul
double produit_v;
double norme_v_neutre;
double norme_v_courant;
float angle;
float debattement;
// Variable de gestion de l'affichage
char ligne_texte [17];
int angle_aff;
int debattement_aff;
//debug
boolean debug = false;

void setup(void) 
{
  Serial.begin(9600);

//  pinMode(PB_PLUS,INPUT);
//  pinMode(PB_MINUS, INPUT);
//  pinMode(PB_INIT, INPUT);
  
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
  
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C
  display.clearDisplay();   // clears the screen and buffer   // Efface l'écran
}

void loop(void) 
{
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
   
  // Lecture du capteur LIS3DH
  v_courant [0] = event.acceleration.x;
  v_courant [1] = event.acceleration.y;
  v_courant [2] = event.acceleration.z;
  
  // plan de calcul : y/z :
  // axes : x:0  y:1  z:2
  // Calcul du produit : v_neutre . v_courant 
    produit_v = v_neutre [1] * v_courant [1] + v_neutre [2] * v_courant [2];
  //
  // Calcul norme : v_neutre 
    norme_v_neutre = sqrt (sq (v_neutre [1]) + sq (v_neutre [2]));
  //
  // Calcul norme : v_courant 
    norme_v_courant = sqrt (sq (v_courant [1]) + sq (v_courant [2]));
  //
  // Calcul de l'angle en radians
    angle = acos (produit_v  / (norme_v_neutre * norme_v_courant));
  //  debattement gouverne
  //  Equation = 2 x sinus (Angle/2) * corde gouverne
    debattement = abs( (corde_gouverne * 2.0) * sin (angle/2.0));
  // Conversion de l'angle en degres
    angle = abs(angle * 180 / 3.14159265359);


  
  if (digitalRead(PB_INIT) == LOW)
  {
     v_neutre [1] = event.acceleration.y;
     v_neutre [2] = event.acceleration.z;
     INIT = true;
     display.clearDisplay();
     delay(100);
  }

  if (digitalRead(PB_PLUS) == LOW & digitalRead(PB_MINUS) == LOW )
  {
    if (INVERT == true);INVERT = false;  
    delay(100);  
  }

  if (digitalRead(PB_MINUS) == LOW & digitalRead(PB_INIT) == LOW )
  {
    if (INVERT == false);INVERT = true;  
    delay(100); 
  }
  
  if (digitalRead(PB_PLUS) == LOW)
  {
    corde_gouverne++;
    delay(100);
  }
  
  if (digitalRead(PB_MINUS) == LOW) 
  {
    corde_gouverne--;
    delay(100); 
  }

  angle_aff = (int) (angle*10);
  debattement_aff = (int) (debattement*10);
 
  
  if (INIT  == false)
  {
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Click on");
    display.println("init !");
  }
  else
  {
    if (INVERT == false)
    {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("Chord: "); display.println(corde_gouverne);
      display.print("Angle: "); display.println(angle_aff/10);/*display.print(",");display.print(angle_aff%10); display.println(" deg")*/;
      display.print("Distance: "); 
      display.setTextSize(4);
      display.setTextColor(WHITE);
      display.setCursor(80,0);
      display.print(debattement_aff/10);/*display.print(",");display.print(debattement_aff%10);display.println(" mm")*/;
    }
    if (INVERT == true)
    {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("Chord: "); display.println(corde_gouverne);
      display.print("Dis: "); display.println(debattement_aff/10);/*display.print(",");display.print(debattement_aff%10);display.println(" mm")*/;
      display.print("Angle: "); 
      display.setTextSize(4);
      display.setTextColor(WHITE);
      display.setCursor(80,0);
      display.print(angle_aff/10);/*display.print(",");display.print(angle_aff%10); display.println(" deg")*/;
    }
  }
  display.display();
  delay(75);
  display.clearDisplay();

  if (debug == true)
  {
    Serial.print(" v_courant_x: "); Serial.print(v_courant [0]); Serial.print(";");
    Serial.print(" v_courant_y: "); Serial.print(v_courant [1]); Serial.print(";");
    Serial.print(" v_courant_z: "); Serial.print(v_courant [2]); Serial.print(";");
    Serial.print(" v_neutre_x: "); Serial.print(v_neutre [0]); Serial.print(";");
    Serial.print(" v_neutre_y: "); Serial.print(v_neutre [1]); Serial.print(";");
    Serial.print(" v_neutre_z: "); Serial.print(v_neutre [2]); Serial.print(";");
    Serial.print(" produit_v: "); Serial.print(produit_v); Serial.print(";");
    Serial.print(" norme_v_neutre: "); Serial.print(norme_v_neutre); Serial.print(";");
    Serial.print(" norme_v_courant: "); Serial.print(norme_v_courant); Serial.print(";");
    Serial.print(" Angle: "); Serial.print(angle); Serial.print(";");
    Serial.print(" Angle_aff: ");Serial.print(angle_aff/10);Serial.print(",");Serial.print(angle_aff%10); Serial.print(" deg"); Serial.print(";");
    Serial.print(" Chord :"); Serial.print(corde_gouverne);Serial.print(";");
    Serial.print(" Debatement :");Serial.print(debattement_aff/10);Serial.print(",");Serial.print(debattement_aff%10); Serial.print(";");
    Serial.println(INVERT);
  }
  
}
