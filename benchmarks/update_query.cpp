#include "update_query.h"

void update_query::init(uint64_t thd_id, workload* h_wl, uint64_t query_id) {
  kv_cnt = g_update_updates_per_txn;
  kvs = (update_request*)mem_allocator.alloc(sizeof(update_request) * g_update_updates_per_txn, thd_id);

  uint64_t k = (thd_id * g_update_updates_per_txn + query_id * g_thread_cnt * g_update_updates_per_txn) % g_synth_table_size;
  for (uint64_t i = 0; i < kv_cnt; i++) {
    update_request* kv = &kvs[i];

    kv->key = k;
    kv->val = k;
    k = (k+1) % g_synth_table_size;
  }
}

