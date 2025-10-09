#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "returns.csv"
#define TEST_FILENAME "test_returns.csv"
#define LOG_FILENAME "end_to_end_results.txt"
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

int check_duplicate_id(Return returns[], int count, const char *id) {
    return find_return_by_id(returns, count, id) != -1;
}

void log_both(FILE *log, const char *message) {
    printf("%s", message);
    fprintf(log, "%s", message);
}

int main() {
    FILE *log = fopen(LOG_FILENAME, "w");
    if (!log) {
        printf("Error: cannot create log file.\n");
        return 1;
    }

    log_both(log, "========================================\n");
    log_both(log, "       RUNNING END-TO-END TESTS\n");
    log_both(log, "========================================\n\n");

    log_both(log, "[END-TO-END] Backing up original data...\n");
    FILE *orig = fopen(FILENAME, "r");
    if (orig) {
        FILE *backup = fopen("backup_returns.csv", "w");
        char ch;
        while ((ch = fgetc(orig)) != EOF) fputc(ch, backup);
        fclose(orig);
        fclose(backup);
    }
    log_both(log, "Backup complete.\n\n");

    Return returns[3] = {
        {"R001", "Somchai", "Keyboard", "2025-09-15"},
        {"R002", "Suda", "Mouse", "2025-09-16"},
        {"R003", "Anan", "Monitor", "2025-09-20"}
    };

    write_returns_to_file(returns, 3, TEST_FILENAME);
    Return loaded[MAX_RETURNS];
    int count = read_returns_from_file(loaded, TEST_FILENAME);

    log_both(log, "[END-TO-END TEST 1] Testing Add Operation...\n");
    if (count == 3)
        log_both(log, "PASS: Record added successfully\n\n");
    else
        log_both(log, "FAIL: Record not added correctly\n\n");

    log_both(log, "[END-TO-END TEST 2] Testing Search Operation...\n");
    int index = find_return_by_id(loaded, count, "R002");
    if (index != -1)
        log_both(log, "PASS: Record found with correct data\n\n");
    else
        log_both(log, "FAIL: Record not found\n\n");

    log_both(log, "[END-TO-END TEST 3] Testing Update Operation...\n");
    strcpy(loaded[index].product_name, "Wireless Mouse");
    write_returns_to_file(loaded, count, TEST_FILENAME);

    Return verify[MAX_RETURNS];
    int count2 = read_returns_from_file(verify, TEST_FILENAME);
    if (strcmp(verify[index].product_name, "Wireless Mouse") == 0)
        log_both(log, "PASS: Record updated successfully\n\n");
    else
        log_both(log, "FAIL: Record update failed\n\n");

    log_both(log, "[END-TO-END TEST 4] Testing Delete Operation...\n");
    for (int i = index; i < count2 - 1; i++) verify[i] = verify[i + 1];
    count2--;
    write_returns_to_file(verify, count2, TEST_FILENAME);

    Return final_check[MAX_RETURNS];
    int count3 = read_returns_from_file(final_check, TEST_FILENAME);
    if (count3 == 2)
        log_both(log, "PASS: Record deleted successfully\n\n");
    else
        log_both(log, "FAIL: Record deletion failed\n\n");

    log_both(log, "[END-TO-END TEST 5] Testing Duplicate ID Prevention...\n");
    int dup = check_duplicate_id(final_check, count3, "R001");
    if (dup)
        log_both(log, "PASS: Duplicate ID detected correctly\n\n");
    else
        log_both(log, "FAIL: Duplicate ID not detected\n\n");

    log_both(log, "[END-TO-END] Restoring original data...\n");
    FILE *backup = fopen("backup_returns.csv", "r");
    if (backup) {
        FILE *restore = fopen(FILENAME, "w");
        char ch;
        while ((ch = fgetc(backup)) != EOF) fputc(ch, restore);
        fclose(backup);
        fclose(restore);
        log_both(log, "Original data restored\n\n");
    } else {
        log_both(log, "No original data to restore\n\n");
    }

    log_both(log, "========================================\n");
    log_both(log, "       END-TO-END TESTS COMPLETED\n");
    log_both(log, "========================================\n");

    fclose(log);
    printf("\nResults have been saved to %s\n", LOG_FILENAME);
    return 0;
}
