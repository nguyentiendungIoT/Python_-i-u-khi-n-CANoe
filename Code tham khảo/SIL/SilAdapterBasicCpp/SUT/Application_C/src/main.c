/*
 * This file contains the application's entry point.
 */

#include <stdio.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

#include "Logic.h"
#include "IO.h"

int main()
{

  printf("Room Temperature Control application, version 1.0\r\n\r\n");

  // Initializes the SiL adapter and the connection to CANoe
  InitializeIO();

  for(;;)
  {
    ControlRoomTemperature();
    Sleep(100);
  }

  // Shuts down the SiL adapter and the connection to CANoe
  ShutDownIO();

  return 0;
}
