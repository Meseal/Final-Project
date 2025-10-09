#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_FILENAME "test_returns.csv"
#define RESULT_FILENAME "unit_test_results.txt"
#define MAX_FIELD_LENGTH 50
#define MAX_RETURNS 100

typedef struct {
    char return_id[MAX_FIELD_LENGTH];
    char customer_name[MAX_FIELD_LENGTH];
    char product_name[MAX_FIELD_LENGTH];
    char return_date[MAX_FIELD_LENGTH];
} Return;

int read_returns_from_file(Return returns[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    int count = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n",
                  returns[count].return_id,
                  returns[count].customer_name,
                  returns[count].product_name,
                  returns[count].return_date) == 4) {
        count++;
    }
    fclose(file);
    return count;
}

void write_returns_to_file(Return returns[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s\n",
                returns[i].return_id,
                returns[i].customer_name,
                returns[i].product_name,
                returns[i].return_date);
    }
    fclose(file);
}

int find_return_by_id(Return returns[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(returns[i].return_id, id) == 0)
            return i;
    }
    return -1;
}

int is_id_duplicate(Return returns[], int count, const char *id) {
    return find_return_by_id(returns, count, id) != -1;
}

void log_result(FILE *log, const char *test_name, int passed) {
    fprintf(log, "%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

int main() {
    FILE *log = fopen(RESULT_FILENAME, "w");
    if (!log) {
        printf("Error: cannot create result file.\n");
        return 1;
    }

    int all_passed = 1;

    Return returns[2] = {
        {"R001", "Somchai", "Keyboard", "2025-09-15"},
        {"R002", "Suda", "Mouse", "2025-09-16"}
    };

    int pass1 = (is_id_duplicate(returns, 2, "R001") == 1 &&
                 is_id_duplicate(returns, 2, "R999") == 0);
    log_result(log, "test_is_id_duplicate", pass1);
    if (!pass1) all_passed = 0;

    int pass2 = (find_return_by_id(returns, 2, "R002") == 1 &&
                 find_return_by_id(returns, 2, "R999") == -1);
    log_result(log, "test_find_return_by_id", pass2);
    if (!pass2) all_passed = 0;

    Return temp[2] = {
        {"R001", "Somchai", "Keyboard", "2025-09-15"},
        {"R002", "Suda", "Mouse", "2025-09-16"}
    };
    write_returns_to_file(temp, 2, TEST_FILENAME);
    Return loaded[MAX_RETURNS];
    int count = read_returns_from_file(loaded, TEST_FILENAME);
    int pass3 = (count == 2 &&
                 strcmp(loaded[0].return_id, "R001") == 0 &&
                 strcmp(loaded[1].customer_name, "Suda") == 0);
    log_result(log, "test_read_write_returns", pass3);
    if (!pass3) all_passed = 0;

    fprintf(log, "\nOVERALL RESULT: %s\n", all_passed ? "PASS" : "FAIL");
    fclose(log);

    printf("Unit test results written to %s\n", RESULT_FILENAME);
    return 0;
}
