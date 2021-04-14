#include <SimpleISA.h>

    float ISA::Amperes;   // Floating point with current in Amperes
	double ISA::AH;      //Floating point with accumulated ampere-hours
	double ISA::KW;
	double ISA::KWH;

    double ISA::Voltage;
    double ISA::Voltage1;
    double ISA::Voltage2;
    double ISA::Voltage3;
    double ISA::VoltageHI;
    double ISA::Voltage1HI;
    double ISA::Voltage2HI;
    double ISA::Voltage3HI;
    double ISA::VoltageLO;
    double ISA::Voltage1LO;
    double ISA::Voltage2LO;
    double ISA::Voltage3LO;
    double ISA::milliamps;
    double ISA::Temperature;
    long ISA::watt;
	long ISA::As;
	long ISA::lastAs;
	long ISA::wh;
  	long ISA::lastWh;	
    uint32_t ISA::lastRecv = 0;
    int ISA::framecount = 0;

bool ISA::Alive(uint32_t time)
{
   return (time - ISA::lastRecv) < 100;
}

void ISA::handle521(uint32_t data[8], uint32_t time)  //AMperes

{	
	ISA::lastRecv = time;
    ISA::framecount++;
	long current=0;
    current = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::milliamps=current;
    ISA::Amperes=current/1000.0f;

    //  if(debug2)Serial<<"Current: "<<Amperes<<" amperes "<<milliamps<<" ma frames:"<<framecount<<"\n";
    	
}

void ISA::handle522(uint32_t data[8], uint32_t time)  //Voltage

{	
    ISA::framecount++;
    ISA::lastRecv = time;
	long volt=0;
    volt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Voltage=volt/1000.0f;
	ISA::Voltage1=Voltage-(Voltage2+Voltage3);
    if(ISA::framecount<150)
    {
        ISA::VoltageLO = ISA::Voltage;
        ISA::Voltage1LO = ISA::Voltage1;
    }
    if(ISA::Voltage < ISA::VoltageLO &&  ISA::framecount>150) ISA::VoltageLO = ISA::Voltage;
    if(ISA::Voltage > ISA::VoltageHI && ISA::framecount>150) ISA::VoltageHI = ISA::Voltage;
    if(ISA::Voltage1 < ISA::Voltage1LO && ISA::framecount>150) ISA::Voltage1LO = ISA::Voltage1;
    if(ISA::Voltage1 > ISA::Voltage1HI && ISA::framecount>150) ISA::Voltage1HI = ISA::Voltage1;
  
    // if(debug2)Serial<<"Voltage: "<<Voltage<<" vdc Voltage 1: "<<Voltage1<<" vdc "<<volt<<" mVdc frames:"<<framecount<<"\n";
     	
}

void ISA::handle523(uint32_t data[8], uint32_t time) //Voltage2

{	
	ISA::framecount++;
    ISA::lastRecv = time;
	long volt=0;
    volt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Voltage2 = volt/1000.0f;
    if(ISA::Voltage2 > 3) ISA::Voltage2 -= ISA::Voltage3;
    if(ISA::framecount < 150) ISA::Voltage2LO = ISA::Voltage2;
    if(ISA::Voltage2 < ISA::Voltage2LO  && ISA::framecount > 150) ISA::Voltage2LO = ISA::Voltage2;
    if(ISA::Voltage2 > ISA::Voltage2HI && ISA::framecount > 150) ISA::Voltage2HI = ISA::Voltage2;
    
		
    //  if(debug2)Serial<<"Voltage: "<<Voltage<<" vdc Voltage 2: "<<Voltage2<<" vdc "<<volt<<" mVdc frames:"<<framecount<<"\n";
 
   	
}

void ISA::handle524(uint32_t data[8], uint32_t time)  //Voltage3

