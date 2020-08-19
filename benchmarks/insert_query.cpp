#include "insert_query.h"

void insert_query::init(uint64_t thd_id, workload* h_wl, uint64_t query_id) {
  kv_cnt = g_inserts_per_txn;
  kvs = (kv_request*)mem_allocator.alloc(sizeof(kv_request) * g_inserts_per_txn, thd_id);

  uint64_t k = thd_id * g_inserts_per_txn + query_id * g_thread_cnt * g_inserts_per_txn;
  for (uint64_t i = 0; i < kv_cnt; i++) {
    kv_request* kv = &kvs[i];

    kv->key = k;
    kv->val = k;
    k++;
  }
}

