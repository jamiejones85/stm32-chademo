#ifndef SimpleISA_h
#define SimpleISA_h
#include <stdint.h>
#include "my_math.h"
#include "my_fp.h"
#include "stm32_can.h"

class ISA
{
   public:
      static void handle511(uint32_t data[8], uint32_t time);
      static void handle521(uint32_t data[8], uint32_t time);
      static void handle522(uint32_t data[8], uint32_t time);
      static void handle523(uint32_t data[8], uint32_t time);
      static void handle524(uint32_t data[8], uint32_t time);
      static void handle525(uint32_t data[8], uint32_t time);
      static void handle526(uint32_t data[8], uint32_t time);
      static void handle527(uint32_t data[8], uint32_t time);
      static void handle528(uint32_t data[8], uint32_t time);
      static bool Alive(uint32_t time);
      
      static void Start(Can* can);
      static void Stop(Can* can);
      static void Restart(Can* can);
      static void Default(Can* can);
      static void InitCurrent(Can* can);
      static void Store(Can* can);
      static void initialize(Can* can);

	  static float Amperes;   // Floating point with current in Amperes
	  static double AH;      //Floating point with accumulated ampere-hours
	  static double KW;
	  static double KWH;

      static double Voltage;
      static double Voltage1;
      static double Voltage2;
      static double Voltage3;
      static double VoltageHI;
      static double Voltage1HI;
      static double Voltage2HI;
      static double Voltage3HI;
      static double VoltageLO;
      static double Voltage1LO;
      static double Voltage2LO;
      static double Voltage3LO;
      static double milliamps;
      static double Temperature;
      static long watt;
	  static long As;
	  static long lastAs;
	  static long wh;
  	  static long lastWh;	

   protected:

   private:
        static int framecount;
        static uint32_t lastRecv;



};

#endif 