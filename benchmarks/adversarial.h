#ifndef _ADVERSARIAL_H_
#define _ADVERSARIAL_H_

#include "txn.h" // Must precede thread.h

#include "index_hash.h"
#include "index_btree.h"
#include "index_mbtree.h"
#include "index_mica.h"
#include "index_mica_mbtree.h"
#include "adversarial_const.h"
#include "adversarial_query.h"
#include "mem_alloc.h"
#include "row.h"
#include "thread.h"
#include "wl.h"

class adversarial_wl : public workload {
public:
  RC init();
  RC init_schema(const string schema_file);
  RC init_table();

  RC get_txn_man(txn_man*& txn_manager, thread_t* h_thd);

  HASH_INDEX* the_index;
  table_t* the_table;
};

class adversarial_txn_man : public txn_man {
public:
  void init(thread_t* h_thd, workload* h_wl, uint64_t part_id);
  RC run_txn(base_query* query);

private:
  adversarial_wl* _wl;
};

#endif
