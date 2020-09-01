#include "adversarial_query.h"

void adversarial_query::init(uint64_t thd_id, workload* h_wl, uint64_t query_id) {
  kv_cnt = g_adversarial_inserts_per_txn;
  kvs = (adversarial_request*)mem_allocator.alloc(sizeof(adversarial_request) * g_adversarial_inserts_per_txn, thd_id);

  // To avoid re-inserting key zero
  if (thd_id == 0) {
    query_id++;
  }

  uint64_t k = thd_id * g_adversarial_inserts_per_txn + query_id * g_thread_cnt * g_adversarial_inserts_per_txn;
  val0 = k;
  for (uint64_t i = 0; i < kv_cnt; i++) {
    adversarial_request* kv = &kvs[i];

    kv->key = k;
    kv->val = k;
    k++;
  }
}

