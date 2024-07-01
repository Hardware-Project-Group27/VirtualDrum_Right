#ifndef WEBSOCKETCON
#define WEBSOCKETCON

#include <WebSockets.h>

class WSMsgRecievedHandler; // forward declaration 

class WebSocketCon{


    private:

        WSMsgRecievedHandler* wsMsgRecievedHandler;

        void setup_websocket();
        static void hexdump(const void *mem, uint32_t len, uint8_t cols = 16);
        static void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);


    public:
        WebSocketCon();
        void setup();
        void loop();
        void sendMsg(String msg);
        void setWSMsgRecievedHandler(WSMsgRecievedHandler* wsMsgRecievedHandler);

};

#endif