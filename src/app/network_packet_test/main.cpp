#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"
#include "decafs_types/file_types.h"

#include <stdio.h>
#include <thread>
#include "network_core/decafs_client.h"


// ----------------------- ESPRESSO STORAGE FAKE FUNCTIONS ------------------

ssize_t read_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {

  return count;
}

ssize_t write_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {

  return count;
}

int delete_chunk(int fd, int file_id, int stripe_id, int chunk_num) {

  return 0;
}

// ----------------------- BARISTA CORE FAKE FUNCTIONS ----------------------
// set_node_down
extern "C" uint32_t set_node_down (uint32_t node_number) {
 return 0;
}

// set_node_up
extern "C" uint32_t set_node_up (uint32_t node_number) {
  return 0;
}

// open_file 
extern "C" void open_file (const char *pathname, int flags, struct client client) {

  send_open_result(client, 1); 
}

extern "C" void open_dir(const char* pathname, struct client client) {

  decafs_dirent* entries = (decafs_dirent*)malloc(3*sizeof(decafs_dirent));
  entries[0].file_id = 36;
  entries[0].d_type = 'f';
  std::string filename = "testtestfileawithalongname";
  memcpy(entries[0].d_name, filename.c_str(), strlen(filename.c_str())+1);

  entries[1].file_id = 97;
  entries[1].d_type = 'f';
  std::string filename2 = "otherfileforlengthtestingofaverylongername";
  memcpy(entries[1].d_name, filename2.c_str(), strlen(filename2.c_str())+1);

  entries[2].file_id = 97;
  entries[2].d_type = 'f';
  std::string filename3 = "otherfileforlengthtestingofaverylongername";
  memcpy(entries[2].d_name, filename3.c_str(), strlen(filename3.c_str())+1);

  decafs_dir* dirents = new decafs_dir(0, 3, entries); 
  send_opendir_result(client, dirents);

  delete dirents;
  free(entries);
}

extern "C" void delete_file(char* pathname, struct client client) {

  send_remove_result(client, 0);
}

extern "C" void make_dir(const char* pathname, mode_t mode, struct client client) {

  send_mkdir_result(client, 0);
}

extern "C" void read_file (int fd, size_t count, struct client client) {

  char fake_buffer[2500];
  memset(fake_buffer, 48, count);

  send_read_result(client, fd, count, fake_buffer);
}

extern "C" void write_file (int fd, const void *buf, size_t count, struct client client) {

  send_write_result(client, fd, count);
}

extern "C" void close_file (int fd, struct client client) {

  send_close_result(client, -1);
}

extern "C" void file_seek(int fd, uint32_t offset, int whence, struct client client) {

  send_seek_result(client, 0);
}

extern "C" void file_storage_stat(const char* filename, struct client client) {

  std::string str("this is a file storage stat response!");
  send_file_storage_stat_result(client, str.c_str());
}

// read_response_handler
extern "C" void read_response_handler (ReadChunkResponse *read_response) {

}

// write_response_handler
extern "C" void write_response_handler (WriteChunkResponse *write_response) {

}

// delete_response_handler
extern "C" void delete_response_handler (DeleteChunkResponse *delete_response) {

}

int main(int argc, char** argv) {

  int port = 12345;
  BaristaServer* barista_server = BaristaServer::init(port);
  std::thread barista_thread(&BaristaServer::run, barista_server);

  EspressoClient espresso("localhost", port, 1);
  std::thread espresso_thread(&EspressoClient::run, &espresso);

  // hostname, port, user_id
  DecafsClient client("localhost", port, 2);
  client.openConnection();

  sleep(1);

  // REMOVE
  std::cout << "------------ REMOVE DECAFS CLIENT TEST -------------" << std::endl;
  int rm_res = client.remove("testfile");
  std::cout << "remove returned: " << rm_res << std::endl;
  sleep(1);

  // MKDIR
  std::cout << "------------ MKDIR DECAFS CLIENT TEST ---------" << std::endl;
  int res = client.mkdir("testfile", 0);
  std::cout << "mkdir returned: " << res << std::endl;
  sleep(1);

  // OPEN
  std::cout << "------------ DECAFS CLIENT OPEN TEST ----------" << std::endl;
  int fd = client.open("testfile", O_RDONLY);
  std::cout << "open returned: " << fd << std::endl;
  sleep(1);

  // WRITE
  std::cout << "------------ DECAFS CLIENT WRITE TEST ----------" << std::endl;
  char testwrite[] = "testing network write.";
  int bytes_written = client.write(fd, testwrite, strlen(testwrite));
  std::cout << "write returned: " << bytes_written << std::endl;
  sleep(1);

  // SEEK
  std::cout << "------------ DECAFS CLIENT LSEEK TEST ----------" << std::endl;
  int seek_result = client.lseek(fd, 0, SEEK_SET);
  std::cout << "lseek returned: " << seek_result << std::endl;  
  sleep(1);

  // READ
  std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
  char testread[100];
  int bytes_read = client.read(fd, testread, 100);
  std::cout << "read returned: " << bytes_read << std::endl;
  sleep(1);

  // CLOSE
  std::cout << "------------ DECAFS CLIENT CLOSE TEST ----------" << std::endl;
  int close = client.close(fd);
  std::cout << "close returned: " << close << std::endl;
  sleep(1);

  // OPENDIR
  std::cout << "------------- DECAFS OPENDIR TEST ------------" << std::endl;
  //decafs_dir* dirp = client.opendir(".");
  client.opendir(".");
  sleep(1);

  //FILE_STORAGE_STAT
  std::cout << "------------- DECAFS FILE STORAGE STAT TEST ----------------" << std::endl;
  FileStorageStatResponsePacket* fssrp = client.file_storage_stat("testfile");
  std::cout << *fssrp << std::endl;
  delete fssrp;

  barista_server->close();
  BaristaServer::shutdown();

  barista_thread.join();
  espresso_thread.join();

  return 0;
}
