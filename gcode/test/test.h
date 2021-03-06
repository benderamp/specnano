#include<stdio.h>
#include"gcode_analyzer_test.h"
#include"g01_test.h"

/**
 * Pass test.
 */
void pass(char* test_name, char* message);

/**
 * Fail test.
 */
void fail(char* test_name, char* message);

/**
 * Run all self tests.
 */
void self_test(char* input_data_path);

