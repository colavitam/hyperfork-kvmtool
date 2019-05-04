#ifndef KVM__UTIL_INIT_H
#define KVM__UTIL_INIT_H

struct kvm;
struct pre_copy_context;

struct init_item {
	struct hlist_node n;
	const char *fn_name;
	int (*init)(struct kvm *);
};

struct copy_item {
	struct hlist_node n;
	const char *fn_name;
	int (*copy)(struct kvm *, struct pre_copy_context *ctxt);
};


int init_list__init(struct kvm *kvm);
int init_list__exit(struct kvm *kvm);
int init_list__pre_copy(struct kvm *kvm, struct pre_copy_context *ctxt);
int init_list__post_copy(struct kvm *kvm, struct pre_copy_context *ctxt);

int init_list_add(struct init_item *t, int (*init)(struct kvm *),
			int priority, const char *name);
int exit_list_add(struct init_item *t, int (*init)(struct kvm *),
			int priority, const char *name);
int pre_copy_list_add(struct copy_item *t, int (*copy)(struct kvm *, struct pre_copy_context *ctxt),
			int priority, const char *name);
int post_copy_list_add(struct copy_item *t, int (*copy)(struct kvm *, struct pre_copy_context *ctxt),
			int priority, const char *name);

#define __init_list_add(cb, l)						\
static void __attribute__ ((constructor)) __init__##cb(void)		\
{									\
	static char name[] = #cb;					\
	static struct init_item t;					\
	init_list_add(&t, cb, l, name);					\
}

#define __exit_list_add(cb, l)						\
static void __attribute__ ((constructor)) __init__##cb(void)		\
{									\
	static char name[] = #cb;					\
	static struct init_item t;					\
	exit_list_add(&t, cb, l, name);					\
}

#define __pre_copy_list_add(cb, l)						\
static void __attribute__ ((constructor)) __init__##cb(void)		\
{									\
	static char name[] = #cb;					\
	static struct copy_item t;					\
	pre_copy_list_add(&t, cb, l, name);					\
}

#define __post_copy_list_add(cb, l)						\
static void __attribute__ ((constructor)) __init__##cb(void)		\
{									\
	static char name[] = #cb;					\
	static struct copy_item t;					\
	post_copy_list_add(&t, cb, l, name);					\
}

#define core_init(cb) __init_list_add(cb, 0)
#define base_init(cb) __init_list_add(cb, 2)
#define dev_base_init(cb)  __init_list_add(cb, 4)
#define dev_init(cb) __init_list_add(cb, 5)
#define virtio_dev_init(cb) __init_list_add(cb, 6)
#define firmware_init(cb) __init_list_add(cb, 7)
#define late_init(cb) __init_list_add(cb, 9)

#define core_exit(cb) __exit_list_add(cb, 0)
#define base_exit(cb) __exit_list_add(cb, 2)
#define dev_base_exit(cb) __exit_list_add(cb, 4)
#define dev_exit(cb) __exit_list_add(cb, 5)
#define virtio_dev_exit(cb) __exit_list_add(cb, 6)
#define firmware_exit(cb) __exit_list_add(cb, 7)
#define late_exit(cb) __exit_list_add(cb, 9)

#define core_pre_copy(cb) __pre_copy_list_add(cb, 0)
#define base_pre_copy(cb) __pre_copy_list_add(cb, 2)
#define dev_base_pre_copy(cb) __pre_copy_list_add(cb, 4)
#define dev_pre_copy(cb) __pre_copy_list_add(cb, 5)
#define virtio_dev_pre_copy(cb) __pre_copy_list_add(cb, 6)
#define firmware_pre_copy(cb) __pre_copy_list_add(cb, 7)
#define late_pre_copy(cb) __pre_copy_list_add(cb, 9)

#define core_post_copy(cb) __post_copy_list_add(cb, 0)
#define base_post_copy(cb) __post_copy_list_add(cb, 2)
#define dev_base_post_copy(cb) __post_copy_list_add(cb, 4)
#define dev_post_copy(cb) __post_copy_list_add(cb, 5)
#define virtio_dev_post_copy(cb) __post_copy_list_add(cb, 6)
#define firmware_post_copy(cb) __post_copy_list_add(cb, 7)
#define late_post_copy(cb) __post_copy_list_add(cb, 9)
#endif
