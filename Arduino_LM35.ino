/* 
 *  Name : S U R Y A P R A K A S H M
 *  Date : 1/Mar/2024
 *  Description : Develope a code for Arduino to interface an LM35 temperature sensor to control the led based on the temperature reading
                  When the temperature falls below 30 degrees Celsius, make the onboard LED blink every 250 milliseconds.
                  If the temperature rises above 30 degrees Celsius, make the onboard LED blink every 500 milliseconds.  
                  (without using Millis(), delay() and micros() function to will blink the led)
              
 *  solution    : I have used <TaskScheduler.h> Library to perform this Task  
 */ 

#include <TaskScheduler.h>     

#define lm35pin A0

Scheduler scheduler;

// Function prototypes
void tempCallback();
void ledCallback();

// Task to check the temperature every 1000 milliseconds
Task checkTemp(1000, TASK_FOREVER, &tempCallback);
// Task to toggle the LED every 250 milliseconds
Task toggleLed(250, TASK_FOREVER, &ledCallback);

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);      // Onboard led pin as an Output(connected to 13th pin)

  scheduler.addTask(checkTemp);      // Add Task to the sheduler
  scheduler.addTask(toggleLed);

  checkTemp.enable();                // Enable the temperature checking task
  toggleLed.disable();               // Disable the led toggling task initially
}

void loop()
{
  scheduler.execute();               //Execute the task in the sheduler    
}

// Callback function for the temperature task

void tempCallback() 
{
  //Read temperature from LM35 sensor
  int sensorVal = analogRead(lm35pin);
  float voltage = sensorVal * (5000.0 / 1024.0);
  float tempC = voltage / 10.0;
  
  // Print temperature to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" degrees Celsius");

  // Temperature falls below 30deg Celsius LED will blink every 250 millisecond
  if (tempC < 30.0) 
  {
    toggleLed.setInterval(250);
    toggleLed.enable();
  } 
  // Temperature rise above  30deg Celsius LED will blink every 500 millisecond
  else if (tempC > 30.0)
  {
    toggleLed.setInterval(500);
    toggleLed.enable();
  }
  else {
    toggleLed.disable();
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// Callback function for the LED task
void ledCallback() 
{
  // Toggle the LED state
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
