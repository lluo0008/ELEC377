/*+
 * Module:  lab2.c
 *
 * Purpose: Skeleton solution to ELEC 377 Lab2.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *theTask;

int cnt;

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){

    int numChars = 0;
    int pageSize = PAGE_SIZE >> 10;
    if (fpos == 0){
	    // write headers
        numChars += sprintf(page + numChars, "PID\t");
        numChars += sprintf(page + numChars, "UID\t");
        numChars += sprintf(page + numChars, "VSZ\t");
        numChars += sprintf(page + numChars, "RSS\n");
	    // find first task
        theTask = &init_task;
        while (theTask -> pid == 0)
        {
            theTask = theTask -> next_task;
        }
        // write first task
        firstTask = theTask;
        numChars += sprintf(page + numChars, "%d\t%d\t", theTask -> pid, theTask -> uid);

        

        if (theTask->mm == NULL)
        {
            numChars += sprintf(page + numChars, "0\t0\n");
        }
        else
        {
            numChars += sprintf(page + numChars, "%d\t%d\n", (theTask->mm->total_vm)*pageSize, (theTask->mm->rss)*pageSize); 
        }
        // advance to next task
        do
        {
            theTask = theTask -> next_task;
        }
        while (theTask -> pid == 0);


        // numChars = sprintf(page, "Hello");
        // numChars += sprintf(page + numChars, "World\n");
    } else {
        if (theTask == firstTask)
        {
            *eof = 0;
            *start = page;
            return 0;
        }
	    // write task info for one task
        numChars += sprintf(page + numChars, "%d\t%d\t", theTask -> pid, theTask -> uid);
        
        if (theTask->mm == NULL)
        {
            numChars += sprintf(page + numChars, "0\t0");
        }
        else
        {
            numChars += sprintf(page + numChars, "%d\t%d", (theTask->mm->total_vm)*pageSize, (theTask->mm->rss)*pageSize); 
        }
	    // advance to next task
        do
        {
            theTask = theTask -> next_task;
        }
        while (theTask -> pid == 0);
    }
    *eof = 1;
    *start = page;
    return numChars;
}

int init_module(){
   struct proc_dir_entry * proc_entry;
   proc_entry = create_proc_entry("lab2", 0444, NULL);
   if (proc_entry == NULL)
   {
       return -1;
   }
   proc_entry->read_proc = my_read_proc;
   return 0;
}

void cleanup_module(){
    remove_proc_entry("lab2", NULL);
}
