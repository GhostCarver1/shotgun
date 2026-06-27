#include "tests.h"

#define PASSCMP(val1, val2) printf("\033[32m[PASS] %s==%s\033[0m\n",val1,val2); return 0
#define FAILCMP(val1, val2) printf("\033[31m[FAIL] %s!=%s\033[0m\n",val1,val2); return 1
#define FAIL(message) printf("\033[31m[FAIL] %s \033[0m\n",message); return 1

int test_json_extractor_1()
{
    char json[] = "{\"hello\":\"hi\"}";
    char buffer[10];
    Result result = extract_json_value(json, "hello",  buffer, 10);
    if (result.status==ERROR)
    {
        FAIL(result.message);
    }
    if (!strcmp(buffer, "hi")){
        PASSCMP(buffer, "hi");
    }
    else 
    {
        FAILCMP(buffer,"hi");
    }
}

int test_json_string_extractor_1()
{
    char json[] = "{\"player_name\":[\"Bob\",\"Charlie\",\"alice\"]}";
    char buffer[4][10];
    Result result = extract_json_list_of_strings(json, "player_name", 4, 10, buffer);
    if (result.status==ERROR)
    {
        FAIL(result.message);
    }
    printf("buffer: %s\n",buffer[0]);
    printf("buffer: %s\n",buffer[1]);
    printf("buffer: %s\n",buffer[2]);
}

int main() {
    test_json_extractor_1();
    test_json_string_extractor_1();
    return 0;
}