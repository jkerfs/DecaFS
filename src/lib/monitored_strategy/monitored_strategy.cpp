#include "monitored_strategy.h"
#include "monitored_strategy_c_api.h"

std::list<WriteReq> writeRequests;

extern "C" void strategy_startup() {
  // Register custom monitoring here given a specified timeout.
  // register_monitor_module (&monitor, TIMEOUT);

  register_node_failure_handler (&node_failure_handler_func);
  register_node_up_handler (&node_up_handler_func);
}

extern "C" void monitor_func () {
  // Do custom monitoring here. This function will be called every TIMEOUT
  // seconds after registration above.
}

extern "C" void node_failure_handler_func (uint32_t node_number) {
  printf ("Handling node failure...\n");
}

extern "C" void node_up_handler_func (uint32_t node_number) {
  printf ("Handling node coming online...\n");
  auto itW = writeRequests.begin();
  while (itW!=writeRequests.end()) {
    if(itW->node_id == node_number) {
      uint32_t req_id = get_new_request_id();
      replace_request_lookups.insert(std::pair<uint32_t, uint32_t>(req_id, req_id));
      process_write_chunk (req_id, itW->fd, itW->file_id,
                                  itW->node_id, itW->stripe_id, itW->chunk_num,
                                   itW->offset, itW->buf, itW->count);

      printf("Write Request:\n file_id:%d \t node_id:%d\t stripe_id:%d\t chunk_num:%d\n",
        itW->file_id, itW->node_id, itW->stripe_id, itW->chunk_num);
      writeRequests.erase(itW);
    }
    itW++;
  }
}
