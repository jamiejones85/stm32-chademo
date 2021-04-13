#include <SimpleISA.h>

    int ISA::framecount = 0;
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

void ISA::handle521(uint32_t data[2])  //AMperes

{	
	ISA::framecount++;
	long current=0;
    current = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::milliamps=current;
    ISA::Amperes=current/1000.0f;

    //  if(debug2)Serial<<"Current: "<<Amperes<<" amperes "<<milliamps<<" ma frames:"<<framecount<<"\n";
    	
}

void ISA::handle522(uint32_t data[2])  //Voltage

{	
    ISA::framecount++;
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

void ISA::handle523(uint32_t data[2]) //Voltage2

{	
	ISA::framecount++;
	long volt=0;
    volt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Voltage2 = volt/1000.0f;
    if(ISA::Voltage2 > 3) ISA::Voltage2 -= ISA::Voltage3;
    if(ISA::framecount < 150) ISA::Voltage2LO = ISA::Voltage2;
    if(ISA::Voltage2 < ISA::Voltage2LO  && ISA::framecount > 150) ISA::Voltage2LO = ISA::Voltage2;
    if(ISA::Voltage2 > ISA::Voltage2HI && ISA::framecount > 150) ISA::Voltage2HI = ISA::Voltage2;
    
		
    //  if(debug2)Serial<<"Voltage: "<<Voltage<<" vdc Voltage 2: "<<Voltage2<<" vdc "<<volt<<" mVdc frames:"<<framecount<<"\n";
 
   	
}

void ISA::handle524(uint32_t data[2])  //Voltage3

{	
	ISA::framecount++;
	long volt=0;
    volt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Voltage3 = volt/1000.0f;
    if(ISA::framecount < 150) ISA::Voltage3LO = ISA::Voltage3;
    if(ISA::Voltage3 < ISA::Voltage3LO && ISA::framecount > 150 && ISA::Voltage3 > 10) ISA::Voltage3LO = ISA::Voltage3;
    if(ISA::Voltage3 > ISA::Voltage3HI && ISA::framecount > 150) ISA::Voltage3HI = ISA::Voltage3;
    
    //  if(debug2)Serial<<"Voltage: "<<Voltage<<" vdc Voltage 3: "<<Voltage3<<" vdc "<<volt<<" mVdc frames:"<<framecount<<"\n";
}

void ISA::handle525(uint32_t data[2])  //Temperature

{	
	ISA::framecount++;
	long temp=0;
    temp = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::Temperature=temp/10;
		
    //  if(debug2)Serial<<"Temperature: "<<Temperature<<" C  frames:"<<framecount<<"\n";
   
}



void ISA::handle526(uint32_t data[2]) //Kilowatts

{	
	ISA::framecount++;
	watt = 0;
    watt = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    
    ISA::KW = watt/1000.0f;
		
    //  if(debug2)Serial<<"Power: "<<watt<<" Watts  "<<KW<<" kW  frames:"<<framecount<<"\n";
    
}


void ISA::handle527(uint32_t data[2]) //Ampere-Hours

{	
	ISA::framecount++;
	ISA::As = 0;
    ISA::As = (data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]);
    
    ISA::AH += (ISA::As - ISA::lastAs)/3600.0f;
    ISA::lastAs = ISA::As;

		
    //  if(debug2)Serial<<"Amphours: "<<AH<<"  Ampseconds: "<<As<<" frames:"<<framecount<<"\n";
    
}

void ISA::handle528(uint32_t data[2])  //kiloWatt-hours

{	
	ISA::framecount++;
	
    ISA::wh = (long)((data[5] << 24) | (data[4] << 16) | (data[3] << 8) | (data[2]));
    ISA::KWH += (ISA::wh - ISA::lastWh)/1000.0f;
	ISA::lastWh = ISA::wh;
    //  if(debug2)Serial<<"KiloWattHours: "<<KWH<<"  Watt Hours: "<<wh<<" frames:"<<framecount<<"\n";
   
}

void ISA::Start(Can* can) {

}
void ISA::Stop(Can* can) {

}
void ISA::Restart(Can* can) {

}
void ISA::Default(Can* can) {

}
void ISA::InitCurrent(Can* can) {

}


// void ISA::printCAN(uint32_t data[2])
// {
	
//    //This routine simply prints a timestamp and the contents of the 
//    //incoming CAN message
   
