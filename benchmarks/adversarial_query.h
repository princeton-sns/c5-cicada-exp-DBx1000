#ifndef _ADVERSARIAL_QUERY_H_
#define _ADVERSARIAL_QUERY_H_

#include "mem_alloc.h"
#include "query.h"

class adversarial_request {
public:
  uint64_t key;
  uint64_t val;
};

class adversarial_query : public base_query {
public:
  void init(uint64_t thd_id, workload* h_wl) { assert(false); };
  void init(uint64_t thd_id, workload* h_wl, uint64_t query_id);

  uint64_t val0;
  uint64_t kv_cnt;
  adversarial_request* kvs;
};

#endif
