#ifndef __MINOS_SCHED_CLASS_H__
#define __MINOS_SCHED_CLASS_H__

struct vcpu;
struct pcpu;

enum {
	SCHED_CLASS_FIFO = 0,
	MAX_SCHED_CLASS,
};

struct sched_class {
	int32_t name;
	unsigned long flags;
	unsigned long sched_interval;
	struct vcpu *(*pick_vcpu)(struct pcpu *);
	void (*set_vcpu_state)(struct pcpu *, struct vcpu *, int);
	int (*add_vcpu)(struct pcpu *, struct vcpu *);
	int (*remove_vcpu)(struct pcpu *, struct vcpu *);
	int (*init_pcpu_data)(struct pcpu *);
	void (*deinit_pcpu_data)(struct pcpu *);
	int (*init_vcpu_data)(struct pcpu *, struct vcpu *k);
	int (*reset_vcpu_data)(struct pcpu *, struct vcpu *k);
	void (*deinit_vcpu_data)(struct pcpu *, struct vcpu *k);
	void (*sched)(struct pcpu *, struct vcpu *, struct vcpu *);
	int (*sched_vcpu)(struct pcpu *, struct vcpu *);
	unsigned long (*tick_handler)(struct pcpu *);
	int (*can_idle)(struct pcpu *);
};

struct sched_class *get_sched_class(int32_t cls);
int register_sched_class(struct sched_class *cls);

#endif
