#ifndef __ESPRESSO_CORE_H__
#define __ESPRESSO_CORE_H__

#include <stdio.h>

#include "network_core/network.h"
#include "network_core/esp_svc.h"
#include "espresso_storage/espresso_storage.h"

#define NUM_EXPECTED_ARGS 3
#define NAME_ID 1
#define METADATA_FILE 2

void handle_cmd_args(int argc, char** argv);
void load_persistent_metadata(char* metadata_path);

/*
 *	Read data from a chunk at a specific offset.
 */
ssize_t read_data (int fd, int file_id, int stripe_id, int chunk_num, int offset, void *buf, int count);

/*
 *	Write data to a chunk at a specific offset.
 */
ssize_t write_data (int fd, int file_id, int stripe_id, int chunk_num, int offset, void *buf, int count);

/*
 *	Delete a chunk with given file_id and stripe_id.
 */
int delete_data (int fd, int file_id, int stripe_id, int chunk_num);

#endif
