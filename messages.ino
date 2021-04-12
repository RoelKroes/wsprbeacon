//******************************************************
// First mesage is a standard WSPR message
//******************************************************
void process_wspr_message()
{
  // Second get the first 4 maidenhead characters
  for (int j=0; j<4; j++)
  {
    UGPS.MH_1[j] = MaidenHead[j];
  }
  
  // Third get the dbm value which is 10mW for the si5351
  UGPS.dbm_1 = DBMPOWER; 

#if defined(DEVMODE)
  Serial.print(F("WSPR message: "));
  Serial.print(MYCALL); Serial.print(F(" "));
  Serial.print(UGPS.MH_1); Serial.print(F(" "));
  Serial.println(UGPS.dbm_1);
#endif  

}
