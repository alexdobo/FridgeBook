////
//
// General code from http://www.pushingbox.com for Arduino + Ethernet Shield (official) v1.2
//
////

#include <SPI.h>
#include <Ethernet.h>

  /////////////////
 // MODIFY HERE //
/////////////////
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };   // Be sure this address is unique in your network

//Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
char DEVID1[] = "v5F6CC1A03EB0EB4";        //Scenario : "The mailbox is open"
char DEVID2[] = "vEFD45732E968A15"; //For eggs

//Numeric Pin where you connect your switch
uint8_t pinDevid1 = 3; // Example : the mailbox switch is connect to the Pin 3

// Debug mode
boolean DEBUG = true;
  //////////////
 //   End    //
//////////////


char serverName[] = "api.pushingbox.com";
boolean pinDevid1State = false;                // Save the last state of the Pin for DEVID1
boolean lastConnected = false;                 // State of the connection last time through the main loop


// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  Serial.begin(9600);
  pinMode(pinDevid1, INPUT);
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else{
    Serial.println("Ethernet ready");
    // print the Ethernet board/shield's IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}



void loop()
{
      ////
      // Listening for the pinDevid1 state
      ////
      if (digitalRead(pinDevid1) == HIGH && pinDevid1State == false) // switch on pinDevid1 is ON 
      {
        if(DEBUG){Serial.println("pinDevid1 is HIGH");}
        pinDevid1State = true;
        //Sending request to PushingBox when the pin is HIGHT
        sendToPushingBox(DEVID1,item);
      }
       if (digitalRead(pinDevid1) == LOW && pinDevid1State == true) // switch on pinDevid1 is OFF
      {
        if(DEBUG){Serial.println("pinDevid1 is LOW");}
        pinDevid1State = false;
        //Sending request to PushingBox when the pin is LOW
        //sendToPushingBox(DEVID1);    //Here you can run an other scenario by creating a DEVID2 variable
      }
      
      
      //DEBUG part
      // this write the respons from PushingBox Server.
      // You should see a "200 OK"
      if (client.available()) {
        char c = client.read();
        if(DEBUG){Serial.print(c);}
      }
      
      // if there's no net connection, but there was one last time
      // through the loop, then stop the client:
      if (!client.connected() && lastConnected) {
        if(DEBUG){Serial.println();}
        if(DEBUG){Serial.println("disconnecting.");}
        client.stop();
      }
      lastConnected = client.connected();
}


//Function for sending the request to PushingBox
void sendToPushingBox(char devid[], char item[]){
  client.stop();
  if(DEBUG){Serial.println("connecting...");}

  if (client.connect(serverName, 80)) {
    if(DEBUG){Serial.println("connected");}

//pushingbox?devid=v5F6CC1A03EB0EB4&item=milk"
//Debug using 
//no eggs
//curl "http://api.pushingbox.com/pushingbox?devid=v5F6CC1A03EB0EB4&item=milk"
//eggs 
//curl "http://api.pushingbox.com/pushingbox?devid=vEFD45732E968A15&item=eggs"


    if(DEBUG){Serial.println("sendind request");}
    client.print("GET /pushingbox?devid=");
    if(item="eggs"){
		client.print(DEVID2);//for egg pun
	}
	else
	{
		client.print(devid); // preset to v5F6CC1A03EB0EB4
	}
    client.print("&item=");
	client.print(item); //eg. milk, eggs etc.
	
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
}
