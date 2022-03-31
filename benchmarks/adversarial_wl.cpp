#include "adversarial.h"
#include "table.h"

RC adversarial_wl::init() {
  workload::init();
  string path = "./benchmarks/ADVERSARIAL_schema.txt";
  init_schema(path);
  init_table();

  return RCOK;
}

RC adversarial_wl::init_schema(const string schema_file) {
  workload::init_schema(schema_file);
  the_table = tables["MAIN_TABLE"];
  the_index = hash_indexes["HASH_MAIN_INDEX"];

  return RCOK;
}

RC adversarial_wl::init_table() {
#if CC_ALG == MICA
  ::mica::util::lcore.pin_thread(0);
  mica_db->activate(0);
#endif

  row_t* row;
#if CC_ALG == MICA
  row_t row_container;
  row = &row_container;
#endif

  // Insert key zero
  uint64_t row_id;
  int part_id = 0;
  uint64_t k = 0;
  the_table->get_new_row(row, part_id, row_id);
  row->set_primary_key(k);
  row->set_value(static_cast<int>(AdversarialMainTable::VALUE), k);

  index_insert(the_index, k, row, part_id);

#if CC_ALG == MICA
  mica_db->deactivate(0);
#endif

  return RCOK;
}

RC adversarial_wl::get_txn_man(txn_man *& txn_manager, thread_t * h_thd) {
	txn_manager = (adversarial_txn_man*)mem_allocator.alloc(sizeof(adversarial_txn_man), h_thd->get_thd_id());
	new(txn_manager) adversarial_txn_man();
  txn_manager->init(h_thd, this, h_thd->get_thd_id());

  return RCOK;
}

