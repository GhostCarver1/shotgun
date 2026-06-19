#include <assert.h>
#include <stdio.h>

void test_add()
{
    assert(2+3== 5);
    assert(0+0 == 0);
    assert(-1+1 == 0);

    printf("test_add passed\n");
}