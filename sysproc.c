#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int
set_priority(void){
  int pid, priority;

  if(argint(0, &pid) < 0){
    return -1; 
  }
  if(argint(1, &priority) < 0){
    return -1; 
  }

  if (priority < 0) {
    priority = 0;
  }
  if (priority > 39) {
    priority = 39;
  }

  struct proc *p;
  acquire(&ptable.lock);
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if (p -> pid == pid){
      p->nice = priority;
      release(&ptable.lock);
      return 0;
    }
  }
  return -1;
}

int get_priority(void)
{
  int pid;
  if(argint(0, &pid) < 0){
    return -1; 
  }
  struct proc *p;
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if (p->pid == pid) {
      int nice = p->nice;
      release(&ptable.lock);
      return nice;
    }
  }
  release(&ptable.lock);
  return -1; //not found
}

// current process status
int 
cps()
{
	struct proc *p;
	
	// Enable interrupts on this processor
	sti();
	
	// Loop over process table looking for process with pid
	acquire(&ptable.lock);
	cprintf("name \t pid \t state \t priority \n");
	for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
		if(p->state == SLEEPING)
			cprintf("%s \t %d \t SLEEEPING \t %d \n", p->name, p->pid, p->nice);
		else if(p->state == RUNNING)
			cprintf("%s \t %d \t RUNNING \t %d \n", p->name, p->pid, p->nice);
		else if(p->state == RUNNABLE)
			cprintf("%s \t %d \t RUNNABLE \t %d \n", p->name, p->pid, p->nice);
	}
	
	release(&ptable.lock);
	
	return 22;
}