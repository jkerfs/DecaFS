#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"
#include "network_core/decafs_client.h"

#include <stdio.h>
#include <thread>
#include "network_core/barista_network_helper.h"

int main(int argc, char** argv) {
  int port = 3899;
  char * filenames[10];
  int j;
  for (j = 0; j < 10; j++) {
    char * temp = (char*)calloc(8, sizeof(char));
    sprintf(temp, "file%d", j);
    filenames[j] = temp;
  }

  DecafsClient client(argv[1] , port, 2);
  client.openConnection();

  int N = 100;
  char* testwrite = (char*)calloc(N, sizeof(char));
  int i;
  for (i = 0; i < N - 1; i++) {
    testwrite[i] = 'a' + i % 26;
  }
  testwrite[i] = '\0';

  sleep(1);

  int fd, bytes_written, bytes_read, close;
  for(j = 0; j < 10; j++) {
    // OPEN
    fd = client.open(filenames[j], O_RDWR);
    sleep(1);

    // WRITE
    std::cout << "------------ DECAFS CLIENT WRITE TEST ----------" << std::endl;
    bytes_written = client.write(fd, testwrite, strlen(testwrite));
    std::cout << "write returned: " << bytes_written << std::endl;
    sleep(1);

    // CLOSE
    close = client.close(fd);
    sleep(1);
  }

  for(j = 0; j < 10; j++) {
    // OPEN
    fd = client.open(filenames[j], O_RDWR);
    sleep(1);

    // READ
    std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
    char * testread = (char *)calloc(N, sizeof(char));
    bytes_read = client.read(fd, testread, strlen(testwrite));
    std::cout << "read returned: " << bytes_read << std::endl;
    std::cout << testread << std::endl;
    sleep(1);

    // CLOSE
    close = client.close(fd);
    sleep(1);
  }

  return 0;
}
