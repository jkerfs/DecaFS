
#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"
#include "network_core/decafs_client.h"

#include <stdio.h>
#include <thread>
#include "network_core/barista_network_helper.h"

//Helper Functions
int openDF(DecafsClient * client, char * filename) {
  int fd;

  std::cout << "------------ DECAFS CLIENT OPEN TEST ----------" << std::endl;
  fd = client->open(filename, O_RDWR);
  std::cout << "open returned: " << fd << std::endl;

  sleep(1);

  return fd;
}

int writeDF(DecafsClient * client, int fd, char * message) {
  int bytes_written;

  std::cout << "------------ DECAFS CLIENT WRITE (0) TEST ----------" << std::endl;
  bytes_written = client->write(fd, message, 0);
  std::cout << "write returned: " << bytes_written << std::endl;
  sleep(1);

  return bytes_written;
}

int readDF(DecafsClient * client, int fd, char * readbuf, int len) {
  int bytes_read;

  std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
  bytes_read = client->read(fd, readbuf, len);
  std::cout << "read returned: " << bytes_read << std::endl;
  sleep(1);

  return bytes_read;
}

void closeDF(DecafsClient * client, int fd) {
  std::cout << "------------ DECAFS CLIENT CLOSE TEST ----------" << std::endl;
  int close = client->close(fd);
  std::cout << "close returned: " << close << std::endl;
  sleep(1);
}

void statDF(DecafsClient * client, char * filename) {
  std::cout << "------------ DECAFS CLIENT FILE STORAGE STAT TEST ----------" << std::endl;
  client->file_storage_stat(filename);
  sleep(1);
}

void seekDF(DecafsClient * client, int fd) {
  std::cout << "------------ DECAFS CLIENT SEEK TEST ----------" << std::endl;
  int offset = client->lseek(fd, 7, SEEK_SET);
  std::cout << "seek returned: " << offset << std::endl;
  sleep(1);
}




int main(int argc, char** argv) {
  int port = 3899;
  char filename[20];
  char message[1000];
  char testread[1000];
  DecafsClient client(argv[1], port, 2);
  client.openConnection();
  int fd[10];

  sleep(1);

  //start tests here

  //open and write 10 test files
  for (int i=0; i<10; i++) {
    sprintf(filename, "testfile%2d\0", i);
    fd[i] = openDF(&client, filename);
    sprintf(message, "test numero %2d\n\ncomplete.\0", i);
    writeDF(&client, fd[i], message);
    closeDF(&client, fd[i]);
  }

  //read from the 10 test files
  for (int i=0; i<10; i++) {
    sprintf(filename, "testfile%2d\0", i);
    fd[i] = openDF(&client, filename);
    readDF(&client, fd[i], testread, strlen(filename));
    printf("%s\n", testread);
    closeDF(&client, fd[i]);
  }

  //kill a few nodes
  //TODO: kill some nodes

  //read again
  for (int i=0; i<10; i++) {
    sprintf(filename, "testfile%2d\0", i);
    fd[i] = openDF(&client, filename);
    readDF(&client, fd[i], testread, strlen(filename));
    printf("%s\n", testread);
    closeDF(&client, fd[i]);
  }

  //write again
  for (int i=0; i<10; i++) {
    sprintf(filename, "testfile%2d\0", i);
    fd[i] = openDF(&client, filename);
    sprintf(message, "writing again %2d\n\ncomplete.\0", i);
    writeDF(&client, fd[i], message);
    closeDF(&client, fd[i]);
  }

  //read final
  for (int i=0; i<10; i++) {
    sprintf(filename, "testfile%2d\0", i);
    fd[i] = openDF(&client, filename);
    readDF(&client, fd[i], testread, strlen(filename));
    printf("%s\n", testread);
    closeDF(&client, fd[i]);
  }

  printf("Tests complete.\n");

  return 0;
}

  /*
  // DELETE
  std::cout << "------------ DECAFS CLIENT DELETE TEST ----------" << std::endl;
  client.remove_file(filename);
  sleep(1);

  // FILE STORAGE STAT
  std::cout << "------------ DECAFS CLIENT FILE STORAGE STAT TEST ----------" << std::endl;
  client.file_storage_stat(filename);
  sleep(1);
  */
