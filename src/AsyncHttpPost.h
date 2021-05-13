#ifndef __ASYNC_HTTP_POST_H__
#define __ASYNC_HTTP_POST_H__
#include <AsyncWebSocket.h>

/* define function poiter type */
typedef void(*pfRespOperator)(void *, size_t);

class AsyncHttpPost {
    public:
        void setApi(String api);
        void setHost(String host);
        void setData(String data);
        void setCallback(pfRespOperator callback);
        void httpPost(void);
        AsyncHttpPost getObject();
        String getApi();
        String getHost();
        String getData();
        unsigned int getLength();
        pfRespOperator getCallback();

    private:
        String api;
        String host;
        String data;
        unsigned int length;
        pfRespOperator resp;

};
#endif