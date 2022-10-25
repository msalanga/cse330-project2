#include <linux/semaphore.h>
#include <linux/module.h> 
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/timekeeping.h>

#define AUTHOR "Muhammad Alshanfari"
MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);

static int buffSize, prod, cons, uuid;

module_param(buffSize, int, 0644);
module_param(prod, int, 0644);
module_param(cons, int, 0644);
module_param(uuid, int, 0644);

MODULE_PARAM_DESC(cons, "no. of consumers");
MODULE_PARAM_DESC(prod, "no. of producers");
MODULE_PARAM_DESC(buffSize, "Size of buffer");
MODULE_PARAM_DESC(cons, "UUID of user");

struct task_struct tasks[] = vmalloc(buffSize*sizeof(struct task_struct));
int productionI = 0;  
int consumerI = 0; 

struct semaphore empty, mutex, full; 

static void consumption(void *arg) {
	double total_time = 0;
	down_interruptible(&full);
	for_each_process(struct task_struct *p) {
	  	if (p->cred->uid.val == uuid) { 
	  		printk("[]Consumed Item#%d at buffer index:%d for PID:%d\n",consumerI, consumerI, p->pid);
	  		tasks[consumerI]->pid = p->pid;
	    		tasks[consumerI]->cred->uid.val = p->cred->uid.val;
	    		consumerI = (consumerI + 1) % bufferSize;
	    		down_interruptible(&empty);
	    		up(empty);
	  	} else { continue;} 
	  	if (&empty == 0) { l
	  		up(full);
	  		break;
	  	}
	  }
	  return; 
}

static void production(void *arg) {
	down_interruptible(&mutex);
	if(&empty == 0) { break; }
	for_each_process(struct task_struct *p) {
	  	if (p->cred->uid.val == uuid) { 
	  		printk("[]Produced Item#%d at buffer index:%d for PID:%d\n",productionI, productionI, p->pid);
	  		tasks[productionI]->pid = p->pid;
	    		tasks[productionI]->cred->uid.val = p->cred->uid.val;
	    		productionI = (productionI + 1) % bufferSize;
	    		down_interruptible(&empty);
	    		up(full);
	  	} else { continue;} 
	  	if (&empty == 0) { l
	  		up(mutex);
	  		break;
	  	}
	  }
	  return; 
}


int main() {
  	sema_init(&empty, buffSize);
  	sema_init(&full, 0);
  	sema_init(&mutex, 1);
        int i = 0;
	for(i = 0; i < prod; i++) {
		productionI = kthread_run(production, NULL, "Producer-1");
	}

	for(i = 0; i < cons; i++) {
		productionI = kthread_run(consumption, NULL, "Producer-1");
	}
  			
}

