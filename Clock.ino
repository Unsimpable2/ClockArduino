#include <TM1638plus.h>

#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7

#define DelaySec 1000
#define DelayKey 100

short int sec1 = 0;
short int sec2 = 0;
short int min1 = 0;
short int min2 = 0;
short int hour1 = 0;
short int hour2 = 0;

uint8_t buttons = 0;

short int sum1 = 103;
short int sum2 = 600;
short int sum3 = 24;

bool high_freq = false; 

TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);

void setup()
{
  Serial.begin(9600);
  tm.displayBegin();
  tm.reset();
}

void loop() 
{
  start();
  clock();
}

void start()
{
  tm.displayHex(7, sec1);
  tm.displayHex(6, sec2);

  tm.displayASCII(5, 45);

  tm.displayHex(4, min1);
  tm.displayHex(3, min2);

  tm.displayASCII(2, 45);

  tm.displayHex(1, hour1);
  tm.displayHex(0, hour2);
}

void clock()
{
  buttons = tm.readButtons();
  
  if(buttons == 0x01)
  {
    sec1 = 0;
    sec2 = 0;
    knefle();
  }

  delay(DelaySec);

  sec1++;

  if(sec1 == 9)
  {
    tm.displayHex(7, sec1);
    delay(DelaySec);
    sec2++;
    sec1 = 0;
  }

  if(sec2 == 6)
  {
    min1++;
    sec1 = 0;
    sec2 = 0;
  }

  if((min1 * 10 + sec2 + sec1) == sum1)
  {
    tm.displayHex(4, min1);
    min2++;
    min1 = 0;
    sec1 = 0;
    sec2 = 0;
  }

  if(min2 == 6)
  {
    hour1++;
    min2 = 0;
    min1 = 0;
    sec1 = 0;
    sec2 = 0;
  }

  if((hour1 * 60 + min2) == sum2)
  {
    tm.displayHex(1, hour1);
    hour2++;
    hour1 = 0;
    min2 = 0;
    min1 = 0;
    sec1 = 0;
    sec2 = 0;
  }

  if((hour2 * 10 + hour1) == sum3)
  {
    hour2 = 0;
    hour1 = 0;
    min1 = 0;
    min2 = 0;
    sec1 = 0;
    sec2 = 0;
  }
}

void knefle()
{
  while(1)
  {
    tm.setLED(3, 1);
    tm.setLED(4, 1);
    
    buttons = 0;
    buttons = tm.readButtons();

    start();

    if(buttons == 0x08)
    {
      min1++;

      if(min1>9)
      {
        min1=0;
        min2++;
      }

      if(min2>5)
      {
        min2 = 0;
        hour1++;
      }

      if(hour1>9)
      {
        hour1=0;
        hour2++;
      }

      if(hour2 == 2 && hour1>3)
      {
        hour2 = 0;
        hour1 = 0;
      }

      delay(DelayKey);
    }

    if(buttons == 0x20)
    {
      min1--;

      if(min1<0)
      {
        min1 = 9;
        min2--;
      }

      if(min2<0)
      {
        min2 = 5;
        hour1--;
      }

      if(hour1<0)
      {
        hour1 = 9;
        hour2--;
      }

      if(hour2<0)
      {
        hour2 = 2;
        hour1 = 3;
      }

      delay(DelayKey);
    }

    if(buttons == 0x80)
    {
      tm.setLED(3, 0);
      tm.setLED(4, 0);
      buttons = 0;
      break;
    }    
  }
}