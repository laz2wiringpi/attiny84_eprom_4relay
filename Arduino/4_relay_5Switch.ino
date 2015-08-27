// constants won't change. They're used here to 
// set pin numbers:

#include <Bounce2.h>
#include <EEPROM.h>



//#ifdef  __AVR_ATTINY84__  
 
//#define STATUSLED   0 
#define  startPin   0     // the number of the pushbutton pin

#define  bounceinterval   12 
#define Eprom_array_size 100


int Relaypins[] = { 2, 3, 4, 5 };
int RelayInputpins[] = { 7, 8, 9, 10 };

// 6 not used 

//#endif

 

/* 
 //#ifdef   __OTHER  
 
//#define STATUSLED   13 
#define  startPin   12     // the number of the pushbutton pin

#define  bounceinterval   12 
#define Eprom_array_size 100


int Relaypins[] = { 4, 5, 6, 7 };
int RelayInputpins[] = { 8, 9, 10, 11 };

//#endif
*/


int RelayInputpinsbuttonState[4] = { 0, 0, 0, 0 };




 
 
int readingdata = 0;
int Storingdata = 0;

int lastbuttonwaittime = 0;
int  readingstart = 0;

int storetime[Eprom_array_size];
byte  storetimepin[Eprom_array_size];


Bounce Bouncepin1 = Bounce();

// Instantiate another Bounce object
Bounce Bouncepin2 = Bounce();

Bounce Bouncepin3 = Bounce();

// Instantiate another Bounce object
Bounce Bouncepin4 = Bounce();


///
 
//Bounce bounce_Storingdata = Bounce();

void setup() {

	for (int i = 0; i <= 3; i++){
		pinMode(RelayInputpins[i], INPUT_PULLUP);

	}
		
	Bouncepin1.attach(RelayInputpins[0]);
	Bouncepin1.interval(bounceinterval); // interval in ms
	Bouncepin2.attach(RelayInputpins[1]);
	Bouncepin2.interval(bounceinterval); // interval in ms
	Bouncepin3.attach(RelayInputpins[2]);
	Bouncepin3.interval(bounceinterval); // interval in ms
	Bouncepin4.attach(RelayInputpins[3]);
	Bouncepin4.interval(bounceinterval); // interval in ms



	for (int i = 0; i <= 3; i++){
		pinMode(Relaypins[i], OUTPUT );
		digitalWrite(Relaypins[i], LOW);
	}
	
		

 
	pinMode(startPin, INPUT_PULLUP);
 
	
	//pinMode(STATUSLED, OUTPUT);
 

	 

	
    
	//bounce_Storingdata.update();


	  readingstart = digitalRead(startPin );

	  if (readingstart) 
      readflashdata();

}

 
void dosetuploop(){

	int reading[4] = { 0, 0, 0, 0 };
	int savedata = 0;
	int fistsave = 1;
//	flashstatus(3);
	while (true)
	{


		if ( digitalRead(startPin) )
		{
			readingstart = true;
			writeflash();
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
			 


			int buttonwaittime = millis();
			 
				 

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
 	


	if (!readingstart)
	{
		
		dosetuploop();

	}
	 
	else
	{
		playback();
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

//	flashstatus(5);

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
		delay(tmp);
	//	digitalWrite(STATUSLED, LOW);
	}
 
	//digitalWrite(STATUSLED, LOW);
}
 





