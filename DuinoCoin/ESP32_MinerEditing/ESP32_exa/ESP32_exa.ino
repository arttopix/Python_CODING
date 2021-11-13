  const char* serverName = "http://10.10.1.1/api/espCheckin.php"; // the URL of my checking API

  unsigned long checkinDelay = 10000; // Check in to the IOT service every 10 seconds
  unsigned long wdtResetDelay = 1000; // How often to reset the watchdog timer

  unsigned long lastCheckin = 0;
  unsigned long lastWdtReset = 0;
  unsigned long lastShares = 0;

  esp_task_wdt_add(NULL);// Register this task with the watchdog
  for(;;) // Loop forever
  { 
    if ((millis() - lastWdtReset) > wdtResetDelay) 
    {
      esp_task_wdt_reset();
      lastWdtReset = millis();
    }

    if (WiFi.status() == WL_CONNECTED && (millis() - lastCheckin) > checkinDelay) 
    {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.setTimeout(2000); // Set the timeout low so we don't hang anything too much
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = String("rigname=") + rig_identifier;
      httpRequestData = httpRequestData + "&username=" + username;
      httpRequestData = httpRequestData + "&pool=" + host + ":" + port;
      httpRequestData = httpRequestData + "&accepted_1=" + shares_one;
      httpRequestData = httpRequestData + "&accepted_2=" + shares_two;
      httpRequestData = httpRequestData + "&hashrate_1=" + hashrate_one;
      httpRequestData = httpRequestData + "&hashrate_2=" + hashrate_two;
      httpRequestData = httpRequestData + "&difficulty_1=" + diff_one;
      httpRequestData = httpRequestData + "&difficulty_2=" + diff_two;
      httpRequestData = httpRequestData + "&connected=" + ((shares_one + shares_two)>lastShares);
      
      int httpResponseCode = http.POST(httpRequestData);
      if(0) 
      { // Save cycles and I don't care what they said, but could do
        yield();
        Serial.print("CHECKIN: Response code: ");
        Serial.print(httpResponseCode);
        if(httpResponseCode == HTTP_CODE_OK) 
        {
          String payload = http.getString();
          DynamicJsonDocument doc(1024);
          char buffer[2048];
  Job received
          yield();
          deserializeJson(doc, payload);
          serializeJsonPretty(doc, buffer);
          Serial.print(" ");
          Serial.println(buffer);
        } 
        else 
        {
          Serial.print(", Error: ");
          Serial.println(http.errorToString(httpResponseCode).c_str());
        }
      }
      http.end();
      lastCheckin = millis();
      lastShares = shares_one + shares_two;
    }
    yield();
  }
