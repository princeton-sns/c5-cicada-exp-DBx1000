#include "insert.h"

void insert_txn_man::init(thread_t * h_thd, workload * h_wl, uint64_t thd_id) {
  txn_man::init(h_thd, h_wl, thd_id);
  _wl = (insert_wl*)h_wl;
}

RC insert_txn_man::run_txn(base_query* query) {
  insert_query* i_query = (insert_query*)query;

#if CC_ALG == MICA
  mica_tx->begin(false);
#endif

#if CC_ALG != MICA
  row_t* row = NULL;
#else
  row_t row_stub;
  auto row = &row_stub;
#endif

  for (uint64_t i = 0; i < i_query->kv_cnt; i++) {
    kv_request* kv = &i_query->kvs[i];
    uint64_t key = kv->key;
    uint64_t val = kv->val;

    uint64_t row_id;
    auto part_id = 0;
    if (!insert_row(_wl->the_table, row, part_id, row_id)) return finish(Abort);
    row->set_primary_key(key);
    row->set_value(static_cast<int>(InsertMainTable::VALUE), val);

    // TODO: Add option for index
    // if (!insert_idx(_wl->the_index, key, row, part_id)) return finish(Abort);
  }

  return finish(RCOK);
}
