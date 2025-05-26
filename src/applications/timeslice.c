#include <rtthread.h>
#include "board.h"
#include "appdef.h"

rt_thread_t tid1 = RT_NULL;
rt_thread_t tid2 = RT_NULL;
rt_thread_t tid_exit = RT_NULL;
/* 线程入口 */
static void thread_entry(void* parameter)
{
    rt_uint32_t value;
    rt_uint32_t count = 0;
    rt_base_t level;

    value = (rt_uint32_t)parameter;
    while (1)
    {
        if(0 == (count % 5))
        {
            rt_kprintf("thread %d is running ,thread %d count = %d\n", value , value , count);
            
            if(count> 200)
                return;
            rt_thread_mdelay(1000);
        }
         count++;
     }
}


int timeslice_sample(void)
{
    /* 创建线程 1 */
    tid1 = rt_thread_create("thread1",
                            thread_entry, (void*)1,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL){
        rt_thread_startup(tid1);
        //rt_kprintf("tid1");
    }
        
    /* 创建线程 2 */
    tid2 = rt_thread_create("thread2",
                            thread_entry, (void*)2,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE-5);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}

// 终止和删除所有线程
void terminate_timeslice(void) {
    if (tid1 != RT_NULL) {
        rt_thread_delete(tid1);
        tid1 = RT_NULL;
        //rt_kprintf("Thread 1 terminated.\n");
    }

    if (tid2 != RT_NULL) {
        rt_thread_delete(tid2);
        tid2 = RT_NULL;
        //rt_kprintf("Thread 2 terminated.\n");
    }
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timeslice_sample, timeslice sample);
