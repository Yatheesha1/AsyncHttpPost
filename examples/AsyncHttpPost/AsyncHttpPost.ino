#include <WiFi.h>
#include "AsyncHttpPost.h"

#define ssid "JioFi Private"
#define pwd "robotics"

#define MESSAGE_MAX_LEN 500
char* path = "/post";
char* host = "httpbin.org";

long currentTime = 0;
int times;

void respFromServer(void *args, void* data, size_t len) {
    currentTime = millis();
    char * p1, *p2;
    char * ch = (char*)data;
    ch[len] = '\0';
    Serial.printf("%s\n", (char*)args);
    Serial.printf("Response --> %s\n", ch);
    Serial.printf("Time --> %d\n", currentTime-times);
}

void postToServer(String input) {
    char *args = "Sample arg is char message";
    AsyncHttpPost asyncHttpPost;
    asyncHttpPost.setApi(path, strlen(path));
    asyncHttpPost.setHost(host, strlen(host));
    asyncHttpPost.setData((char*)(input.c_str()), input.length());
    asyncHttpPost.setArgs((void*)args);
    asyncHttpPost.setCallback(respFromServer);

    times = millis();
    asyncHttpPost.httpPost();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Begin");
    WiFi.begin(ssid,pwd); 
    while (WiFi.status() != WL_CONNECTED) {
        Serial.printf(".");
        delay(500);
    }
    
    Serial.printf("\nConnected to %s\n", WiFi.SSID().c_str());

    String input_sample = "{\"id\": 1, \"name\": \"Jessa\"}";
    Serial.printf("\nSample request %s\n", input_sample.c_str());
    postToServer(input_sample);
}

void loop() {
    String input;
    if (Serial.available()>0) {
        input = Serial.readStringUntil('\n');
        Serial.printf("Input: %s\n", input.c_str());

        postToServer(input);
    }
}