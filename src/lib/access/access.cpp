#include "access.h"
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <algorithm>
#define CACHE_SIZE 10
/*
typedef struct Info {
 void * data;
 int count;
} Info;
*/
//unordered_map<string, Info> cache;

string makeKeyStr(int file_id, int chunk_num) {
  char temp[15];
  sprintf(temp, "%5d;%9d", file_id, chunk_num);
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
  string key = makeKeyStr(file_id, chunk_num);
  std::cout << "KEY: " << key << std::endl;
  /*if (cache.count(key) > 0) {
    Info res = cache[key];
    std::cout << "FOUND " << std::endl;
    if (res.count >= count) {
     //printHead(res.data, std::min(10, count));
     memcpy(buf, res.data, count*sizeof(uint8_t));
     std::cout << "Retrieved from cache!" << std::endl;
     return count;
    }
  }*/

  return network_read_chunk (request_id, fd, file_id, node_id, stripe_id,
                             chunk_num, offset, count);
}

ssize_t process_write_chunk (uint32_t request_id, int fd, int file_id,
                             int node_id, int stripe_id, int chunk_num,
                             int offset, void *buf, int count) {


  /*if (cache.size() >= CACHE_SIZE) {
    std::cout << "REMOVING " << cache.begin()->first << std::endl;
    //free((void*)((cache.begin()->first).data));
    cache.erase(cache.begin());
  }
  void * temp = calloc(sizeof(uint8_t), count);
  memcpy(temp, buf, count * sizeof(uint8_t));

  cache[makeKeyStr(file_id, chunk_num)] = Info{temp, count};
*/
  return network_write_chunk (request_id, fd, file_id, node_id, stripe_id,
                              chunk_num, offset, buf, count);
}


ssize_t process_delete_chunk (uint32_t request_id, int file_id, int node_id,
                              int stripe_id, int chunk_num) {
  /*std::string key = makeKeyStr(file_id, chunk_num);
  auto it = cache.find(key);
  if (it != cache.end()) {
    //free((it->first).data);
    cache.erase(it);
  }
*/
  return network_delete_chunk (request_id, file_id, node_id, stripe_id,
                               chunk_num);
}
