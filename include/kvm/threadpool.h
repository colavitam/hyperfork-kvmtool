#ifndef KVM__THREADPOOL_H
#define KVM__THREADPOOL_H

#include "kvm/mutex.h"

#include <linux/list.h>

struct kvm;
struct pre_copy_context;

typedef void (*kvm_thread_callback_fn_t)(struct kvm *kvm, void *data);

struct thread_pool__job {
	kvm_thread_callback_fn_t	callback;
	struct kvm			*kvm;
	void				*data;

	int				signalcount;
	struct mutex			mutex;

	struct list_head		queue;
};

static inline void thread_pool__init_job(struct thread_pool__job *job, struct kvm *kvm, kvm_thread_callback_fn_t callback, void *data)
{
	*job = (struct thread_pool__job) {
		.kvm		= kvm,
		.callback	= callback,
		.data		= data,
		.mutex		= MUTEX_INITIALIZER,
	};
	INIT_LIST_HEAD(&job->queue);
}

int thread_pool__init(struct kvm *kvm);
int thread_pool__pre_copy(struct kvm *kvm, struct pre_copy_context *ctxt);
int thread_pool__post_copy(struct kvm *kvm, struct pre_copy_context *ctxt);
int thread_pool__post_copy_parent(struct kvm *kvm, struct pre_copy_context *ctxt);
int thread_pool__exit(struct kvm *kvm);

void thread_pool__do_job(struct thread_pool__job *job);
void thread_pool__cancel_job(struct thread_pool__job *job);

#endif
