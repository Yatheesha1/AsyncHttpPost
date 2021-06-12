#include "AsyncHttpPost.h"

static AsyncClient * aClient = NULL;
AsyncHttpPost g_asyncHttpPost;

void AsyncHttpPost::setHost(String host)
{
    this->host = host;
}

void AsyncHttpPost::setApi(String api)
{
    this->api = api;
}

void AsyncHttpPost::setData(String data)
{
    this->data = data;
    this->length = data.length();
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

String AsyncHttpPost::getApi() {
    return this->api;
}

String AsyncHttpPost::getHost() {
    return this->host;
}

String AsyncHttpPost::getData() {
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

    Serial.printf("\nBody: %s\n", g_asyncHttpPost.getData().c_str());
    if (aClient)                                      /* return client already exists */
        return false;

    aClient = new AsyncClient();                      /* initialize client object */
    if (!aClient)                                     /* could not allocate client */
        return false;

    aClient->onError([](void * arg, AsyncClient * client, int error) {  /* delete client if has an error in sendind data */
        aClient = NULL;
        delete client;
    }, NULL);

    aClient->onConnect([](void * arg, AsyncClient * client) {            /* Enter if client connected to cloud */
        Serial.println("Client Connected");
        aClient->onError(NULL, NULL);

        client->onDisconnect([](void * arg, AsyncClient * c) {             /* called after data sent or disconnected */
            aClient = NULL;
            delete c;
        }, arg);

        client->onData([](void * arg, AsyncClient * c, void * data, size_t len) {  /* called once client receives data as response */
            Serial.printf("Response recieved\n");
            pfRespOperator callback = g_asyncHttpPost.getCallback();
            callback(arg, data, len);
        }, arg);

        Serial.printf("Generating request\n");

        /* request query to send to client */
        String req = String("POST ") + g_asyncHttpPost.getApi() + " HTTP/1.1\r\n" +
                    "Host: " + g_asyncHttpPost.getHost() + "\r\n" +
                    "Connection: close\r\n" +
                    "Content-Length: " + g_asyncHttpPost.getLength() + "\r\n" +
                    "Content-Type: application/json;charset=UTF-8\r\n\r\n" +
                    g_asyncHttpPost.getData() + "\r\n";
        Serial.printf("Request: %s\n", req.c_str());
        client->write(req.c_str());                             /* send the request */
        yield();
    }, args);

    if (!aClient->connect(g_asyncHttpPost.getHost().c_str(), 80)) {                /* connect to cloud server if not connected */
        Serial.printf("\nHost: %s\n", g_asyncHttpPost.getHost().c_str());
        Serial.println("Connect Fail");
        AsyncClient * client = aClient;
        aClient = NULL;
        delete client;
        return false;
    }

    return true;
}
/* end of http request */
////////////////////////////////////////////////////////////////////////////////////////////////////////