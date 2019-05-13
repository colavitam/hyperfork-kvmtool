#ifndef KVM__8250_SERIAL_H
#define KVM__8250_SERIAL_H

struct kvm;
struct pre_copy_context;

int serial8250__init(struct kvm *kvm);
int serial8250__exit(struct kvm *kvm);
void serial8250__update_consoles(struct kvm *kvm);
void serial8250__inject_sysrq(struct kvm *kvm, char sysrq);
int serial8250__pre_copy(struct kvm *kvm, struct pre_copy_context *ctxt);
int serial8250__post_copy(struct kvm *kvm, struct pre_copy_context *ctxt);

#endif /* KVM__8250_SERIAL_H */
