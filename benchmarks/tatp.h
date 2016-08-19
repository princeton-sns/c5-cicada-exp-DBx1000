#ifndef _TATP_H_
#define _TATP_H_

#include "wl.h"
#include "txn.h"
#include "global.h"
#include "helper.h"

class tatp_query;

class tatp_wl : public workload {
 public:
  RC init();
  RC init_table();
  RC init_schema(string schema_file);
  RC get_txn_man(txn_man*& txn_manager, thread_t* h_thd);
  int key_to_part(uint64_t key);

  table_t* t_subscriber;
  table_t* t_access_info;
  table_t* t_special_facility;
  table_t* t_call_forwarding;

  INDEX* i_subscriber;
  INDEX* i_subscriber_sub_nbr;
  INDEX* i_access_info;
  INDEX* i_special_facility;
  ORDERED_INDEX* i_call_forwarding;
  INDEX* i_call_forwarding_s_id;

 private:
  static void* threadInitTable(void* This);

  void gen_subscriber(uint64_t s_id, uint64_t thd_id);
  void gen_access_info(uint64_t s_id, uint64_t thd_id);
  void gen_spe_and_cal(uint64_t s_id, uint64_t thd_id);

  uint32_t next_tid;

  uint32_t tid_lock[256];
};

class tatp_txn_man : public txn_man {
 public:
  void init(thread_t* h_thd, workload* h_wl, uint64_t thd_id);
  RC run_txn(base_query* query);

 private:
  tatp_wl* _wl;
  RC run_delete_call_forwarding(tatp_query* query);
  RC run_get_access_data(tatp_query* query);
  RC run_get_new_destination(tatp_query* query);
  RC run_get_subscriber_data(tatp_query* query);
  RC run_insert_call_forwarding(tatp_query* query);
  RC run_update_location(tatp_query* query);
  RC run_update_subscriber_data(tatp_query* query);
};

#endif