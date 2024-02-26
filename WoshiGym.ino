//go tools switch processor to old bootloader

#include <Servo.h>
#include <Keypad.h> //keypad library
#include <Wire.h>
#include <Adafruit_GFX.h> //Adafruit GFX
#include <Adafruit_SSD1306.h> //Adafruit ssd1306
#include <Arduino.h>

// Define the keypad
const byte rows[4] = {2, 3, 4, 5};//connect to the row pinouts of the keypad
const byte cols[4] = {6, 7, 8, 9};//connect to the column pinouts of the keypad 
char keys[4][4] = { //create 2D arry for keys
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'},

};

Keypad mykeypad = Keypad(makeKeymap(keys), rows, cols, 4, 4);

//define OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//define Servo
Servo myServo;  // Create a Servo object

void setup() {
  Serial.begin(115200);
  Wire.begin();
  myServo.attach(10); //Servo on pin 10

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();
}
/*display.clearDisplay(); //all pixels are off
display.drawPixel(x,y, color) – plot a pixel in the x,y coordinates
display.setTextSize(n) – set the font size, supports sizes from 1 to 8
display.setCursor(x,y) – set the coordinates to start writing text
display.print(“message”) – print the characters at location x,y
display.display() – call this method for the changes to make effect*/

void setting(int textsize){
  display.clearDisplay();
  display.setTextSize(textsize);
  display.setTextColor(WHITE);
}

int prepassword[6] = {};
char correctPassword[7] = " ";
static char enteredPassword[7] = " ";
static byte position = 0;
int closeSafe = 1;

void loop() {
  char myKey = mykeypad.getKey();//get key and put in to the veriable
  static unsigned long lastKeyPressTime = 0;

  if (myKey) {
    if (myKey == '#') {
      checknumber();
    }else{
    checkPassword(myKey);
    lastKeyPressTime = millis();
  }}else{
    if (millis() - lastKeyPressTime > 10000) { // 10 seconds of inactivity
      display.clearDisplay();
      position = 0;
      strcpy(enteredPassword, "      ");
      display.display();
    }
  }
}

void checkPassword(char myKey){
    setting(1);
    display.setCursor(0,0);
    display.print("Enter Passcode:");
    display.display();
    enteredPassword[position++] = myKey;
    display.setTextSize(3);
    display.setCursor(10, 30);
    display.print(enteredPassword);

   if (position == 6) {
    position = 0; // Reset position for the next input
    if (strcmp(enteredPassword, correctPassword) == 0) {
      setting(2);
      display.setCursor(0, 10);
      Safe();
    } else {
      setting(2);
      display.setCursor(0, 10);
      display.print("Please try again");
    }
    // Reset enteredPassword for the next input
    strcpy(enteredPassword, "      ");

   }

    display.display();
}

void Safe(){
  if(closeSafe){
    // Rotate the servo 180 degrees clockwise
    myServo.write(0);  // 0 degrees is fully clockwise
    display.setCursor(0, 10);
    display.print("safebox is opened");
    delay(1000);  // Wait for a second
    closeSafe = 0;
  }else{
    // Rotate the servo 180 degrees counterclockwise
    myServo.write(180);  // 180 degrees is fully counterclockwise
    display.setCursor(0, 10);
    display.print("safebox is closed");
    delay(1000);  // Wait for a second
    closeSafe = 1;
    strcpy(correctPassword, "      ");
  }
}

void checknumber(){
  setting(3);
  for (int i = 0; i < 6; i++) {
    if(closeSafe){
      prepassword[i] = random(9);
      }
    display.setCursor(18 * (i + 1), 25);
    display.print(prepassword[i]);
      }
  display.display();
  delay(6000);
  position = 0;
  strcpy(enteredPassword, "      ");
  if(closeSafe){
  generatePassword();}
  delay(6000);
}

void generatePassword(){
  prepassword[0] = (prepassword[0] + 4 * 12 - 15)%10;
  prepassword[1] = (prepassword[1] + 1 * 11 - 3)%10;
  prepassword[2] = (prepassword[2] + 13 * 10 - 5)%10;
  prepassword[3] = (prepassword[3] + 30 * 7 - 18)%10;
  prepassword[4] = (prepassword[4] + 13 * 8 - 8)%10;
  prepassword[5] = (prepassword[5] + 2 * 3 - 1)%10;
  setting(3);
  display.setCursor(0, 10);
  /*for (int i = 0; i < 6; i++) {
  display.setCursor(18 * (i + 1), 25);
  display.print(prepassword[i]);
    }
  display.display();*/
  strcpy(correctPassword, "");
  for (int i = 0; i < 6; i++) {
    char temp[7];
    itoa(prepassword[i], temp, 10);  // Convert int to char array
    strcat(correctPassword, temp);
  }
  /*setting(3);
  display.setCursor(0,20);
  display.print(correctPassword);
  display.display();*/
}
