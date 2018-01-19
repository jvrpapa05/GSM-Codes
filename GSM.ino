#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
String msg = String("");
int SmsContentFlag = 0;
int ledPin = 13;

String url1 = "http://maps.google.com/maps?f=q&q=(";
String longitude = "14.5961257";
String url2 = ",";
String latitude = "120.9834896";
String url3 = ")";

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode( ledPin, OUTPUT );
  digitalWrite( ledPin, LOW );
  GprsTextModeSMS();
}

void loop()
{
  char SerialInByte;
  if (Serial.available())
  {
    mySerial.print((unsigned char)Serial.read());
  }
  else if (mySerial.available())
  {
    char SerialInByte;
    SerialInByte = (unsigned char)mySerial.read();
    Serial.print( SerialInByte );
    if ( SerialInByte == 13 )
    {
      ProcessGprsMsg();
    }
    if ( SerialInByte == 10 ) {
    }
    else {
      msg += String(SerialInByte);
    }
  }
}

void GprsTextModeSMS() {
  mySerial.println( "AT+CMGF=1" );
}

void GprsReadSmsStore( String SmsStorePos ) {
  mySerial.print( "AT+CMGR=" );
  mySerial.println( SmsStorePos );
}


void ClearGprsMsg() {
  msg = "";
}

void ProcessGprsMsg()
{
  Serial.println("");
  Serial.print( "GPRS Message: [" );
  Serial.print( msg );
  Serial.println( "]" );

  String sms;
  sms = msg;

  if ( sms.indexOf("SampleLoc") >= 0 ) {
    digitalWrite( ledPin, HIGH );
    Serial.println( "Sample Location ONLY" );

    mySerial.println("AT+CMGF=1");
    delay(1000);
    mySerial.println("AT+CMGS=\"09360273942\"\r");
    delay(1000);
    mySerial.println(url1 + longitude + url2 + latitude + url3);
    delay(100);
    mySerial.println((char)26);
    delay(1000);

  }

  if ( sms.indexOf("On") >= 0 ) {
    digitalWrite( ledPin, HIGH );
    Serial.println( "LED IS ON" );

    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"09360273942\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("LED IS ON");// The SMS text you want to send
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);

  }

  if ( sms.indexOf("Off") >= 0 ) {
    digitalWrite( ledPin, LOW );
    Serial.println( "LED IS OFF" );

    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"09360273942\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("LED IS OFF");// The SMS text you want to send
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);

  }
  ClearGprsMsg();
  SmsContentFlag = 0;
}
