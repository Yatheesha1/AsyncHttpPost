#ifndef __ASYNC_HTTP_POST_H__
#define __ASYNC_HTTP_POST_H__
#include <AsyncWebSocket.h>

/* define function poiter type */
typedef void(*pfRespOperator)(void *, void *, size_t);

class AsyncHttpPost {
    public:
        void setApi(char *api, uint32_t len);
        void setHost(char *host, uint32_t len);
        void setData(char *data, uint32_t len);
        void setArgs(void *args);
        void setCallback(pfRespOperator callback);
        bool httpPost(void);
        AsyncHttpPost getObject();
        char *getApi(uint32_t *len);
        char *getHost(uint32_t *len);
        char *getData(uint32_t *len);
        void *getArgs();
        unsigned int getLength();
        pfRespOperator getCallback();

    private:
        char *api;
        char *host;
        char *data;
        void *args;
        unsigned int length;
        pfRespOperator resp;

};
#endif