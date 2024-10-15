#include "types.h"
#include "stat.h"
#include "user.h"

int main (void) 
{
    int pid = getpid();
    printf(1, "Current priority: %d\n", get_priority(pid));

    set_priority(pid, 15);
    printf(1, "New set_priority: %d\n", get_priority(pid));

    set_priority(pid, 100);
    printf(1, "New priority (100 -> 39): %d\n", get_priority(pid));

    exit();
}