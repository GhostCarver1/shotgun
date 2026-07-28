#include "tests.h"


#define FAIL(message) printf("\033[31m[FAIL] %s \033[0m\n",message); return 1;
#define PASS(message) printf("\032[31m[PASS] %s \033[0m\n",message); return 0;
#define CHECK(result) if (result.status==ERROR) {FAIL(result.message);}

int assert_equal_buffers(char * buffer1, char * buffer2)
{
    if (!strcmp(buffer1, buffer2)){
        printf("\033[32m[PASS] buffers are equal %s == %s\033[0m\n",buffer1,buffer2); 
        return 0;
    }
    else {
        printf("\033[31m[] buffers are not equal %s != %s\033[0m\n",buffer1,buffer2); 
        return 1;
    }
}

int assert_equal_status(Status result_status, Status expected_status)
{
    if (result_status == expected_status){
        printf("\033[32m[PASS] status are equal %d == %d\033[0m\n",result_status,result_status); 
        return 0;
    }
    else {
        printf("\033[31m[] status are not equal %d != %d\033[0m\n",result_status,result_status); 
        return 1;
    }
}

int test_json_extractor_1()
{
    char json[] = "{\"hello\":\"hi\"}";
    char buffer[10];
    Result result = extract_json_value(json, "hello",  buffer, 10);
    CHECK(result);
    return assert_equal_buffers(buffer,"hi");
}

int test_json_string_extractor_1()
{
    char json[] = "{\"player_name\":[\"Bob\",\"Charlie\",\"Alice\",\"Justin\"]}";
    char buffer[4][10];
    Result result = extract_json_list_of_strings(json, "player_name", 4, 10, buffer);
    CHECK(result)

    int failures = 0;
    failures += assert_equal_buffers(buffer[0],"Bob");
    failures += assert_equal_buffers(buffer[1],"Charlie");
    failures += assert_equal_buffers(buffer[2],"Alice");
    failures += assert_equal_buffers(buffer[3],"Justin");

    return failures;
}

int test_json_string_extractor_2()
{
    char json[] = "{\"player_name\":[\"Bob\",\"Charlie\",\"Alice\",\"Justin\"]}";
    char buffer[4][10];
    Result result = extract_json_list_of_strings(json, "player_name", 4, 10, buffer);

    return assert_equal_status(result.status, SUCCESS);
}

int test_json_string_extractor_3()
{
    char json[] = "{\"player_name\":[\"1\",\"2\",\"3\",\"4\"]}";
    char buffer[4][10];
    Result result = extract_json_list_of_strings(json, "player_name", 4, 10, buffer);
    CHECK(result)

    int failures = 0;

    failures += assert_equal_buffers(buffer[0],"1");
    failures += assert_equal_buffers(buffer[1],"2");
    failures += assert_equal_buffers(buffer[2],"3");
    failures += assert_equal_buffers(buffer[3],"4");

    return failures;
}

int test_json_string_extractor_4()
{
    char json[] = "{\"pl\":[]}";
    char buffer[4][10];
    Result result = extract_json_list_of_strings(json, "pl", 4, 10, buffer);

    return assert_equal_status(result.status, SUCCESS);
}

int main() {
    int failures = 0;
    failures += test_json_string_extractor_1();
    failures += test_json_string_extractor_2();
    failures += test_json_string_extractor_3();
    failures += test_json_string_extractor_4();
    failures += test_json_extractor_1();
    printf("FAILURES: %d ", failures);
    return failures;
}