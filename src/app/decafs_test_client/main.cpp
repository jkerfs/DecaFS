#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"
#include "network_core/decafs_client.h"

#include <stdio.h>
#include <thread>
#include "network_core/barista_network_helper.h"

int main(int argc, char** argv) {
  int port = 3899, fd, bytes_read, bytes_written;
  char testread[1000];
  char filename[] = "testfile";
  DecafsClient client(argv[1], port, 2);
  client.openConnection();
  sleep(1);

  // Write the File to the Distributed File System
  fd = client.open(filename, O_RDWR);
  sleep(1);
  std::cout << "------------ DECAFS CLIENT WRITE TEST ----------" << std::endl;
  char testwrite1[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100";
  bytes_written = client.write(fd, testwrite1, strlen(testwrite1));
  std::cout << "write returned: " << bytes_written << std::endl;
  sleep(1);
  client.close(fd);
  sleep(1);

  // Display the File Metadata
  fd = client.open(filename, O_RDWR);
  sleep(1);
  std::cout << "------------ DECAFS CLIENT FILE STORAGE STAT TEST ----------" << std::endl;
  client.file_storage_stat(filename);
  sleep(1);
  client.close(fd);
  sleep(1);

  // Wait for the Node to be Killed
  std::cout << "Kill the Node!" << std::endl;
  getchar();

  // Update the file
  fd = client.open(filename, O_RDWR);
  sleep(1);
  std::cout << "------------ DECAFS CLIENT WRITE TEST ----------" << std::endl;
  char testwrite2[] = "100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1";
  bytes_written = client.write(fd, testwrite2, strlen(testwrite2));
  std::cout << "write returned: " << bytes_written << std::endl;
  sleep(1);
  client.close(fd);
  sleep(1);

  // Read the file from the replica node
  fd = client.open(filename, O_RDWR);
  sleep(1);
  std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
  bytes_read = client.read(fd, testread, strlen(testwrite2));
  std::cout << "read returned: " << bytes_read << std::endl;
  sleep(1);
  client.close(fd);
  sleep(1);

  // Wait for the Node to be Restarted
  std::cout << "Reboot the Node!" << std::endl;
  getchar();

  // Read the file from the original node
  fd = client.open(filename, O_RDWR);
  sleep(1);
  std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
  bytes_read = client.read(fd, testread, strlen(testwrite2));
  std::cout << "read returned: " << bytes_read << std::endl;
  sleep(1);
  client.close(fd);
  sleep(1);

  return 0;
}
