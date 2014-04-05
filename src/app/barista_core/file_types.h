#ifndef __FILE_TYPES_H__
#define __FILE_TYPES_H__

#include <stdint.h>

#include "limits.h"

/*
 * Stores information about a specific instance of an open file in
 * DecaFS.
 */
struct file_instance {
  uint32_t client_id;
  uint32_t process_id;
  uint32_t file_id;
  uint32_t fd_number;

  bool operator ==(const file_instance & other) const {
    return (this->client_id == other.client_id &&
            this->process_id == other.process_id &&
            this->file_id == other.file_id &&
            this->fd_number == other.fd_number);
  }
  
  bool operator <(const file_instance &other) const {
    return ((this->client_id < other.client_id) ? true :
               (this->process_id < other.process_id) ? true :
                  (this->file_id < other.file_id) ? true :
                     (this->fd_number < other.fd_number) ? 
                        true : false);
  }
};

/*
 * Distinctly idenfies a chunk of a file.
 */
struct file_chunk {
  uint32_t file_id;
  uint32_t stripe_id;
  uint32_t chunk_num;

  bool operator ==(const file_chunk & other) const {
    return (this->file_id == other.file_id &&
            this->stripe_id == other.stripe_id &&
            this->chunk_num == other.chunk_num);
  }
  
  bool operator <(const file_chunk &other) const {
    return ((this->file_id < other.file_id) ? true :
               (this->stripe_id < other.stripe_id) ? true :
                   (this->chunk_num < other.chunk_num) ? 
                      true : false);
  }
};

/*
 *  Storage Information about one file in DecaFS.
 */
struct decafs_file_stat {
  uint32_t file_id; /* DecaFS file id for the file. */
  uint32_t size; /* Size of the file in bytes */
  uint32_t stripe_size; 
  uint32_t chunk_size;
  uint32_t replica_size;
  uint32_t num_stripes;  /* Number of stripes in the file */
  uint32_t nodes[MAX_CHUNK]; /* Where each stripe is stored */
  uint32_t num_replica_chunks; /* Number of replica chunks in the file. */
  uint32_t replica_chunk_nodes[MAX_CHUNK]; /* Where each replica is stored. */
  struct timeval last_access_time;
};


#endif