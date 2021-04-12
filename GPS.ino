/*********************************************************************************************************************************/
// Get data from the GPS, Get other variables, generate the two messages, print some debug info
void CheckGPS()
{    
  processGPSData();
  process_wspr_message();
  printGPSData();
}

/*********************************************************************************************************************************/
// This custom version of delay() ensures that the gps object is being "fed".

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (SerialGPS.available())
      gps.encode(SerialGPS.read());
  } while (millis() - start < ms);
}

/*********************************************************************************************************************************/
// Blink a LED for visual status information
// Blink 'nr' times for 'ms' milliseconds 
void BlinkLED(unsigned int nr, unsigned int ms)
{
  if (LED_PIN > 0)
  {
    int i = 0;

    for (i=0; i<nr; i++)
    {
      digitalWrite(LED_PIN,HIGH);
      delay(ms);
      digitalWrite(LED_PIN,LOW); 
      delay(ms);
    }
  }
}


/*********************************************************************************************************************************/
// Process all data from the GPS and check the GPS time to see if we need to start send WSPR
static void processGPSData()
{
  // Get the statyus of the satellites  
  if (gps.satellites.isValid())
  {
    UGPS.Satellites = gps.satellites.value();
  }
  else
  {
    UGPS.Satellites = 0;
  }

 // Time
 if (gps.time.isValid())
 {
    UGPS.Hours = gps.time.hour();
    UGPS.Minutes = gps.time.minute();
    UGPS.Seconds = gps.time.second();
 }
 else
 {
    UGPS.Hours = 0;
    UGPS.Minutes = 0;
    UGPS.Seconds = 0;
 }

 // Position
 if (gps.location.isValid())
 {
    // Set the GPS location
    UGPS.Longitude = gps.location.lng();
    UGPS.Latitude = gps.location.lat();
    UGPS.GPS_valid = 1;
    // Translate the GPS position to a Maidenhead location
    Get_MaidenHead();

    // Blink the LED twice for 50ms to show that the GPS location is valid
    BlinkLED(2,50);
        // Check if it is time to transmit
    if ((UGPS.Seconds <= 2) && (UGPS.Minutes % 2 == 0)) 
    {
      //We need to get the correct frequency en transmit power
      switch (UGPS.Minutes)
      {
        case 0:
        case 12:
        case 24: 
        case 36:
        case 48: freq = freqArray[0]; break;
        case 2: 
        case 14:
        case 26:
        case 38:
        case 50: freq = freqArray[1]; break;
        case 4:
        case 16:
        case 28:
        case 40:
        case 52: freq = freqArray[2]; break;
        case 6:
        case 18:
        case 30:
        case 42:
        case 54: freq = freqArray[3]; break;
        case 8:
        case 20:
        case 32:
        case 44:
        case 56: freq = freqArray[4]; break;
        default:  freq = freqArray[5];
      }
      UGPS.sendMsg1 = true;
    } 
 }
 else
 {
   // Invalid GPS location
   UGPS.Longitude = 0;
   UGPS.Latitude = 0;
   UGPS.GPS_valid = 0;
 }

}

/*********************************************************************************************************************************/
void printGPSData()
{
#if defined(DEVMODE)   
  Serial.print(F("       Valid Time: ")); Serial.println(gps.time.isValid()); 
  Serial.print(F("   Valid Location: ")); Serial.println(gps.location.isValid()); 
  Serial.print(F("             Time: ")); Serial.print(UGPS.Hours); Serial.print(F(":")); Serial.print(UGPS.Minutes); Serial.print(F(":")); Serial.println(UGPS.Seconds);
  Serial.print(F("         Latitude: ")); Serial.println(UGPS.Latitude, 6);
  Serial.print(F("        Longitude: ")); Serial.println(UGPS.Longitude, 6);
  Serial.print(F("       Satellites: ")); Serial.println(UGPS.Satellites);
  Serial.print(F("       Maidenhead: ")); Serial.println(MaidenHead);
  Serial.println();
  Serial.println(F("-------------------------"));  
#endif
}


/*********************************************************************************************************************************/
// Calculate the Maidenhead locator out of GPS coordinates
void Get_MaidenHead()
{
  int o1, o2, o3;
  int a1, a2, a3;
  double remainder;
  
  // longitude
  remainder = UGPS.Longitude + 180.0;
  o1 = (int)(remainder / 20.0);
  remainder = remainder - (double)o1 * 20.0;
  o2 = (int)(remainder / 2.0);
  remainder = remainder - 2.0 * (double)o2;
  o3 = (int)(12.0 * remainder);

  // latitude
  remainder = UGPS.Latitude + 90.0;
  a1 = (int)(remainder / 10.0);
  remainder = remainder - (double)a1 * 10.0;
  a2 = (int)(remainder);
  remainder = remainder - (double)a2;
  a3 = (int)(24.0 * remainder);
  MaidenHead[0] = (char)o1 + 'A';
  MaidenHead[1] = (char)a1 + 'A';
  MaidenHead[2] = (char)o2 + '0';
  MaidenHead[3] = (char)a2 + '0';
  MaidenHead[4] = (char)o3 + 'A';
  MaidenHead[5] = (char)a3 + 'A';
  MaidenHead[6] = (char)0;
}
