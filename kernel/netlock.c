#include <linux/syscalls.h>
#include <linux/sched.h>

/*
    enum __netlock_t {
    NET_LOCK_N,  Placeholder for no lock 
    NET_LOCK_R,  Indicates regular lock 
    NET_LOCK_E,  Indicates exclusive lock 
    } ;
    typedef enum __netlock_t netlock_t;
*/

asmlinkage int netlock_acquire(netlock_t type)
{
      
    return 0;
}

asmlinkage int netlock_release(void)
{

    return 0;
}


