/*
 * This file contains the application's entry point.
 */

#include <iostream>
#include <chrono>
#include <thread>

#include "Logic.hpp"
#include "IO.hpp"

int main()
{

  std::cout << "Room Temperature Control application, version 1.0" << std::endl;

  InitializeIO();

  while (true)
  {
    ControlRoomTemperature();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  ShutDownIO();

  return 0;
}
