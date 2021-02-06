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


/************************* MQTT Connection Setup *********************************/

#define MQTT_SERVER      "192.168.1.172" /* "test.mosquitto.org" */
/* Keep in mind that this is the unencrypted port */
#define MQTT_SERVERPORT  1883

#define MQTT_CLIENT_ID   "LivingRoomNano"
#define MQTT_USERNAME    "thermometer001"
#define MQTT_KEY         "thermometer001pw"


EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }

/****************************** Feeds ***************************************/

// Setup a feed called 'temperature' for publishing.
// Notice MQTT paths for MQTT follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, "home/livingroom/temperature");

Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, "home/livingroom/humidity");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, "home/livingroom/onoffbutton");

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
  Serial.print(F("\nSending temperature val "));
  Serial.print(x);
  Serial.print("...");
  if (! temperature.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("\nSending humidity val "));
  Serial.print(x);
  Serial.print("...");
  if (! humidity.publish(x++)) {
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
