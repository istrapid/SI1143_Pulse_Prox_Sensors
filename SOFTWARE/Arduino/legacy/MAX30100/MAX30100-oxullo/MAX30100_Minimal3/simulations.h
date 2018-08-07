float s = 00.00;

void normalOp() {
  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  pox.update();
  s = pox.getSpO2();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(pox.getHeartRate(), 1);
    display.println(" bpm");
    display.print(pox.getSpO2());
    display.println(" SPO2");
    display.print(pox.getTemperature(), 2);
    display.println(" C");
    display.display();

    tsLastReport = millis();
  }
}

// Simulations
void startSim(byte scenario) {
  //randomSeed(pox.getTemperature() + millis());
  float modifier;

  switch (scenario) {

    // Tachycardia
    case SIM_000:
      randomSeed(pox.getHeartRate());
      BTooth.write( ACK );
      delay( 10 );

      pox.update();
      s = pox.getSpO2();
      modifier = random(100, 120) + (random(0, 99)/100.0);
      if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(modifier, 2);
        display.println(" bpm");
        display.print(pox.getSpO2(), 1);
        display.println(" SPO2");
        display.print(pox.getTemperature(), 2);
        display.println(" C");
        display.display();

        tsLastReport = millis();
      }
      break;

    //Low Temp (Hypothermia)
    case SIM_001:
      BTooth.write( ACK );
      // Do stuff
      break;
  }
}
