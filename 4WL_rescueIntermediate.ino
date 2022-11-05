#include <ATX2.h>
int L, M, R, BL, BR;
int refL = 500, refM = 500, refR = 500, refBL = 500, refBR = 500;

extern volatile unsigned long timer0_millis;
unsigned long last_time = 0;

void setup()
{
  XIO();
  servoUp();
}

int mL = 50, mR = 50;
int mLT = 100, mRT = 100;
int sbL = 20, sbR = 20;

int dR = 190, dL = 175;
int dBK = 100;

void loop()
{
  beep(2);
  OK();
  beep(2);
  delay(200);

  turbo(200, 800);
  runInTrack(mL, mR, dBK);
  turn('R', dR);

  turbo(200, 500);
  runInTrack(mL, mR, dBK);
  turn('R', dR);

  turbo(200, 200);
  runInTrack(mL, mR, dBK);
  turn('R', dR);

  turbo(200, 200);
  runInTrack(mL, mR, dBK);
  turn('L', dL);

  turbo(200, 800);
  runInTrack(mL, mR, dBK);
  hit();
  runInTrack(mL, mR, 0);
  moveBlock('B', 100, 100, 240);
  turn('L', dL);
  backwardTrack(sbL, sbR, 200);

  turbo(200, 1200);
  runInTrack(mL, mR, dBK);
  turn('R', dR);

  turbo(200, 100);
  runInTrack(mL, mR, dBK);
  turn('R', dL);

  runInTrack(mL, mR, dBK);
  hit();
  moveBlock('B', 100, 100, 100);
  moveBlock('B', 100, 100, 150);
  turn('R', dL);
  backwardTrack(sbL, sbR, 200);
  backwardTrack(sbL, sbR, 200);

  moveBlock('F', 100, 100, 600);
  turn('R', dL);
  backwardTrack(sbL, sbR, 200);
  backwardTrack(sbL, sbR, 200);

  moveBlock('F', 100, 100, 800);
  runInTrack(mL, mR, 200);
  runInTrack(mL, mR, dBK);
  turn('R', dL);

  turbo(200, 200);
  runInTrack(mL, mR, dBK);
  hit();
  turn('L', dL);
  turn('L', dL);
  backwardTrack(sbL, sbR, 300);

  turbo(200, 500);
  runInTrack(mL, mR, dBK);
  hit();
  moveBlock('B', 100, 100, 230);
  turn('L', dL);
  backwardTrack(sbL, sbR, 150);

  moveBlock('F', 100, 100, 800);
  turbo(200, 200);
  runInTrack(mL, mR, dBK);

  turn('R', dL);
  turbo(200, 200);
  runInTrack(mL, mR, dBK);

  turn('L', dL);
  turbo(200, 400);
  runInTrack(mL, mR, dBK);

  turn('L', dL);
  backwardTrack(sbL, sbR, 200);
  turbo(200, 600);
  runInTrack(mL, mR, dBK);

  turn('R', dL);
  turbo(200, 200);
  runInTrack(mL, mR, dBK);

  turn('L', dL);
  turn('L', dL);
  backwardTrack(sbL, sbR, 200);
  runInTrack(mL, mR, dBK);
  // sensorSet(300);
}

void turbo(int d1, int d2)
{
  timer_runInTrack(mL, mR, dBK, d1);
  timer_runInTrack(mLT, mRT, dBK, d2);
}

void sensorSet(int ref)
{
  glcdClear();
  glcdMode(2);
  setTextSize(2);
  while (true)
  {
    L = analog(0);
    M = analog(1);
    R = analog(2);
    BL = analog(3);
    BR = analog(4);
    if (L < ref)
      glcd(6, 3, "B");
    else
      glcd(6, 3, "W");
    if (M < ref)
      glcd(6, 4, "B");
    else
      glcd(6, 4, "W");
    if (R < ref)
      glcd(6, 5, "B");
    else
      glcd(6, 5, "W");
    if (BL < ref)
      glcd(6, 6, "B");
    else
      glcd(6, 6, "W");
    if (BR < ref)
      glcd(6, 7, "B");
    else
      glcd(6, 7, "W");
    glcd(1, 2, "L = %d    ", L);
    glcd(2, 2, "M = %d    ", M);
    glcd(3, 2, "R = %d    ", R);
    glcd(4, 1, "BL = %d    ", BL);
    glcd(5, 1, "BR = %d    ", BR);
    if (sw_OK())
    {
      break;
    }
  }
}

void runInTrack(int powerL, int powerR, int delayBK)
{
  while (true)
  {
    if (analog(1) <= refM)
    {
      AO();
      delay(300);
      break;
    }
    else if (analog(0) <= refL)
    {
      TR(powerL);
    }
    else if (analog(2) <= refR)
    {
      TL(powerR);
    }
    else if (analog(1) > refM)
    {
      FD2(powerL, powerR);
    }
  }
  BK2(powerL, powerR);
  delay(delayBK);
  AO();

  delay(300);
  beep();
}

void timer_runInTrack(int powerL, int powerR, int delayBK, unsigned long period)
{
  timer0_millis = 0;
  while (true)
  {
    if (analog(1) <= refM)
    {
      break;
    }
    else if (analog(0) <= refL)
    {
      TR(powerL);
    }
    else if (analog(2) <= refR)
    {
      TL(powerR);
    }
    else if (analog(1) > refM)
    {
      FD2(powerL, powerR);
    }

    if (timer_robot(period))
    {
      break;
    }
  }
}

void backwardTrack(int powerL, int powerR, int delayBK)
{
  while (true)
  {
    BK2(powerL, powerR);
    if (analog(3) <= refBL || analog(4) <= refBR)
    {
      break;
    }
  }
  if (analog(3) <= refBL)
  {
    while (analog(4) > refBR)
    {
      motor(1, 0);
      motor(2, 0);
      motor(3, -powerR);
      motor(4, -powerR);
    }
  }
  else if (analog(4) <= refBR)
  {
    while (analog(3) > refBL)
    {
      motor(1, -powerL);
      motor(2, -powerL);
      motor(3, 0);
      motor(4, 0);
    }
  }
  AO();
  delay(300);
  FD2(powerL, powerR);
  delay(delayBK);
  AO();
}

void turn(char t, int ms)
{
  if (t == 'L')
    SL(100);
  else
    SR(100);
  delay(ms);
  AO();
  delay(300);
}

void moveBlock(char d, int powerL, int powerR, int ms)
{
  if (d == 'F')
    FD2(powerL, powerR);
  else
    BK2(powerL, powerR);
  delay(ms);
  AO();
  delay(300);
}

void NB_moveBlock(char d, int powerL, int powerR, int ms)
{
  if (d == 'F')
    FD2(powerL, powerR);
  else
    BK2(powerL, powerR);
  delay(ms);
}

bool timer_robot(unsigned long period)
{
  if (millis() - last_time > period)
  {
    last_time = 0;
    timer0_millis = 0;
    return true;
  }
  return false;
}

void servoUp()
{
  servo(1, 75);
  delay(300);
}

void servoKick()
{
  servo(1, 45);
  delay(300);
}

void hit()
{
  servoKick();
  servoUp();
  beep();
}