{	
	ISA::framecount++;
    ISA::lastRecv = time;
	long volt=0;
    volt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Voltage3 = volt/1000.0f;
    if(ISA::framecount < 150) ISA::Voltage3LO = ISA::Voltage3;
    if(ISA::Voltage3 < ISA::Voltage3LO && ISA::framecount > 150 && ISA::Voltage3 > 10) ISA::Voltage3LO = ISA::Voltage3;
    if(ISA::Voltage3 > ISA::Voltage3HI && ISA::framecount > 150) ISA::Voltage3HI = ISA::Voltage3;
    
    //  if(debug2)Serial<<"Voltage: "<<Voltage<<" vdc Voltage 3: "<<Voltage3<<" vdc "<<volt<<" mVdc frames:"<<framecount<<"\n";
}

void ISA::handle525(uint32_t data[8], uint32_t time)  //Temperature

{	
	ISA::framecount++;
    ISA::lastRecv = time;
	long temp=0;
    temp = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Temperature=temp/10;
		
    //  if(debug2)Serial<<"Temperature: "<<Temperature<<" C  frames:"<<framecount<<"\n";
   
}



void ISA::handle526(uint32_t data[8], uint32_t time) //Kilowatts

{	
	ISA::framecount++;
    ISA::lastRecv = time;
	watt = 0;
    watt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::KW = watt/1000.0f;
		
    //  if(debug2)Serial<<"Power: "<<watt<<" Watts  "<<KW<<" kW  frames:"<<framecount<<"\n";
    
}


void ISA::handle527(uint32_t data[8], uint32_t time) //Ampere-Hours

{	
	ISA::framecount++;
    ISA::lastRecv = time;
	ISA::As = 0;
    ISA::As = (data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]);
    
    ISA::AH += (ISA::As - ISA::lastAs)/3600.0f;
    ISA::lastAs = ISA::As;

		
    //  if(debug2)Serial<<"Amphours: "<<AH<<"  Ampseconds: "<<As<<" frames:"<<framecount<<"\n";
    
}

void ISA::handle528(uint32_t data[8], uint32_t time)  //kiloWatt-hours

{	
	ISA::framecount++;
	ISA::lastRecv = time;	
    ISA::wh = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    ISA::KWH += (ISA::wh - ISA::lastWh)/1000.0f;
	ISA::lastWh = ISA::wh;
    //  if(debug2)Serial<<"KiloWattHours: "<<KWH<<"  Watt Hours: "<<wh<<" frames:"<<framecount<<"\n";
   
}

void ISA::Start(Can* can) {
           
    uint32_t data[8] = { 0x34, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
    can->Send(0x411, data);
     
    //if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
}

void ISA::Stop(Can* can) {
    uint32_t data[8] = { 0x34, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
    can->Send(0x411, data);

    //if(debug) {printCAN(&outframe);} //If the debug variable is set, show our transmitted frame
}
void ISA::Restart(Can* can) {
    //Has the effect of zeroing AH and KWH  
    uint32_t data[8] = { 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    can->Send(0x411, data);
     
    //if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
}
void ISA::Default(Can* can) {
    //Returns module to original defaults  
    uint32_t data[8] = { 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    can->Send(0x411, data);
    //if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
}
void ISA::InitCurrent(Can* can) {
    ISA::Stop(can);
    //delay(500);?

    uint32_t data[8] = { 0x21, 0x42, 0x01, 0x61, 0x00, 0x00, 0x00, 0x00 };
    can->Send(0x411, data);

    //if(debug)printCAN(&outframe);
    //delay(500);
      
    ISA::Store(can);
    //delay(500);
    ISA::Start(can);

    //delay(500);
    lastAs=As;
    lastWh=wh;                
}

void ISA::Store(Can* can) {

    uint32_t data[8] = { 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    can->Send(0x411, data);
    //if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
}


void ISA::initialize(Can* can) {
  
    ISA::Stop(can);
    // 	delay(700);?
        uint32_t data[8] = { 0x20, 0x42, 0x02, 0x60, 0x00, 0x00, 0x00, 0x00 };

    for(int i=0;i<9;i++) {
        data[0] = 0x20 + i;
        data[3] = 0x60 + (i*18);
        can->Send(0x411, data);
     
        //if(debug)printCAN(&outframe);
        //delay(500);
      
        ISA::Store(can);
        //delay(500);
    }
    
    //delay(500);
    ISA::Start(can);
    //delay(500);
    ISA::lastAs = ISA::As;
    ISA::lastWh = ISA::wh;

                      
}
