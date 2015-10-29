#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"
#include "network_core/decafs_client.h"

#include <stdio.h>
#include <thread>
#include "network_core/barista_network_helper.h"

int main(int argc, char** argv) {
  int port = 3899;
  char filename[] = "testfile";
  DecafsClient client(argv[1] , port, 2);
  client.openConnection();

  int N = 100000;
  char* testwrite = (char*)calloc(N, sizeof(char));
  int i;
  for (i = 0; i < N - 1; i++) {
    testwrite[i] = 'a' + i % 26;
  }
  testwrite[i] = '\0';

  sleep(1);

  // OPEN
  std::cout << "------------ DECAFS CLIENT OPEN TEST ----------" << std::endl;
  int fd = client.open(filename, O_RDWR);
  std::cout << "open returned: " << fd << std::endl;
  sleep(1);

  // WRITE
  std::cout << "------------ DECAFS CLIENT WRITE TEST ----------" << std::endl;
  int bytes_written = client.write(fd, testwrite, strlen(testwrite));
  std::cout << "write returned: " << bytes_written << std::endl;
  sleep(10);

  // CLOSE
  std::cout << "------------ DECAFS CLIENT CLOSE TEST ----------" << std::endl;
  int close = client.close(fd);
  std::cout << "close returned: " << close << std::endl;
  sleep(1);

  // OPEN
  std::cout << "------------ DECAFS CLIENT OPEN TEST ----------" << std::endl;
  fd = client.open(filename, O_RDWR);
  std::cout << "open returned: " << fd << std::endl;
  sleep(1);

  // READ
  std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
  char * testread = (char *)calloc(N, sizeof(char));
  int bytes_read = client.read(fd, testread, strlen(testwrite));
  std::cout << "read returned: " << bytes_read << std::endl;
  std::cout << testread << std::endl;
  sleep(10);

  // CLOSE
  std::cout << "------------ DECAFS CLIENT CLOSE TEST ----------" << std::endl;
  close = client.close(fd);
  std::cout << "close returned: " << close << std::endl;
  sleep(1);

  return 0;
}
