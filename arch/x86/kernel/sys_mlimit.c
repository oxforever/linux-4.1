#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/cred.h>

asmlinkage int sys_set_mlimit(uid_t uid, int mem_max) 
{

    struct user_struct *user;
    if (uid < 0 || mem_max < 0) {
        return -EINVAL;
    }

    user = find_user(uid);
    atomic_set(&user->mem_max, mem_max);
    free_uid(user);

    return 0;
}