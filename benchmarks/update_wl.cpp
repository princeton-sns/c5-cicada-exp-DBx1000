#include "update.h"
#include "table.h"

RC update_wl::init() {
  workload::init();
  string path = "./benchmarks/UPDATE_schema.txt";
  init_schema(path);
  init_table();

  return RCOK;
}

RC update_wl::init_schema(const string schema_file) {
  workload::init_schema(schema_file);
  the_table = tables["MAIN_TABLE"];
  the_index = hash_indexes["HASH_MAIN_INDEX"];

  return RCOK;
}

RC update_wl::init_table() {
#if CC_ALG == MICA
  ::mica::util::lcore.pin_thread(0);
  mica_db->activate(0);
#endif

  row_t* row;
#if CC_ALG == MICA
  row_t row_container;
  row = &row_container;
#endif

  uint64_t row_id;
  int part_id = 0;
  for (uint64_t k = 0; k < g_synth_table_size; k++) {
    the_table->get_new_row(row, part_id, row_id);
    row->set_primary_key(k);
    row->set_value(static_cast<int>(UpdateMainTable::VALUE), k);

    index_insert(the_index, k, row, part_id);
  }

#if CC_ALG == MICA
  mica_db->deactivate(0);
#endif

  return RCOK;
}

RC update_wl::get_txn_man(txn_man *& txn_manager, thread_t * h_thd) {
	txn_manager = (update_txn_man*)mem_allocator.alloc(sizeof(update_txn_man), h_thd->get_thd_id());
	new(txn_manager) update_txn_man();
  txn_manager->init(h_thd, this, h_thd->get_thd_id());

  return RCOK;
}

