#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
	if (argc != 3){
        printf(2, "Input was incorrect\n");
        exit();
    }
    int pid = atoi(argv[1]);
    int priority = atoi(argv[2]);

    if(set_priority(pid, priority) < 0){
        printf(2, "failed to set priority for given pid.\n");
    }
	exit();
}