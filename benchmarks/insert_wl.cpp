#include "insert.h"

RC insert_wl::init() {
  workload::init();
  string path = "./benchmarks/INSERT_schema.txt";
  init_schema(path);

  return RCOK;
}

RC insert_wl::init_schema(const string schema_file) {
  workload::init_schema(schema_file);
  the_table = tables["MAIN_TABLE"];
  the_index = hash_indexes["HASH_MAIN_INDEX"];

  return RCOK;
}

RC insert_wl::init_table() { return RCOK; }

RC insert_wl::get_txn_man(txn_man *& txn_manager, thread_t * h_thd) {
	txn_manager = (insert_txn_man*)mem_allocator.alloc(sizeof(insert_txn_man), h_thd->get_thd_id());
	new(txn_manager) insert_txn_man();
  txn_manager->init(h_thd, this, h_thd->get_thd_id());

  return RCOK;
}

