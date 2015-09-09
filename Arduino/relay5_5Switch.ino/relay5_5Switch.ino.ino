// constants won't change. They're used here to 
// set pin numbers:

#include <Bounce2.h>
#include <EEPROM.h>


#if defined (__AVR_ATtiny84__)
#define  startPin   4     // the number of the pushbutton pin
#define  ledred   10 
#define   ledyellow   9 
#define  bounceinterval   12 
#define Eprom_array_size 100

#define _RELAY1 0
#define _RELAY2 1
#define _RELAY3 2
#define _RELAY0 3

#define _SW_RELAY1 5
#define _SW_RELAY2 6
#define _SW_RELAY3 7
#define _SW_RELAY0 8




#else

#define  startPin   12     // the number of the pushbutton pin
#define  ledred   2 
#define   ledyellow   11

#define _RELAY1 4
#define _RELAY2 5
#define _RELAY3 6
#define _RELAY0 7

#define _SW_RELAY1 8
#define _SW_RELAY2 9
#define _SW_RELAY3 10
#define _SW_RELAY0 3

#define  bounceinterval   12 
#define Eprom_array_size 100




#endif 
 
int Relaypins[] = { _RELAY0, _RELAY1, _RELAY2, _RELAY3 };
int RelayInputpins[] = { _SW_RELAY0, _SW_RELAY1, _SW_RELAY2, _SW_RELAY3 };

int RelayInputpinsbuttonState[4] = { 0, 0, 0, 0 };



 
 
int readingdata = 0;
int Storingdata = 0;

unsigned long  lastbuttonwaittime = 0;
int  readingstart = 0;

unsigned  int storetime[Eprom_array_size];
byte  storetimepin[Eprom_array_size];


Bounce Bouncepin1 = Bounce();

// Instantiate another Bounce object
Bounce Bouncepin2 = Bounce();

Bounce Bouncepin3 = Bounce();

// Instantiate another Bounce object
Bounce Bouncepin4 = Bounce();

 
// Bounce BouncestartPin = Bounce();
///
 
//Bounce bounce_Storingdata = Bounce();

void setup() {

	for (int i = 0; i <= 3; i++){
		pinMode(RelayInputpins[i], INPUT_PULLUP);

	}
	
	pinMode(startPin, INPUT_PULLUP);

	Bouncepin1.attach(RelayInputpins[0]);
	Bouncepin1.interval(bounceinterval); // interval in ms
	Bouncepin2.attach(RelayInputpins[1]);
	Bouncepin2.interval(bounceinterval); // interval in ms
	Bouncepin3.attach(RelayInputpins[2]);
	Bouncepin3.interval(bounceinterval); // interval in ms
	Bouncepin4.attach(RelayInputpins[3]);
	Bouncepin4.interval(bounceinterval); // interval in ms


//	BouncestartPin.attach(RelayInputpins[3]);
//	BouncestartPin.interval(bounceinterval); // interval in ms


	for (int i = 0; i <= 3; i++){
		pinMode(Relaypins[i], OUTPUT );
		digitalWrite(Relaypins[i], LOW);
	}
	
	  pinMode(ledred, OUTPUT);
  pinMode(ledyellow, OUTPUT);
  
 

 

  

 
 
 
	
	//pinMode(STATUSLED, OUTPUT);
 

	 

  readingstart = 0;
 
      readflashdata();

	  // statup status
	  digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW
	  digitalWrite(ledyellow, LOW);   // turn the LED on (HIGH is the voltage level)


	  digitalWrite(ledred, HIGH);    // turn the LED off by making the voltage LOW
	  digitalWrite(ledyellow, HIGH);   // turn the LED on (HIGH is the voltage level)
	  delay(500);
	  digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW
	  digitalWrite(ledyellow, LOW);   // turn the LED on (HIGH is the voltage level)



}

 
void dosetuploop(){

   // wait for depress
	while (  !digitalRead(startPin)) {
		// 
	}


	delay(250); // bounce ignore 

	int reading[4] = { 0, 0, 0, 0 };
	int savedata = 0;
	int fistsave = 1;
//	flashstatus(3);

	Storingdata = 0;
 
	lastbuttonwaittime = millis();

	while (true)
	{

	 


		readingstart = !digitalRead(startPin);

			if (readingstart)
			{

				readingstart = true;
				digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW
				digitalWrite(ledyellow, HIGH);    // turn the LED off by making the voltage LOW
				storetime[Storingdata + 1] = 0;

				writeflash();
				while (!digitalRead(startPin)) {
					// 
				}


				delay(250); // bounce ignore 
				digitalWrite(ledyellow, LOW);    // turn the LED off by making the voltage LOW
		 
				return;

			}

		 


		 
		 
		Bouncepin1.update(); 
		Bouncepin2.update();
		Bouncepin3.update();
		Bouncepin4.update();

		reading[0] = !Bouncepin1.read();
		reading[1] = !Bouncepin2.read();
		reading[2] = !Bouncepin3.read();
		reading[3] = !Bouncepin4.read();

		/*
		
		for (int i = 0; i <= 3; i++){
			reading[i] = !digitalRead(RelayInputpins[i]);
		 
		}
*/
		// read the state of the switch into a local variable:
		savedata = 0;
		for (int i = 0; i <= 3; i++){
			if (reading[i] != RelayInputpinsbuttonState[i]) {
				RelayInputpinsbuttonState[i] = reading[i];
				savedata = 1;
			}
		}

         
			if (savedata) {
			 


			unsigned  long  buttonwaittime = millis();
			 
		 
			if ((buttonwaittime - lastbuttonwaittime) > 65535) {

				readingstart = true;
				digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW
				digitalWrite(ledyellow, HIGH);    // turn the LED off by making the voltage LOW
			 
				digitalWrite(ledyellow, LOW);    // turn the LED off by making the voltage LOW
				return;
			}
			 

			//if (Storingdata >= Eprom_array_size)
			//	Storingdata = 0;

			Storingdata++;
			storetime[Storingdata] = buttonwaittime - lastbuttonwaittime;
			if (fistsave == 1){
				storetime[Storingdata] = Eprom_array_size;


				fistsave = 0;
			}

			lastbuttonwaittime = buttonwaittime;
			int bitbuttonstate = RelayInputpinsbuttonState[0];
			bitbuttonstate = (RelayInputpinsbuttonState[1] << 1) | bitbuttonstate;
			bitbuttonstate = (RelayInputpinsbuttonState[2] << 2) | bitbuttonstate;
			bitbuttonstate = (RelayInputpinsbuttonState[3] << 3) | bitbuttonstate;

			storetimepin[Storingdata] = bitbuttonstate;

			// feedback
			setrelays(bitbuttonstate);

		}
	}

}
void setrelays( int bitstate){



		digitalWrite(Relaypins[0], (bitstate & 1));
		digitalWrite(Relaypins[1], (bitstate & 2));
		digitalWrite(Relaypins[2], (bitstate & 4));
		digitalWrite(Relaypins[3], (bitstate & 8));

	 
 


}

