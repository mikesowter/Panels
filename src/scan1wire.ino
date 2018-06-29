void scan1wire() {
  while ( ds.search(addr)) {

    if ( OneWire::crc8( addr, 7) != addr[7]) {
      fe=SPIFFS.open("/scanErrs.txt","a");
      fe.print("\naddr = ");
      for ( i = 0; i < 3; i++) {
        if (addr[i]<16) fe.print("0");
        fe.print(addr[i], HEX);
        fe.print(" ");
      }
      fe.print("\nbad CRC!\n");
      fe.close();
      return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44,1);        // start conversion, with parasite power on
    t0=millis();
    while (millis()-t0 < 750UL) {  // wait 750ms for 12 bit conversion
      yield();
      //  check for web requests
      server.handleClient();
      // check for OTA
      ArduinoOTA.handle();
    }
    ds.reset();
    ds.select(addr);
    ds.write(0xBE,1);        // Read Scratchpad

    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
    /*    if (data[i]<16) Serial.print("0");
      Serial.print(data[i], HEX);
      Serial.print(" ");  */
    }
    for (i=0; i < numProbes; i++) {
      if ((addr[1]==(knownAddr[i]>>8)) && (addr[2]==(knownAddr[i]&0xFF))) {
        probe=i;
      }
    }
    // Convert the data to actual temperature
    int16_t raw = (data[1] << 8) | data[0];
    //  check for MAX31850 thermocouple interface
    if ( addr[0] == 0x3B) {
      if (data[7] == 0x10) {
        // "count remain" gives offset adjustment
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    }
    //  check for unknown interface
    else if ( addr[0] != 0x28) {
      uint8_t cfg = (data[4] & 0x60);
      Serial.print("\n\rnew probe: ");
      Serial.print(cfg,HEX);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      // no else - do nothing if 12 bit
    }
    celsius = (float)raw / 16.0;

    if (celsius > 0.0 && celsius < 50.0) {
      lastTemp[probe] = celsius;
      sumTemp[probe] += celsius;
      numSamp[probe] += 1;
    }
  }
ds.reset_search();
return;
}
