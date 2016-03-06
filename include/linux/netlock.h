#ifndef __LINUX_NETLOCK_H
#define __LINUX_NETLOCK_H

enum __netlock_t {
   NET_LOCK_N, /* Placeholder for no lock */
   NET_LOCK_R, /* Indicates regular lock */
   NET_LOCK_E, /* Indicates exclusive lock */
};
typedef enum __netlock_t netlock_t;

#endif