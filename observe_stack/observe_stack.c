#include <stdio.h>

static void trace(int n) {
    int local = n;
    printf("n=%2d  &n=%p  &local=%p\n", n, (void*)&n, (void*)&local);
    if (n) trace(n - 1);
}

int main(void) {
    trace(5);
}


/*
output:
n= 5  &n=0x16f89a9cc  &local=0x16f89a9c8
n= 4  &n=0x16f89a99c  &local=0x16f89a998
n= 3  &n=0x16f89a96c  &local=0x16f89a968
n= 2  &n=0x16f89a93c  &local=0x16f89a938
n= 1  &n=0x16f89a90c  &local=0x16f89a908

Those addresses are exactly the “stack frames marching.”
	•	They drop by 0x30 = 48 bytes each call on your box, so your per-call activation record is ~48 B (locals + saved FP + return addr + alignment per your ABI).
	•	Direction ↓ confirms stack grows downward on your mac.
*/