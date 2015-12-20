#include <SPI.h>
#include <RH_RF95.h>

#ifdef __AVR_ATmega1284P__
  #define LED           15 // Moteino MEGAs have LEDs on D15
  #define FLASH_SS      23 // and FLASH SS on D23
#else
  #define LED           9 // Moteinos have LEDs on D9
  #define FLASH_SS      8 // and FLASH SS on D8
#endif

// Singleton instance of the radio driver
RH_RF95 rf95;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!rf95.init())
    Serial.println("init failed");
  else Serial.println("init OK - 915mhz");
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  rf95.setFrequency(915);
  rf95.setTxPower(20);
}

void loop() {
  //Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  if(rf95.available()){
    digitalWrite(LED, HIGH);
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
//      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("#r;");
      Serial.print(rf95.lastRssi(), DEC);
      Serial.print(";");
      buf[len] = '\0'; //hacky way to end a line and print it
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }
    digitalWrite(LED, LOW);
  }
  int len = 0;
  if(len = Serial.available() > 0){
    digitalWrite(LED, HIGH);
    uint8_t sendBuf[RH_RF95_MAX_MESSAGE_LEN];
    len = Serial.readBytesUntil('\n', sendBuf, RH_RF95_MAX_MESSAGE_LEN);
    rf95.send(sendBuf, len);
    rf95.waitPacketSent();
    Serial.print("#s;");
    Serial.println(len, DEC);
    
    digitalWrite(LED, LOW);
  }
  
}