// 	milliseconds = (int) (millis()/1) %1000 ;
// 	seconds = (int) (millis() / 1000) % 60 ;
//     minutes = (int) ((millis() / (1000*60)) % 60);
// 	hours   = (int) ((millis() / (1000*60*60)) % 24);
// 	sprintf(buffer,"%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
// 	Serial<<buffer<<" ";
//     sprintf(bigbuffer,"%02X %02X %02X %02X %02X %02X %02X %02X %02X", 
//     frame->id, frame->data.bytes[0],frame->data.bytes[1],frame->data.bytes[2],
//     frame->data.bytes[3],frame->data.bytes[4],frame->data.bytes[5],frame->data.bytes[6],frame->data.bytes[7],0);
//     Serial<<"Rcvd ISA frame: 0x"<<bigbuffer<<"\n";
    
// }
// void ISA::initialize()
// {
  

// 	firstframe=false;
// 	STOP();
// 	delay(700);
// 	for(int i=0;i<9;i++)
// 	{
	
// Serial.println("initialization \n");
	
// 	outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=(0x20+i);
//         outframe.data.bytes[1]=0x42;  
//         outframe.data.bytes[2]=0x02;
//         outframe.data.bytes[3]=(0x60+(i*18));
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;

// 	   canPort->sendFrame(outframe);
     
//        if(debug)printCAN(&outframe);
// 	   delay(500);
      
//        sendSTORE();
//        delay(500);
//      }
//     //  delay(500);
//       START();
//       delay(500);
//       lastAs=As;
//       lastWh=wh;

                      
// }

// void ISA::STOP()
// {

// //SEND STOP///////

// 	    outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=0x34;
//         outframe.data.bytes[1]=0x00;  
//         outframe.data.bytes[2]=0x01;
//         outframe.data.bytes[3]=0x00;
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;
// 		canPort->sendFrame(outframe);
     
//         if(debug) {printCAN(&outframe);} //If the debug variable is set, show our transmitted frame
// } 
// void ISA::sendSTORE()
// {

// //SEND STORE///////

// 	outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=0x32;
//         outframe.data.bytes[1]=0x00;  
//         outframe.data.bytes[2]=0x00;
//         outframe.data.bytes[3]=0x00;
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;
// 		canPort->sendFrame(outframe);
     
//         if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
        
// }   

// void ISA::START()
// {
           
//  //SEND START///////

// 	    outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=0x34;
//         outframe.data.bytes[1]=0x01;  
//         outframe.data.bytes[2]=0x01;
//         outframe.data.bytes[3]=0x00;
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;
// 		canPort->sendFrame(outframe);
     
//         if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
// }

// void ISA::RESTART()
// {
//          //Has the effect of zeroing AH and KWH  

// 	    outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=0x3F;
//         outframe.data.bytes[1]=0x00;  
//         outframe.data.bytes[2]=0x00;
//         outframe.data.bytes[3]=0x00;
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;
// 		canPort->sendFrame(outframe);
     
//         if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
// }


// void ISA::deFAULT()
// {
//          //Returns module to original defaults  

// 	    outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=0x3D;
//         outframe.data.bytes[1]=0x00;  
//         outframe.data.bytes[2]=0x00;
//         outframe.data.bytes[3]=0x00;
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;
// 		canPort->sendFrame(outframe);
     
//         if(debug)printCAN(&outframe); //If the debug variable is set, show our transmitted frame
// }


// void ISA::initCurrent()
// {
// 	STOP();
// 	delay(500);
	
	
// Serial.println("initialization \n");
	
// 	outframe.id = 0x411;      // Set our transmission address ID
//         outframe.length = 8;       // Data payload 8 bytes
//         outframe.extended = 0; // Extended addresses  0=11-bit1=29bit
//         outframe.rtr=1;                 //No request
//         outframe.data.bytes[0]=(0x21);
//         outframe.data.bytes[1]=0x42;  
//         outframe.data.bytes[2]=0x01;
//         outframe.data.bytes[3]=(0x61);
//         outframe.data.bytes[4]=0x00;
//         outframe.data.bytes[5]=0x00;
//         outframe.data.bytes[6]=0x00;
//         outframe.data.bytes[7]=0x00;

// 		canPort->sendFrame(outframe);
     
//        if(debug)printCAN(&outframe);
// 	delay(500);
      
//        sendSTORE();
//        delay(500);
    
//     //  delay(500);
//       START();
//       delay(500);
//       lastAs=As;
//       lastWh=wh;                      
// }
