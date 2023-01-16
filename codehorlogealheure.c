 // TECHATRONIC.COM  
 // RTC CIRCUIT DS3231 LIBRARY  
 //http://www.rinkydinkelectronics.com/library.php?id=73  
 #include <DS3231.h> // RTC LIBRARY  
 #include <Wire.h>  // WIRE LIBRARY         
 // Init the DS3231 using the hardware interface  
 DS3231 rtc(SDA, SCL);  
 void setup()  
 {  
  // Setup Serial connection  
  Serial.begin(9600);  
  // Initialize the rtc object  
  rtc.begin();  
   // The following lines can be uncommented to set the date and time  
   // rtc.setDOW(FRIDAY);   // Set Day-of-Week to FRIDAY  
   // rtc.setTime(12, 37, 0);   // Set the time to 12:00:00 (24hr format)  
   // rtc.setDate(1, 1, 2021);  // Set the date to January 1st, 2021.  
 }  
 void loop()  
 {  
  // Send Day-of-Week  
  Serial.print(rtc.getDOWStr());  
  Serial.print(" ");  
  // Send date  
  Serial.print(rtc.getDateStr());  
  Serial.print(" -- ");  
  // Send time  
  Serial.println(rtc.getTimeStr());  
  // Wait one second before repeating :)  
  delay (1000);  
 }  