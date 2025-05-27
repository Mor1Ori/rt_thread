#include <rtthread.h>
#include <rthw.h>
#include "board.h"
#include "appdef.h"

#define TEST_HEAP_SIZE (32 * 1024)  // 32KB测试堆空间
static rt_uint8_t test_heap[TEST_HEAP_SIZE];
static rt_smem_t test_mem = RT_NULL;

static rt_thread_t alloc_thread = RT_NULL;

static void *test_ptr[5];
static void thread_alloc(void *parameter)
{
    rt_size_t free_size, used_size, max_block;
    void *new_ptr = RT_NULL;
    
    // 显示初始内存状态
    free_size = rt_smem_get_free_size(test_mem);
    used_size = rt_smem_get_used_size(test_mem);
    max_block = rt_smem_get_max_free_block(test_mem);
    rt_kprintf("Initial state:\n");
    rt_kprintf("Total free: %d bytes\n", free_size);
    rt_kprintf("Used: %d bytes\n", used_size);
    rt_kprintf("Max free block: %d bytes\n", max_block);
    
    // 测试最佳适配分配
    rt_kprintf("\nTest best fit allocation:\n");
    
    // 分配不同大小的内存块
    test_ptr[0] = rt_smem_best_fit_alloc(test_mem, 50);    // 50B
    if (test_ptr[0] != RT_NULL)
        rt_kprintf("Allocate 50B memory block successfully\n");
    rt_thread_mdelay(100);
    
    test_ptr[1] = rt_smem_best_fit_alloc(test_mem, 100);    // 100B
    if (test_ptr[1] != RT_NULL)
        rt_kprintf("Allocate 100B memory block successfully\n");
    rt_thread_mdelay(100);
    
    test_ptr[2] = rt_smem_best_fit_alloc(test_mem, 50);    // 50B
    if (test_ptr[2] != RT_NULL)
        rt_kprintf("Allocate 50B memory block successfully\n");
    rt_thread_mdelay(100);
    
    test_ptr[3] = rt_smem_best_fit_alloc(test_mem, 120);    // 120B
    if (test_ptr[3] != RT_NULL)
        rt_kprintf("Allocate 120B memory block successfully\n");
    rt_thread_mdelay(100);
    
    test_ptr[4] = rt_smem_best_fit_alloc(test_mem, 200);   // 200B
    if (test_ptr[4] != RT_NULL)
        rt_kprintf("Allocate 200B memory block successfully\n");
    
    // 显示分配后的内存状态
    free_size = rt_smem_get_free_size(test_mem);
    used_size = rt_smem_get_used_size(test_mem);
    max_block = rt_smem_get_max_free_block(test_mem);
    rt_kprintf("\nAfter allocation:\n");
    rt_kprintf("Total free: %d bytes\n", free_size);
    rt_kprintf("Used: %d bytes\n", used_size);
    rt_kprintf("Max free block: %d bytes\n", max_block);

    rt_thread_mdelay(500);  // 等待分配线程完成
    
    // 释放1、2、4号内存块
    rt_kprintf("\nRelease 1, 2, 4 memory blocks:\n");
    if (test_ptr[0] != RT_NULL)
    {
        rt_smem_free(test_ptr[0]);  // 释放50B
        rt_kprintf("Release 50B memory block\n");
        test_ptr[0] = RT_NULL;
    }
    rt_thread_mdelay(100);
    
    if (test_ptr[1] != RT_NULL)
    {
        rt_smem_free(test_ptr[1]);  // 释放100B
        rt_kprintf("Release 100B memory block\n");
        test_ptr[1] = RT_NULL;
    }
    rt_thread_mdelay(100);
    
    if (test_ptr[3] != RT_NULL)
    {
        rt_smem_free(test_ptr[3]);  // 释放120B
        rt_kprintf("Release 120B memory block\n");
        test_ptr[3] = RT_NULL;
    }
    
    // 显示释放后的内存状态
    free_size = rt_smem_get_free_size(test_mem);
    used_size = rt_smem_get_used_size(test_mem);
    max_block = rt_smem_get_max_free_block(test_mem);
    rt_kprintf("\nAfter release:\n");
    rt_kprintf("Total free: %d bytes\n", free_size);
    rt_kprintf("Used: %d bytes\n", used_size);
    rt_kprintf("Max free block: %d bytes\n", max_block);
    
    rt_thread_mdelay(500);
    
    // 分配新的120B内存块
    rt_kprintf("\nAllocate new 120B memory block:\n");
    new_ptr = rt_smem_best_fit_alloc(test_mem, 120);
    if (new_ptr != RT_NULL)
        rt_kprintf("Allocate new 120B memory block successfully\n");
    
    // 显示新分配后的内存状态
    free_size = rt_smem_get_free_size(test_mem);
    used_size = rt_smem_get_used_size(test_mem);
    max_block = rt_smem_get_max_free_block(test_mem);
    rt_kprintf("\nAfter new allocation:\n");
    rt_kprintf("Total free: %d bytes\n", free_size);
    rt_kprintf("Used: %d bytes\n", used_size);
    rt_kprintf("Max free block: %d bytes\n", max_block);
    
    rt_thread_mdelay(500);
    
    // 释放所有内存块
    rt_kprintf("\nRelease all memory blocks:\n");
    if (test_ptr[2] != RT_NULL)
    {
        rt_smem_free(test_ptr[2]);  // 释放50B
        rt_kprintf("Release 50B memory block\n");
        test_ptr[2] = RT_NULL;
    }
    rt_thread_mdelay(100);
    
    if (test_ptr[4] != RT_NULL)
    {
        rt_smem_free(test_ptr[4]);  // 释放200B
        rt_kprintf("Release 200B memory block\n");
        test_ptr[4] = RT_NULL;
    }
    rt_thread_mdelay(100);
    
    if (new_ptr != RT_NULL)
    {
        rt_smem_free(new_ptr);  // 释放新分配的120B
        rt_kprintf("Release new 120B memory block\n");
        new_ptr = RT_NULL;
    }
    
    // 显示最终内存状态
    free_size = rt_smem_get_free_size(test_mem);
    used_size = rt_smem_get_used_size(test_mem);
    max_block = rt_smem_get_max_free_block(test_mem);
    rt_kprintf("\nFinal state:\n");
    rt_kprintf("Total free: %d bytes\n", free_size);
    rt_kprintf("Used: %d bytes\n", used_size);
    rt_kprintf("Max free block: %d bytes\n", max_block);
}


int memtest_sample(void)
{
    int i;
    for (i = 0; i < 5; i++)
        test_ptr[i] = RT_NULL;
    
    // 初始化内存管理
    test_mem = rt_smem_init("test_heap", test_heap, TEST_HEAP_SIZE);
    if (test_mem == RT_NULL)
        rt_kprintf("Memory initialization failed!\n");

    // 创建分配线程
    alloc_thread = rt_thread_create("alloc",
                                  thread_alloc,
                                  RT_NULL,
                                  THREAD_STACK_SIZE,
                                  THREAD_PRIORITY,
                                  THREAD_TIMESLICE);
    if (alloc_thread != RT_NULL)
        rt_thread_startup(alloc_thread);
    return 0;
}

// 终止和删除所有线程
void terminate_memtest(void)
{
    if (alloc_thread != RT_NULL)
    {
        rt_thread_delete(alloc_thread);
        alloc_thread = RT_NULL;
    }
}

MSH_CMD_EXPORT(memtest_sample, memtest sample);