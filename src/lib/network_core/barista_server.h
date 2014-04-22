#ifndef _BARISTA_SERVER_H_
#define _BARISTA_SERVER_H_

#include <map>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "net_tcp/tcp_server.h"
#include "volatile_metadata/volatile_metadata_c_api.h"

class BaristaServer : public TcpServer {

  private:

    uint32_t next_node_num;

    std::map<int, ConnectionToClient*> m_espresso_nodes;
    std::map<int, ConnectionToClient*> m_decafs_clients;
    ConnectionToClient* cl;

  public:
    BaristaServer(unsigned short port);

    void clientConnected(ConnectionToClient* client);
    void clientDisconnected(ConnectionToClient* client);
    void handleMessageFromClient(void* buf, ssize_t length, ConnectionToClient* client);
    void serverClosed();
    void serverStarted();
    void serverStopped();

    int numEspressoNodes();
    // for testing only
    ConnectionToClient* getEspressoNode();
}; 

#endif // _BARISTA_SERVER_H_