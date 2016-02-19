#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/syscalls.h>

asmlinkage long sys_fail(int sys_fail_num)
{
	if (sys_fail_num > 0)
	{
		/*refresh the fault injection session if already running*/
		current->sys_fail_num = sys_fail_num;
		current->syscall_cnt = sys_fail_num;
	}
	else if (sys_fail_num == 0)
	{
		/* stop the fault injection session if already running */
		if (current->sys_fail_num != 0)
		{
			current->sys_fail_num = 0;
			current->syscall_cnt = 0;
		}
		else
		{
			return EINVAL;
		}
	}
	else /*inject fault for sys_fail_num < 0*/
	{
		return EINVAL;
	}

	return 0;
}

long __should_fail(void)
{
	current->syscall_cnt--;
	if (current->syscall_cnt == 0 && current->sys_fail_num != 0)
	{
		current->sys_fail_num = 0;
		current->syscall_cnt = 0;
		return EPERM;
	}
	return 0;
}

long __fail_syscall(void)
{
	return -EPERM;
}
