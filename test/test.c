#include "ext/unity.h"
#include "../src/tools.h"
#include "../src/population.h"
#include "../src/field.h"
#include "../src/simulation.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void clean_sarr(short* arr, int s){
    for(int i = 0; i < s; i++){
        arr[i] = 0;
    }
}

void test_pos_to_cell(){
    point p;

    p.x = 0.0; p.y = 0.0;
    TEST_ASSERT_EQUAL_INT(0, pos_to_cell(p));

    p.x = 1.0; p.y = 0.0;
    TEST_ASSERT_EQUAL_INT(256-16, pos_to_cell(p));

    p.y = 1.0; p.x = 0.0;
    TEST_ASSERT_EQUAL_INT(15, pos_to_cell(p));

    p.y = 1.0; p.x = 1.0;
    TEST_ASSERT_EQUAL_INT(255, pos_to_cell(p));
}

void test_add_to_cell(){
    cell_map map;
    init_cell_map(&map, 10);
    add_to_cell(&map, 5, 0, 10);
    add_to_cell(&map, 0, 1, 10);
    add_to_cell(&map, 0, 2, 10);
    add_to_cell(&map, 5, 3, 10);
    add_to_cell(&map, 5, 4, 10);
    add_to_cell(&map, 15, 5, 10);
    add_to_cell(&map, 15, 6, 10);
    add_to_cell(&map, 5, 7, 10);
    add_to_cell(&map, 15, 8, 10);
    add_to_cell(&map, 15, 9, 10);
    short out_arr[10] = {0};

    clean_sarr(out_arr, 10);
    short exp_arr0[10] = {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    get_cell_content(&map, out_arr, 7);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(exp_arr0, out_arr, 1, "Empty case");
    clean_sarr(out_arr, 10);

    short exp_arr1[10] = {0, 3, 4, 7, -1, 0, 0, 0, 0};
    get_cell_content(&map, out_arr, 5);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(exp_arr1, out_arr, 5, "Medium case");

    clean_sarr(out_arr, 10);
    short exp_arr2[10] = {1, 2, -1, 0, 0, 0, 0, 0, 0, 0,};
    get_cell_content(&map, out_arr, 0);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(exp_arr2, out_arr, 3, "Corner bottom");

    clean_sarr(out_arr, 10);
    short exp_arr3[10] = {5, 6, 8, 9, -1, 0, 0, 0, 0, 0,};
    get_cell_content(&map, out_arr, 15);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(exp_arr3, out_arr, 5, "Corner top");
    free_cell_map(&map);
}

void test_map(){
    TEST_ASSERT_EQUAL_FLOAT(3.0, map(1.0, 0.0, 2.0, 1.5, 4.5));
    TEST_ASSERT_EQUAL_FLOAT(3.0, map(0.0, 0.0, 5.0, 3.0, 10.0));
}

void test_map1(){
    TEST_ASSERT_EQUAL_FLOAT(3.0, map1(0.5, 1.5, 4.5));
    TEST_ASSERT_EQUAL_FLOAT(3.0, map1(0.0, 3.0, 10.0));
}

void test_sat1(){
    TEST_ASSERT_EQUAL_FLOAT(0.0, sat1(-0.1));
    TEST_ASSERT_EQUAL_FLOAT(1.0, sat1(3.0));
    TEST_ASSERT_EQUAL_FLOAT(0.33, sat1(0.33));
}

void test_rotate(){
    point p1, p2;
    p1.x = 1.0;
    p1.y = 1.0;
    p2 = rotate(p1, HALF_PI);

    TEST_ASSERT_EQUAL_FLOAT(-1.0, p2.x);
    TEST_ASSERT_EQUAL_FLOAT(1.0, p2.y);

    p2 = rotate(p1, 0);
    TEST_ASSERT_EQUAL_FLOAT(1.0, p2.x);
    TEST_ASSERT_EQUAL_FLOAT(1.0, p2.y);
}

void test_norm(){
    point p1, p2;
    p1.x = 1.0;
    p1.y = 1.0;
    p2.x = 0;
    p2.y = 1.0;
    TEST_ASSERT_EQUAL_FLOAT(sqrt(2.0), norm(p1));
    TEST_ASSERT_EQUAL_FLOAT(1.0, norm(p2));
}

void test_angle(){
    point p1, p2, p3;
    p1.x = 1.0; p1.y = 1.0;
    p2.x = 0; p2.y = 1.0;
    p3.x = -1.0; p3.y = 1.0;

    TEST_ASSERT_EQUAL_FLOAT(HALF_PI/2, angle(p1, p2));
    TEST_ASSERT_EQUAL_FLOAT(-HALF_PI/2, angle(p2, p1));
    TEST_ASSERT_EQUAL_FLOAT(HALF_PI, angle(p1, p3));
}

int main(void){
    UnityBegin("test/test.c");

    RUN_TEST(test_pos_to_cell);
    RUN_TEST(test_add_to_cell);
    RUN_TEST(test_map);
    RUN_TEST(test_map1);
    RUN_TEST(test_sat1);
    RUN_TEST(test_norm);
    RUN_TEST(test_angle);

    UnityEnd();
    return 0;
}
