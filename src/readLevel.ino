/*float readLevel() {
  lastReading = waterLevel;
  i2c_master_read_from(8, reply, 5, FALSE);
  if (reply[0]=='L') {
    if (reply[1]==reply[3]&&reply[2]==reply[4]) {
      float mmDown=((float)reply[1]*256.0+(float)reply[2])/100.0;
      waterLevel = 39.0 - mmDown/10.0; // 20cm is bottom of skimmer, 30cm is top
//      median();
      if (lastReading > 1.0) waterLevel = 0.99*lastReading + .01*waterLevel;
    }
    else diagMess("data corruption in pool level");
  }
  if (waterLevel < 1.0 || waterLevel > 39.0) waterLevel=0.0;
}

float median() {
  medBuf(medPtr++)=waterLevel;
  medPtr&=0x3F;
  for (int i=0;i<64;i++) {    // iterate thru the last 64 values
    if (medBuf(i)==0.0) break;  // should only be 0 at startup

  }




}; */
