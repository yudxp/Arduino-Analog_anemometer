// Bring in analog values 0-5vdc from wind sensors and output NMEA 0183
// Weather Instrument : Wind Speed and Angle  sentence
// Get from arduino forum ( when someone ask to repair his code )
// https://forum.arduino.cc/t/analog-sensors-to-serial-data/602641/11
//


//const byte WindAngleAIPin = A0;
const byte WindSpeedAIPin = A7;


// $ = Message Prefix
// WI = Talker Type: Weather Instrument
// MWV = Wind Speed and Angle
// %d.0 = Wind Angle in Degrees: 0.0 to 360.0
// R = Relative angle (T = True Heading)
// %d.0 = Wind Speed
// N = Knots (Statute Milews per Hour) (K = KPH, M=MPH)
// A = Valid Data
// * = Checksum Indicator
const char WIMWVFormat[] = "$WIMWV,%d.0,R,%d.0,M,A*";


// the setup routine runs once when you press reset:
void setup()
{
  // NMEA0183 standard uses 4800 baud RS-422
  Serial.begin(4800);
}


// the loop routine runs over and over again forever:
void loop()
{
  char buffer[sizeof WIMWVFormat + 10];
  //int windAngle = map(analogRead(WindAngleAIPin), 0, 1023, 350, 10);      // remaps angle value to 350 - 10 Degrees
  int windAngle = 350;
  int windSpeed = map(analogRead(WindSpeedAIPin), 0, 1023, 0, 500)*6/100;        // Value times 6 from the spect, 


  // build the NMEA 0183 string for Weather Instrument : Wind Speed and Angle
  sprintf(buffer, WIMWVFormat, windAngle, windSpeed); // Builds string to send to serial port


  // The XOR checksum covers everything after the "$" to before the "*" //The best part, or your code not working
  byte checksum = 0;
  for (byte i = 1; i < (strlen(buffer) - 1); i++)           // creates Checksum for string
    checksum ^= buffer[i];


  Serial.print(buffer);                                 // print to serial string
  // Add a leading 0 if the checksum is less than two hex digits.
  if (checksum < 0x10)
    Serial.print('0');
  Serial.println(checksum, HEX);                           // print checksum and line end
}
