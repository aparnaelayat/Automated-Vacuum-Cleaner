/* 
 * Credits to all sources on Google and Youtube, acting as reference to this code below. This code is not 
 * TechValer's own creation. TechValer has referred to different projects and websites to 
 * find an easy code for beginners to get started with Relay and Arduino.
 * TechValer does not claim this code to be its own. TechValer is greatly thankful for original 
 * creaters of this code and also all others who acted as reference. 
 */

/* 
 *  About TechValer
 *  
 *  What comes to mind when you think of tech...hmm, we're sure you're thinking of iPhone, 
 *  Alexa, Boston Dynamics robotic dog etc., at least that's what we would have thought of. 
 *  Our point here is, when you look inside this tech...you'll find weird boards with 
 *  components attached to it. This stuff is electronics and we at Techvaler deeply appreciate 
 *  this piece of tech. As the name suggests, we are Tech Enthusiasts who know the Worth and 
 *  Potential of these amazing tech stuff! So, check out our website techvaler.com and 
 *  Youtube Channel: Techcafe to find out more.
 */

/*
 * Thanks to Drona Automations Pvt.Ltd for Sponsoring this project!
 */
 
int Relaypin= 3; // Define input pin for relay

void setup() {
  // put your setup code here, to run once:
pinMode(Relaypin, OUTPUT); // Define the Relaypin as output pin
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(Relaypin, HIGH); // Sends high signal 
delay(1000); // Waits for 1 second
digitalWrite(Relaypin, LOW); // Makes the signal low
delay(1000); // Waits for 1 second
}
