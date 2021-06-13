#ifndef __ASYNC_HTTP_POST_H__
#define __ASYNC_HTTP_POST_H__
#include <AsyncWebSocket.h>

/* define function poiter type */
typedef void(*pfRespOperator)(void *, void *, size_t);

class AsyncHttpPost {
    public:
        void setApi(String api);
        void setHost(String host);
        void setData(String data);
        void setArgs(void *args);
        void setCallback(pfRespOperator callback);
        bool httpPost(void);
        AsyncHttpPost getObject();
        String getApi();
        String getHost();
        String getData();
        void * getArgs();
        unsigned int getLength();
        pfRespOperator getCallback();

    private:
        String api;
        String host;
        String data;
        void *args;
        unsigned int length;
        pfRespOperator resp;

};
#endif