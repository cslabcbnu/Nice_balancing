/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_NUMA_BALANCING_H
#define _LINUX_SCHED_NUMA_BALANCING_H

/*
 * This is the interface between the scheduler and the MM that
 * implements memory access pattern based NUMA-balancing:
 */

#include <linux/sched.h>

#define TNF_MIGRATED	0x01
#define TNF_NO_GROUP	0x02
#define TNF_SHARED	0x04
#define TNF_FAULT_LOCAL	0x08
#define TNF_MIGRATE_FAIL 0x10

enum numa_vmaskip_reason {
	NUMAB_SKIP_UNSUITABLE,
	NUMAB_SKIP_SHARED_RO,
	NUMAB_SKIP_INACCESSIBLE,
	NUMAB_SKIP_SCAN_DELAY,
	NUMAB_SKIP_PID_INACTIVE,
	NUMAB_SKIP_IGNORE_PID,
	NUMAB_SKIP_SEQ_COMPLETED,
};

/* [KNICE] 마이그레이션 공격성 단계 정의 */
#define KNICE_LEVEL_NORMAL   0  /* 기본 상태: 깐깐한 기준 */
#define KNICE_LEVEL_BOOST    1  /* DRAM 부족: 기준 완화 시작 */
#define KNICE_LEVEL_URGENT   2  /* 긴급: 매우 공격적인 밀어내기 */

#ifdef CONFIG_NUMA_BALANCING
extern int knice_aggression_level;
extern atomic_long_t knice_migrated_count;
extern void task_numa_fault(int last_node, int node, int pages, int flags);
extern pid_t task_numa_group_id(struct task_struct *p);
extern void set_numabalancing_state(bool enabled);
extern void task_numa_free(struct task_struct *p, bool final);
bool should_numa_migrate_memory(struct task_struct *p, struct folio *folio,
				int src_nid, int dst_cpu);
bool knice_should_demote(struct task_struct *p, struct folio *folio);
#else
static inline void task_numa_fault(int last_node, int node, int pages,
				   int flags)
{
}
static inline pid_t task_numa_group_id(struct task_struct *p)
{
	return 0;
}
static inline void set_numabalancing_state(bool enabled)
{
}
static inline void task_numa_free(struct task_struct *p, bool final)
{
}
static inline bool should_numa_migrate_memory(struct task_struct *p,
				struct folio *folio, int src_nid, int dst_cpu)
{
	return true;
}
static inline bool knice_should_demote(struct task_struct *p, struct folio *folio);
{
	return false;
}
#endif

#endif /* _LINUX_SCHED_NUMA_BALANCING_H */
