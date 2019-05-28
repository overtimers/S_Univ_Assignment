#include "sched.h"

void init_myrr_rq (struct myrr_rq *myrr_rq)
{
	printk(KERN_INFO "***[MYRR] Myrr class is online \n");
	myrr_rq->nr_running = 0;
	INIT_LIST_HEAD(&myrr_rq->queue);
}

static void enqueue_task_myrr(struct rq *rq, struct task_struct *p, int flags)
{
	struct myrr_rq *myrr_rq = &rq->myrr;
	struct sched_myrr_entity *myrr_entity = &p->myrr;
	list_add_tail(&myrr_entity->run_list, &myrr_rq->queue);
	myrr_rq->nr_running++;
	printk(KERN_INFO "***[MYRR] enqueue: success cpu=%d, nr_running=%d, pid=%d\n",
			cpu_of(rq), myrr_rq->nr_running, p->pid);
}

static void dequeue_task_myrr(struct rq *rq, struct task_struct *p, int flags)
{
	struct myrr_rq *myrr_rq = &rq->myrr;
	struct sched_myrr_entity *myrr_entity = &p->myrr;
	if(myrr_rq->nr_running > 0)
	{
		list_del(&myrr_entity->run_list);
		myrr_rq->nr_running--;
		printk(KERN_INFO "\t***[MYRR] dequeue: success cpu=%d, nr_running=%d, pid=%d\n",
			cpu_of(rq), myrr_rq->nr_running, p->pid);
	}
	else
	{
	}
}
static void check_preempt_curr_myrr(struct rq *rq, struct task_struct *p, int flag) {}
struct task_struct *pick_next_task_myrr(struct rq *rq, struct task_struct *prev)
{
	struct task_struct *next_p = NULL;
	struct sched_myrr_entity *next_se = NULL;
	struct myrr_rq *myrr_rq = &rq->myrr;
	struct list_head *queue = &myrr_rq->queue;

	if(myrr_rq->nr_running == 0){
		return NULL;
	}
	if(prev->sched_class != &myrr_sched_class)
	{
		printk(KERN_INFO "***[MYRR] pick_next_task: other class came in.. prev->pid=%d\n", prev->pid);
		put_prev_task(rq, prev);
	}
	next_se = container_of(queue->next, struct sched_myrr_entity, run_list);
	next_p = container_of(next_se, struct task_struct, myrr);
	printk(KERN_INFO "\t***[MYRR] pick_next_task: cpu=%d, prev->pid=%d, next_p->pid=%d, nr_running=%d\n",
			cpu_of(rq), prev->pid, next_p->pid, myrr_rq->nr_running);
	return next_p;
}
static void put_prev_task_myrr(struct rq *rq, struct task_struct *p) {}
static int select_task_rq_myrr(struct task_struct *p, int cpu, int sd_flag, int flags)
{
	return task_cpu(p);
}
static void set_curr_task_myrr(struct rq *rq) {}
static void prio_changed_myrr(struct rq *rq, struct task_struct *p, int oldprio) {}
static void switched_to_myrr(struct rq *rq, struct task_struct *p)
{
	resched_curr(rq);
}

static void update_curr_myrr(struct rq *rq)
{
	struct myrr_rq *myrr_rq = &rq->myrr;
	struct list_head *queue = &myrr_rq->queue;
	struct sched_myrr_entity *myrr_entity = 
		container_of(queue->next, struct sched_myrr_entity, run_list);
	
	if(!myrr_entity)
		return;

	myrr_entity->update_num++;
	if(myrr_entity->update_num > 3)
	{
		myrr_entity->update_num = 0;
		list_del(&myrr_entity->run_list);
		list_add_tail(&myrr_entity->run_list, queue);
		resched_curr(rq);
	}
}
static void task_tick_myrr(struct rq *rq, struct task_struct *p, int queued)
{
	update_curr_myrr(rq);
}

const struct sched_class myrr_sched_class =
{
	.next = &fair_sched_class,
	.enqueue_task = enqueue_task_myrr,
	.dequeue_task = dequeue_task_myrr,
	.check_preempt_curr = check_preempt_curr_myrr,
	.pick_next_task = pick_next_task_myrr,
	.put_prev_task = put_prev_task_myrr,
#ifdef CONFIG_SMP
	.select_task_rq = select_task_rq_myrr,
#endif
	.set_curr_task = set_curr_task_myrr,
	.task_tick = task_tick_myrr,
	.prio_changed = prio_changed_myrr,
	.switched_to = switched_to_myrr,
	.update_curr = update_curr_myrr,
};

