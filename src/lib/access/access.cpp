#include "access.h"
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <cstdlib>

ssize_t process_read_chunk (uint32_t request_id, int fd, int file_id,
                            int node_id, int stripe_id, int chunk_num,
                            int offset, void* buf, int count) {

  return network_read_chunk (request_id, fd, file_id, node_id, stripe_id,
                             chunk_num, offset, count);
}

ssize_t process_write_chunk (uint32_t request_id, int fd, int file_id,
                             int node_id, int stripe_id, int chunk_num,
                             int offset, void *buf, int count) {

  return network_write_chunk (request_id, fd, file_id, node_id, stripe_id,
                              chunk_num, offset, buf, count);
}


ssize_t process_delete_chunk (uint32_t request_id, int file_id, int node_id,
                              int stripe_id, int chunk_num) {

  return network_delete_chunk (request_id, file_id, node_id, stripe_id, chunk_num);
}
