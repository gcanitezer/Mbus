//------------------------------------------------------------------------------
// Copyright (C) 2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA 
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------
#include <avr/pgmspace.h>
#include <SPI.h>
#include "printf.h"
#include "sscan.h"
#include <mbus.h>
#include <stdint.h>
#include <SoftwareSerial.h>
#include "mbus_config.h"

static int debug = 0;

#define rxPin 2
#define txPin 3

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);
    printf_begin();
    
  IF_SERIAL_DEBUG(printf_P(PSTR("Starting scan bus \n")));
  
  int i = scanvalue(rxPin,txPin); 
  
  IF_SERIAL_DEBUG(printf_P(PSTR("Finished scan bus \n")));
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  //int sensorValue = analogRead(A0);
  // print out the value you read:
  delay(1000);        // delay in between reads for stability
}




