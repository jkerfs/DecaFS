#include "access.h"
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <algorithm>

#define CACHE_SIZE 10


unordered_map<string, char *> cache;

//convert the file_id, stripe_id, and chunk_num into a unique string
string makeKeyStr(int file_id, int stripe_id, int chunk_num) {
  char temp[30];
  sprintf(temp, "%8d;%8d;%8d", file_id, stripe_id, chunk_num);
  return std::string(temp);
}

ssize_t process_read_chunk (uint32_t request_id, int fd, int file_id,
                            int node_id, int stripe_id, int chunk_num,
                            int offset, void* buf, int count) {

 //compute the key for the given file, stripe and chunk
  string key = makeKeyStr(file_id, stripe_id, chunk_num);
  std::cout << "KEY: " << key << std::endl;

  if (cache.count(key)) {
    std::cout << "FOUND " << std::endl;

    char* base = &((cache[key])[offset]);

    ReadChunkResponse readResponse(request_id, fd, file_id, stripe_id, chunk_num, offset, count, (uint8_t*)base);

     read_response_handler(&readResponse);

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

  // Remove an element from cache if cache is full
  if (cache.size() >= CACHE_SIZE) {
    std::cout << "REMOVING element from Cache" << std::endl;
    free(cache.begin()->second);
    cache.erase(cache.begin());
  }

  //compute the key for the given file, stripe and chunk
  string key = makeKeyStr(file_id, stripe_id, chunk_num);

  // allocate memory to store chunk
  if (!cache.count(key)) {
    cache[key] = (char*) malloc(MAX_CHUNK);
  }

  //copy chunk to cache at specific key and offset
  char* base = &((cache[key])[offset]);
  memcpy(base, buf, count);

  WriteChunkResponse writeResponse(request_id, fd, file_id, stripe_id, chunk_num, offset, count);
  write_response_handler(&writeResponse);

  return network_write_chunk (request_id, fd, file_id, node_id, stripe_id,
                              chunk_num, offset, buf, count);
}


ssize_t process_delete_chunk (uint32_t request_id, int file_id, int node_id,
                              int stripe_id, int chunk_num) {

  //compute the key for the given file, stripe and chunk
  string key = makeKeyStr(file_id, stripe_id, chunk_num);

  //free memory and remove chunk from cache if it exists
  if (cache.count(key)) {
    free(cache.find(key)->second);
    cache.erase(key);
  }

  return network_delete_chunk (request_id, file_id, node_id, stripe_id, chunk_num);
}
