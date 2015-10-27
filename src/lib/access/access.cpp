#include "access.h"
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <algorithm>
#define CACHE_SIZE 10


typedef struct Info {
 void * data;
 int count;
} Info;

unordered_map<string, char *> cache;

string makeKeyStr(int file_id, int stripe_id, int chunk_num) {
  char temp[30];
  sprintf(temp, "%8d;%8d;%8d", file_id, stripe_id, chunk_num);
  return std::string(temp);
}

/*void printHead(char *str, int count) {
  for (int i = 0; i < count; i++) {
   std::cout << std::hex << (int)str[i];
  }
  std::cout << std::endl;
}*/

ssize_t process_read_chunk (uint32_t request_id, int fd, int file_id,
                            int node_id, int stripe_id, int chunk_num,
                            int offset, void* buf, int count) {
 std::cout << "offset: " << offset << std::endl;

  string key = makeKeyStr(file_id, stripe_id, chunk_num);
  std::cout << "KEY: " << key << std::endl;

  if (cache.count(key)) {
    std::cout << "FOUND " << std::endl;

    char* base = &((cache[key])[offset]);

    ReadChunkResponse readResponse(request_id, fd, file_id, stripe_id, chunk_num, offset, count, (uint8_t*)base);

     read_response_handler(&readResponse);

     /*memcpy(buf, cache[key][offset], count*sizeof(uint8_t));*/
     std::cout << "Retrieved from cache!" << std::endl;
     std::cout << base << std::endl;

     return 1;
  }

  return network_read_chunk (request_id, fd, file_id, node_id, stripe_id,
                             chunk_num, offset, count);
}

ssize_t process_write_chunk (uint32_t request_id, int fd, int file_id,
                             int node_id, int stripe_id, int chunk_num,
                             int offset, void *buf, int count) {

/*
  if (cache.size() >= CACHE_SIZE) {
    std::cout << "REMOVING " << cache.begin()->first << std::endl;
    //free((void*)((cache.begin()->first).data));
    cache.erase(cache.begin());
  }*/
  string key = makeKeyStr(file_id, stripe_id, chunk_num);
  if (!cache.count(key)) {

    cache[key] = (char*) malloc(MAX_CHUNK);
  }

  char* base = &((cache[key])[offset]);
  memcpy(base, buf, count);

  return network_write_chunk (request_id, fd, file_id, node_id, stripe_id,
                              chunk_num, offset, buf, count);
}


ssize_t process_delete_chunk (uint32_t request_id, int file_id, int node_id,
                              int stripe_id, int chunk_num) {
  /*std::string key = makeKeyStr(file_id, stripe_id, chunk_num);
  auto it = cache.find(key);
  if (it != cache.end()) {
    //free((it->first).data);
    cache.erase(it);
  }
*/
  return network_delete_chunk (request_id, file_id, node_id, stripe_id,
                               chunk_num);
}
