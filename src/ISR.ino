void ISRwatchDog () {
  noInterrupts();
  watchDog++;
  if (watchDog == 60) {
    diagMess("watchDog 60s alert");
  }
  if (watchDog >= 90) {
    errMess("watchDog 90s");
    ESP.restart();
  }
  interrupts();
}
