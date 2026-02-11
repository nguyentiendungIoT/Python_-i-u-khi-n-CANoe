#pragma once

#include "SilAdapterWrapper.h"

/* Initializes the all the IO functionality. */
void InitializeIO(void);

/* Shuts down all the IO functionality. */
void ShutDownIO(void);

/* This function reads a value from the 1st sensor. */
double GetSensorValue1(void);

/* This function reads a value from the 2nd sensor. */
double GetSensorValue2(void);

/* This function reads a value from the 3rd sensor. */
double GetSensorValue3(void);

/* This function sets the state of the heating. */
void SetHeatingState(enum State heatingState, enum State coolingState);