void loop(){
	// 
//	bounce_Storingdata.update();


//	int readingstart = bounce_Storingdata.read();
 	

	//bounce_Storingdata.update();
 


	readingstart = !digitalRead(startPin);
	 

	if (readingstart)
	{
		digitalWrite(ledred, HIGH);    // turn the LED off by making the voltage LOW
		digitalWrite(ledyellow, LOW);    // turn the LED off by making the voltage LOW 
		dosetuploop();
		delay(250);
	 
		 
	}
	 
	else
	{
		digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW	
		digitalWrite(ledyellow , HIGH);    // turn the LED off by making the voltage LOW 
		playback();
	   	 
		digitalWrite(ledyellow, LOW);
	}
 
 }
void writeflash(){

	for (int i = 1; i < Eprom_array_size; i++)
	{
		int tmp = storetime[i];
		int tmpbtn = storetimepin[i];

		
   
    int	addrtmp = i * 3;

	if (tmp <= 0) {
		EEPROM.update(addrtmp - 2, 0);
		EEPROM.update(addrtmp - 1,0);
		EEPROM.update(addrtmp, 0);
	
		break;
	}
	
	EEPROM.update(addrtmp - 2, highByte(tmp));
	EEPROM.update(addrtmp - 1, lowByte(tmp));
	EEPROM.update(addrtmp , lowByte(tmpbtn));
	}
	
//	flashstatus(10);
	for (int i = 1; i < 5; i++)
	{
	digitalWrite(ledyellow, HIGH);    // turn the LED off by making the voltage LOW 
	digitalWrite(ledred , HIGH);    // turn the LED off by making the voltage LOW 
	delay(300);
	digitalWrite(ledyellow, LOW);    // turn the LED off by making the voltage LOW 
	digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW 
	delay(300);
	}
	digitalWrite(ledyellow, LOW);    // turn the LED off by making the voltage LOW 
	digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW 

}
/*
void flashstatus( int fcount )
{
	for (int i = 1; i < fcount; i++)
	{
		digitalWrite(STATUSLED, HIGH);
		delay(300);
		digitalWrite(STATUSLED, LOW);
		delay(300);
	}
}
*/
void readflashdata(){
 
     

    
	for (int i = 1; i < Eprom_array_size; i++)
	{
   

 
		//digitalWrite(STATUSLED, HIGH);
		int	addrtmp = i * 3;
		int tmph = EEPROM.read(addrtmp -2 );
		int tmps = (tmph << 8);
		int tmpl = EEPROM.read(addrtmp - 1);
		int tmpbtn = EEPROM.read(addrtmp);
		if ((tmps + tmpl) <= 0)  break;

		storetime[i] = (tmps  + tmpl);
		storetimepin[i] = tmpbtn ;
		// EEPROM.read (i, Storingdata);
		//digitalWrite(STATUSLED, LOW);
 
	}

 
      
       

}

int DELAYCHECKbutton(int delaytime) {



	unsigned long currentMillis = millis();
	unsigned long tmppreviousMillis = currentMillis;
	 
		

	while (currentMillis - tmppreviousMillis <= delaytime) {

 
		if (!digitalRead(startPin))
			return 1;
		currentMillis = millis();

	}
	return 0;


	//	delay(delaytime);
}

void playback(){
	
//	digitalWrite(STATUSLED, HIGH);
	 

	for (int i = 1; i < Eprom_array_size; i++)
	{
		int tmp = storetime[i];
		int tmpbtn = storetimepin[i];
		if (tmp <= 0) break;
		setrelays(tmpbtn);
	//	digitalWrite(Relaypins[0], tmpbtn);
	//	digitalWrite(STATUSLED, HIGH);
		if (DELAYCHECKbutton(tmp))
		{

		 	return;
		}
	//	digitalWrite(STATUSLED, LOW);
	}
 
	//digitalWrite(STATUSLED, LOW);
}
 





