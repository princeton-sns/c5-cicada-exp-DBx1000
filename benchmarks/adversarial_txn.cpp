#include "adversarial.h"
#include "query.h"
#include "wl.h"
#include "thread.h"
#include "table.h"
#include "row.h"
#include "index_hash.h"
#include "index_btree.h"
#include "index_mica.h"
#include "index_mbtree.h"
#include "tpcc_const.h"
#include "mem_alloc.h"

void adversarial_txn_man::init(thread_t * h_thd, workload * h_wl, uint64_t thd_id) {
  txn_man::init(h_thd, h_wl, thd_id);
  _wl = (adversarial_wl*)h_wl;
}

RC adversarial_txn_man::run_txn(base_query* query) {
  adversarial_query* a_query = (adversarial_query*)query;
  auto index = _wl->the_index;
  auto table = _wl->the_table;
  auto part_id = 0;

#if CC_ALG == MICA
  mica_tx->begin(false);
#endif

#if CC_ALG != MICA
  row_t* row = NULL;
#else
  row_t row_stub;
  auto row = &row_stub;
#endif

  for (uint64_t i = 0; i < a_query->kv_cnt; i++) {
    adversarial_request* kv = &a_query->kvs[i];
    uint64_t key = kv->key;
    uint64_t val = kv->val;

    uint64_t row_id;
    if (!insert_row(table, row, part_id, row_id)) return finish(Abort);
    row->set_primary_key(key);
    row->set_value(static_cast<int>(AdversarialMainTable::VALUE), val);

    // TODO: Add option for index
    // if (index->index_insert(this, key, row, part_id) != RCOK) return finish(Abort);
  }

  // Update key zero
  row = search(index, 0, part_id, WR);
  if (row == nullptr) return finish(Abort);
  row->set_value(static_cast<int>(AdversarialMainTable::VALUE), a_query->val0);

  return finish(RCOK);
}
