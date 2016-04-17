#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/cred.h>

asmlinkage int sys_set_mlimit(uid_t uid, int mem_max) 
{

    struct user_struct *user;
    if (uid < 0 || mem_max < 0) {
        return -EINVAL;
    }
	kuid_t kuid = make_kuid(current_user_ns(), uid);
    user = find_user(kuid);
    atomic_set(&user->mem_max, mem_max);
    free_uid(user);

    return 0;
}