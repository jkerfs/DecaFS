#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"

#include <stdio.h>
#include <thread>
#include "network_core/barista_network_helper.h"

int main(int argc, char** argv) {

  // init barista core information
  barista_core_init (argc, argv);
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n\n", get_stripe_size(),
           get_chunk_size());

  int port = 1034;

  // initialize the barista server
  BaristaServer* barista_server = BaristaServer::init(port);
  std::thread barista_thread(&BaristaServer::run, barista_server); 

  // create an espresso node
  EspressoClient espresso("localhost", port, 1);
  espresso.setName("espresso_1");
  
  EspressoClient espresso2("localhost", port, 2);
  espresso2.setName("espresso_2");

  EspressoClient espresso3("localhost", port, 3);
  espresso3.setName("espresso_3");

  EspressoClient espresso4("localhost", port, 4);
  espresso4.setName("espresso_4");


  sleep(1);

  espresso.openConnection();
  espresso2.openConnection();
  espresso3.openConnection();
  espresso4.openConnection();
  std::thread espresso_thread(&EspressoClient::run, &espresso);
  std::thread espresso_thread2(&EspressoClient::run, &espresso2);
  std::thread espresso_thread3(&EspressoClient::run, &espresso3);
  std::thread espresso_thread4(&EspressoClient::run, &espresso4);

  sleep(1);

  //ConnectionToClient *client = barista_server->getEspressoNode();
  //printf("client is: %p\n", (void*)client);

  int sent, i;

  // call open read write ...
  struct ip_address ip;
  ConnectionToClient *ctc = new ConnectionToClient();
  struct client default_client = {ip, 1, ctc};
  int count;
  int fd[7];
  char buf[] = "1";
  char *read_buf;
  char file_name[20] = {0};

  //generate 7 test files
  for (i=0; i<7; i++) {
    sprintf(file_name, "test%2d.txt\0", i);
    count, fd[i] = open_file (file_name, O_RDWR, default_client);
    sprintf(buf, "test numero %2d\n\ncomplete.\0", i);

    read_buf = (char *)malloc (1000);
    memset (read_buf, '\0', 1000);
    printf ("\n\n ------------------------------WRITE #%2d------------------------------\n", i);
    count = write_file (fd[i], buf, strlen (buf), default_client);
    printf ("(\n(BARISTA) wrote %d bytes.\n", count);
    close_file (fd[i], default_client);
  }

  for (i=0; i<7; i++) {
    sent = network_read_chunk(client, 1, 2, 3, 4, 5, 6);
    printf("read sent: %d bytes\n", sent);
    
    read_file(fd[i], count, default_client);
    //TODO: check to see if read_file call is implemented correctly
    //TODO: check the data return by the read_file to see if it matches what was written to it
  }

  //TODO: kill some nodes and perform some more read & writes


  /*sent = network_write_chunk(client, 1, 2, 3, 4, 5, (void*)"hello!", 6);
  printf("write sent: %d bytes\n", sent);

  sent = network_read_chunk(client, 1, 2, 3, 4, 5, 6);
  printf("read sent: %d bytes\n", sent);

  sent = network_delete_chunk(client, 1, 2, 3); 
  printf("delete sent: %d bytes\n", sent);
*/
  sleep(1);

  barista_server->close();
  barista_thread.join();

  sleep(1);

  espresso_thread.join();

  return 0;
}
