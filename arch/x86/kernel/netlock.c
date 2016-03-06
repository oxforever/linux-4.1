#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/netlock.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
/*
    enum __netlock_t {
    NET_LOCK_N,  Placeholder for no lock 
    NET_LOCK_R,  Indicates regular lock 
    NET_LOCK_E,  Indicates exclusive lock 
    } ;
    typedef enum __netlock_t netlock_t;
*/

static DEFINE_SPINLOCK(s_lock);
static DEFINE_SPINLOCK(guard);
DECLARE_WAIT_QUEUE_HEAD(netlock_wait_queue);
//	wake_up(&cryptocop_ioc_process_wq);
//	wait_event(cryptocop_ioc_process_wq, (jc->processed != 0));

unsigned long flags;
int nreader = 0;
int nwriter = 0;

asmlinkage int netlock_acquire(netlock_t type)
{
	if (type == NET_LOCK_E)	/*Write lock, success if no reader, enqueue if other writer*/
	{
		if (nwriter > 0 || nreader > 0) /*Enqueue*/
		{
			/*Wait until no other lock is present*/
			wait_event(netlock_wait_queue, (nwriter == 0 ||nreader == 0));
		}
		else
		{
			spin_lock_irqsave(&guard, flags);
			++nwriter;
			spin_lock_irqsave(&s_lock, flags);
			spin_unlock_irqrestore(&guard, flags);			
		}
	}
	else if (type == NET_LOCK_R)	/*Read lock, allow concurrent access*/
	{
		if (nwriter > 0)
		{
			/*Wait until no writer lock is present*/
			wait_event(netlock_wait_queue, (nwriter > 0));
		}
		else
		{
			spin_lock_irqsave(&guard, flags);
			++nreader;
			if(nreader == 1)	/*First reader*/
				spin_lock_irqsave(&s_lock, flags);
			spin_unlock_irqrestore(&guard, flags);			
		}	
	}

    return 0;
}

asmlinkage int netlock_release(void)
{
	if (nwriter > 0)
	{
		spin_lock_irqsave(&guard, flags);
		--nwriter;
		spin_unlock_irqrestore(&s_lock, flags);			
		spin_unlock_irqrestore(&guard, flags);			
	}
	else if (nreader > 0)
	{
		spin_lock_irqsave(&guard, flags);
		--nreader;
		spin_unlock_irqrestore(&s_lock, flags);			
		spin_unlock_irqrestore(&guard, flags);	
	}

    return 0;
}


