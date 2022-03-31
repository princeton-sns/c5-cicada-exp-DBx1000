#ifndef _UPDATE_QUERY_H_
#define _UPDATE_QUERY_H_

#include "mem_alloc.h"
#include "query.h"

class update_request {
public:
  uint64_t key;
  uint64_t val;
};

class update_query : public base_query {
public:
  void init(uint64_t thd_id, workload* h_wl) { assert(false); };
  void init(uint64_t thd_id, workload* h_wl, uint64_t query_id);

  uint64_t kv_cnt;
  update_request* kvs;
};

#endif
