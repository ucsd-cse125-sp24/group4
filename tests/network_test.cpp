#include <stdlib.h>
#include <string>
#include <vector>
#include "../networking/client.h"
#include "../networking/server.h"

#define NUM_TRIALS 128
#define MAX_DATA_LENGTH 512

// CreateThread doesn't like calling an object's member function;
// call non-member function which calls the obj's member function instead
DWORD __stdcall call_listen(void* server){
   auto object = reinterpret_cast<Server*>(server);
   if (object){
     object->sock_listen();
   } 
   return 0U;
} 

int test_connect(int num_clients) {
    /*
    Test that clients can be hosted on a server

    @param num_clients: the number of clients which should connect to the server
    @returns 0 upon successful connection, 1 upon failure
    */

    // set up server and listen for clients
    Server server = Server();
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &server, 0, &threadID);

    // connect specified number of clients
    std::vector<Client> client_list;
    for (int i = 0; i < num_clients; i++) {
        // connect client to server; if failure, return 1
        client_list.push_back(Client());
    }
    WaitForSingleObject(hand, 100000);

    // confirm correct number of connections
    if (server.get_num_clients() != num_clients)
        return 1;

    // clean up
    for (Client client : client_list) {
            client.close_conn();
    }
    server.sock_shutdown();

    return 0;
}

int test_data_transport() {
    /*
    Test that data can be communicated between server and client

    @returns 0 if all data is sent and received successfully, 1 upon failure
    */

    // create server-client pair to use
    Server server = Server();
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &server, 0, &threadID);
    Client client = Client();
    WaitForSingleObject(hand, 100000);

    // set up buffers for data, each of length MAX_DATA_LENGTH
    char client_buf[MAX_DATA_LENGTH];
    char server_buf[MAX_DATA_LENGTH];

    srand(0);
    int trial_data_len;
    int j;
    for (int i = 0; i < NUM_TRIALS; i++) {
        // generate random data, w length in range (1, MAX_DATA_LENGTH) + null terminator
        trial_data_len = rand() % (MAX_DATA_LENGTH - 1) + 1;
        for (j = 0; j < trial_data_len; j++)
            client_buf[j] = rand() % 127 + 1; // get char in range [1, 127], no early terminator 0
        client_buf[j] = 0;

        // send data from client to server
        client.sock_send(trial_data_len + 1, client_buf);

        // server receives data
        strcpy_s(server_buf, trial_data_len + 1, server.sock_receive(server.get_client_sock(0)));
        if (strcmp(server_buf, client_buf))
            return 1;
        // server echoes data back to client
        server.sock_send(server.get_client_sock(0), trial_data_len + 1, server_buf);

        // client receives data
        strcpy_s(client_buf, trial_data_len + 1, client.sock_receive());
        // verify data equals what was randomly generated; if not, return 1
        if (strcmp(server_buf, client_buf))
            return 1;
   }
   return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("specify which test:\n./test \"connect\" or \n./test \"data_transport\"");
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "connect")
        return test_connect(1); // this currently hardcodes the test to only try 1 client; expand to 4 once that's supported
    else if (arg == "data_transport")
        return test_data_transport();
    printf("specify which test:\n./test \"connect\" or \n./test \"data_transport\"");
    return 1;
}