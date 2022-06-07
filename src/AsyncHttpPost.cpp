#include "AsyncHttpPost.h"
#define REQ_MAX_LEN 500

const char *request = "POST %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nContent-Length: %d\r\nContent-Type: application/json;charset=UTF-8\r\n\r\n%s\r\n";

// static AsyncClient * aClient = NULL;
AsyncHttpPost g_asyncHttpPost;

AsyncHttpPost::~AsyncHttpPost() {

}

void AsyncHttpPost::setHost(char* host, uint32_t len)
{
    this->host = host;
}

void AsyncHttpPost::setApi(char* api, uint32_t len)
{
    this->api = api;
}

void AsyncHttpPost::setData(char* data, uint32_t len)
{
    this->data = data;
    this->length = len;
}

void AsyncHttpPost::setArgs(void *args)
{
    this->args = args;
}

void AsyncHttpPost::setCallback(pfRespOperator callback)
{
    this->resp = callback;
}

AsyncHttpPost AsyncHttpPost::getObject() {
    return *this;
}

char *AsyncHttpPost::getApi(uint32_t *len) {
    *len = strlen(this->api);
    return this->api;
}

char *AsyncHttpPost::getHost(uint32_t *len) {
    *len = strlen(this->host);
    return this->host;
}

char *AsyncHttpPost::getData(uint32_t *len) {
    *len = strlen(this->data);
    return this->data;
}

void *AsyncHttpPost::getArgs()
{
    return this->args;
}

unsigned int AsyncHttpPost::getLength() {
    return this->length;
}

pfRespOperator AsyncHttpPost::getCallback() { 
    return this->resp; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Start of http request */
bool AsyncHttpPost::httpPost() {
    g_asyncHttpPost = this->getObject();
    uint32_t len = 0;

    AsyncClient * aClient = NULL;

    // Serial.printf("\nBody: %s\n", g_asyncHttpPost.getData(&len));
    if (aClient)                                      /* return client already exists */
        return false;

    aClient = new AsyncClient();                      /* initialize client object */
    if (!aClient)                                     /* could not allocate client */
        return false;

    aClient->onError([](void * arg, AsyncClient * client, int error) {  /* delete client if has an error in sendind data */
        delete client;
        client = NULL;
    }, args);

    aClient->onConnect([](void * arg, AsyncClient * client) {            /* Enter if client connected to cloud */
        Serial.println(F("Client Connected"));
        client->onError(NULL, NULL);

        client->onDisconnect([](void * arg, AsyncClient *client) {             /* called after data sent or disconnected */
            delete client;
            client = NULL;
        }, arg);

        client->onData([](void * arg, AsyncClient * c, void * data, size_t len) {  /* called once client receives data as response */
            Serial.println(F("Response recieved"));
            pfRespOperator callback = g_asyncHttpPost.getCallback();
            callback(arg, data, len);
        }, arg);

        // Serial.printf("Generating request\n");
        uint32_t len = 0;

        /* request query to send to client */
        // String req = String("POST ") + String(g_asyncHttpPost.getApi(&len)) + " HTTP/1.1\r\n" +
        //             "Host: " + String(g_asyncHttpPost.getHost(&len)) + "\r\n" +
        //             "Connection: close\r\n" +
        //             "Content-Length: " + g_asyncHttpPost.getLength() + "\r\n" +
        //             "Content-Type: application/json;charset=UTF-8\r\n\r\n" +
        //             String(g_asyncHttpPost.getData(&len)) + "\r\n";

        char req[REQ_MAX_LEN];
        snprintf(req, REQ_MAX_LEN, request, g_asyncHttpPost.getApi(&len), g_asyncHttpPost.getHost(&len), g_asyncHttpPost.getLength() , g_asyncHttpPost.getData(&len));
        // Serial.printf("Request: %s\n", req);
        client->write(req);                             /* send the request */
        yield();
    }, args);

    if (!aClient->connect(g_asyncHttpPost.getHost(&len), 80)) {                /* connect to cloud server if not connected */
        Serial.printf("\nHost: %s\n", g_asyncHttpPost.getHost(&len));
        Serial.println(F("Connection Failed"));
        delete aClient;
        aClient = NULL;
        return false;
    }

    return true;
}
/* end of http request */
////////////////////////////////////////////////////////////////////////////////////////////////////////