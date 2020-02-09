const byte numChars = 7; //avec le \n à la fin...

const byte MaxByteArraySize = 3;
byte byteArray[MaxByteArraySize] = {};
char receivedChars[numChars];   // an array to store the received data
const byte MaxByteArraySizeResponse = 16;
byte responseArray[MaxByteArraySizeResponse]={};

boolean newCommand = false;
boolean newResponse = false;

void hexCharacterStringToBytes(byte *byteArray, char *hexString)
{
  bool oddLength = strlen(hexString) & 1;

  byte currentByte = 0;
  byte byteIndex = 0;

  for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
  {
    bool oddCharIndex = charIndex & 1;

    if (oddLength)
    {
      // If the length is odd
      if (oddCharIndex)
      {
        // odd characters go in high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Even characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
    else
    {
      // If the length is even
      if (!oddCharIndex)
      {
        // Odd characters go into the high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Odd characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
  }
}

void dumpByteArray(const byte * byteArray, const byte arraySize)
{
for (int i = 0; i < arraySize; i++)
{
  Serial.print("0x");
  if (byteArray[i] < 0x10)
    Serial.print("0");
  Serial.print(byteArray[i], HEX);
  Serial.print(", ");
}
Serial.println();
}

byte nibble(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;  // Not a valid hexadecimal character
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
   
    while (Serial.available() > 0 && newCommand == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newCommand = true;
            
        }
    }
}

void convertNewData() {
        Serial.print("Command = ");
        Serial.println(receivedChars);
        hexCharacterStringToBytes(byteArray,receivedChars);
        //Serial.println(sizeof(byteArray));
        dumpByteArray(byteArray,MaxByteArraySize);
}

void writeCommand() {
 Serial2.write(byteArray,sizeof(byteArray));
 newCommand = false; 
 newResponse = true; 
}

void readResponse() {
Serial2.setTimeout(500);
size_t receivedBytes = Serial2.readBytes(responseArray,MaxByteArraySizeResponse);
   dumpByteArray(responseArray, MaxByteArraySizeResponse); 
 newResponse = false; 
}


void printResults(){
  
 if (responseArray[0] == 0x81){
    if (responseArray[1] == 0x11){
    Serial.println("Concentrations readings averaged over 10 seconds and updated every 1 second:"); 
  resultPrinter(responseArray, MaxByteArraySizeResponse);
  }

   if (byteArray[1] == 0x12){
      Serial.println("Concentrations readings averaged over 900 seconds and updated every 60 seconds:");  
  resultPrinter(responseArray, MaxByteArraySizeResponse); 
  }

   if (byteArray[1] == 0x13){
     Serial.println("Concentrations readings averaged over 900 seconds and updated every 60 seconds:");
  resultPrinter(responseArray, MaxByteArraySizeResponse);   
  }

   if (byteArray[1] == 0x14){
      Serial.println("Temperature and humidity readings:"); 
      resultPrinter2(responseArray, 8); 
  }

    if (byteArray[1] == 0x15){
      Serial.println("Power on or sleep mode:");  
  }

  if (byteArray[1] == 0x16){
      Serial.println("Sensor state’s readings:");
  }

  
  }


//
//  Serial.print("Firmware version readings"); 
//
  
  }

void resultPrinter2(const byte * byteArray, const byte arraySize){

  byte value0 = 0;
  unsigned int value1 = 0;
  unsigned int value2 = 0;

value0 = byteArray[2];
    
for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(value0, b));
  }
Serial.println();

value1 = word(byteArray[3],byteArray[4]);



    Serial.print("Temperature = ");

    float temp = (float(0.9754)*(float)value1-float(4.2488))/100;
    
    Serial.println(temp);

value2 = word(byteArray[5],byteArray[6]);
  
    Serial.print("Humidity = ");
    
    float humi = (float(1.1768)*(float)value2-float(4.727))/100;

    
    Serial.println(humi);

}


void resultPrinter(const byte * byteArray, const byte arraySize){
  
  byte value0 = 0;
  unsigned int value1 = 0;
  unsigned int value2 = 0;
  unsigned int value3 = 0;
  unsigned int value4 = 0;
  unsigned int value5 = 0;
  unsigned int value6 = 0;

    value0 = byteArray[2];
    
for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(value0, b));
  }
Serial.println();
    
    value1 = word(byteArray[3],byteArray[4]);
    Serial.print("PM1 pcs/mL = ");
    Serial.println(value1);
    
    value2 = word(byteArray[5],byteArray[6]);
  
    Serial.print("PM2.5 pcs/ mL = ");
    Serial.println(value2);

    value3 = word(byteArray[7],byteArray[8]);
  
    Serial.print("PM10 pcs/ mL = ");
    Serial.println(value3);

    value4 = word(byteArray[9],byteArray[10]);
  
    Serial.print("PM1 μg/m3 = ");
    Serial.println(value4/10);

    value5 = word(byteArray[11],byteArray[12]);
  
    Serial.print("PM2.5 μg/m3 = ");
    Serial.println(value5/10);

    value6 = word(byteArray[13],byteArray[14]);
  
    Serial.print("PM10 μg/m3 = ");
    Serial.println(value6/10);

  }


void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8E1);

  Serial.println("Concentrations readings averaged over 10 seconds and updated every 1 second = 81116E");
  Serial.println("Concentrations readings averaged over 60 seconds and updated every 10 seconds = 81126D");
  Serial.println("Concentrations readings averaged over 900 seconds and updated every 60 seconds= 81136C");
  Serial.println("Temperature and humidity readings = 81146B");
  Serial.println("Power on or sleep mode = 81156A");
  Serial.println("Sensor state’s readings = 811669");
  Serial.println("Firmware version readings = 811768");
  Serial.println("Type something:"); 
 
}

void loop() {

recvWithEndMarker();
if (newResponse == false){
  if (newCommand == false){
            recvWithEndMarker();
    }else{
          convertNewData();
    writeCommand();
      
      
      }

  }else{
   readResponse();
   printResults();
    }
}
