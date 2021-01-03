/* Based on 
 *  https://github.com/adafruit/Adafruit_MQTT_Library/blob/master/examples/mqtt_ethernet/mqtt_ethernet.ino
 */

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>


#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>


byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,112);
//Uncomment the following, and set to your preference if you don't have automatic dns.
IPAddress dnsIP (192, 168, 0, 1);
//If you uncommented either of the above lines, make sure to change "Ethernet.begin(mac)" to "Ethernet.begin(mac, iotIP)" or "Ethernet.begin(mac, ip, dnsIP)"


/************************* Adafruit.io Setup *********************************/

/* #define AIO_SERVER      "io.adafruit.com" */
#define AIO_SERVER      "test.mosquitto.org"
/* Keep in mind that this is the unencrypted port */
#define AIO_SERVERPORT  1883
/*
#define AIO_USERNAME    "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY         "...your AIO key..."
*/
#define AIO_USERNAME    ""
#define AIO_KEY         ""


EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "TestingTopicEFERGCZ123"); //"/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "TestingTopicEFERGCZ"); //"/feeds/onoff");

void setup() {
  Serial.begin(9600);

  // disable SD card if one in the slot
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  Serial.println("Starting ethernet");
  
  /* Ethernet.begin(mac, iotIP, dnsIP) */
  Ethernet.begin(mac,ip,dnsIP);

  Serial.println(Ethernet.localIP());

  mqtt.subscribe(&onoffbutton);
}

// This is just some counter, should be renamed later
uint32_t x=0;


void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
    
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}
