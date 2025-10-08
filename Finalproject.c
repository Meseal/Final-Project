#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "returns.csv"
#define TEST_FILENAME "test_returns.csv"
#define MAX_LINE_LENGTH 256
#define MAX_FIELD_LENGTH 50
#define MAX_RETURNS 100 

typedef struct {
    char return_id[MAX_FIELD_LENGTH];
    char customer_name[MAX_FIELD_LENGTH];
    char item_name[MAX_FIELD_LENGTH];
    char return_date[MAX_FIELD_LENGTH];
} Return;

int read_returns_from_file(const char *filename, Return *returns, int max_returns) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && count < max_returns) {
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", 
                   returns[count].return_id, 
                   returns[count].customer_name, 
                   returns[count].item_name, 
                   returns[count].return_date) == 4) {
            count++;
        }
    }
    fclose(file);
    return count;
}

int read_returns(Return *returns, int max_returns) {
    return read_returns_from_file(FILENAME, returns, max_returns);
}

void write_returns_to_file(const char *filename, Return *returns, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Cannot open file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s\n", 
                 returns[i].return_id, 
                 returns[i].customer_name, 
                 returns[i].item_name, 
                 returns[i].return_date);
    }
    fclose(file);
}

void write_returns(Return *returns, int count) {
    write_returns_to_file(FILENAME, returns, count);
}

int is_id_duplicate(const char *return_id, const Return *all_returns, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, return_id) == 0) {
            return 1;
        }
    }
    return 0;
}

int find_return_by_id(const char *return_id, const Return *all_returns, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, return_id) == 0) {
            return i;
        }
    }
    return -1;
}

void display_menu() {
    printf("\n--- Product Return Management System ---\n");
    printf("1. Add a new return\n");
    printf("2. Search for a return\n");
    printf("3. Update a return\n");
    printf("4. Delete a return\n");
    printf("5. List all returns\n");
    printf("6. Run Unit Tests\n");
    printf("7. Run END-TO-END Tests\n");
    printf("8. Exit\n");
    printf("----------------------------------------\n");
    printf("Enter your choice: ");
}

void add_return() {
    Return new_return;
    Return all_returns[MAX_RETURNS];
    int count = read_returns(all_returns, MAX_RETURNS);

    if (count >= MAX_RETURNS) {
        printf("Error: Maximum number of returns reached (%d).\n", MAX_RETURNS);
        return;
    }

    char temp_id[MAX_FIELD_LENGTH];
    printf("Enter Return ID: ");
    scanf("%s", temp_id);
    
    if (is_id_duplicate(temp_id, all_returns, count)) {
        printf("Error: Return ID '%s' already exists. Please use a unique ID.\n", temp_id);
        return;
    }
    
    strcpy(new_return.return_id, temp_id); 
    while(getchar() != '\n');
    
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", new_return.customer_name);
    printf("Enter Item Name: ");
    scanf(" %[^\n]", new_return.item_name);
    printf("Enter Return Date (A.D.YYYY-MM-DD): ");
    scanf("%s", new_return.return_date);
    
    printf("\n--- Data to be added ---\n");
    printf("ID: %s\n", new_return.return_id);
    printf("Customer: %s\n", new_return.customer_name);
    printf("Item: %s\n", new_return.item_name);
    printf("Date: %s\n", new_return.return_date);
    printf("------------------------\n");
    
    all_returns[count] = new_return;
    write_returns(all_returns, count + 1);
    
    printf("Return added successfully.\n");
}

void search_return() {
    char query[MAX_FIELD_LENGTH];
    printf("Enter Return ID or Customer Name to search: ");
    scanf(" %[^\n]", query);

    Return all_returns[MAX_RETURNS];
    int count = read_returns(all_returns, MAX_RETURNS);
    int found_count = 0;

    printf("\nSearch Results:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, query) == 0 || 
            strstr(all_returns[i].customer_name, query) != NULL) {
            printf("ID: %s, Customer: %s, Item: %s, Date: %s\n",
                   all_returns[i].return_id,
                   all_returns[i].customer_name,
                   all_returns[i].item_name,
                   all_returns[i].return_date);
            found_count++;
        }
    }

    if (found_count == 0) {
        printf("No matching return found.\n");
    }
}

void update_return() {
    char search_id[MAX_FIELD_LENGTH];
    printf("Enter Return ID to update: ");
    scanf("%s", search_id);

    Return all_returns[MAX_RETURNS];
    int count = read_returns(all_returns, MAX_RETURNS);
    int found_index = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, search_id) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        int update_choice;
        Return *r = &all_returns[found_index];

        printf("\nFound Return ID: %s\n", r->return_id);
        printf("Current Data: Customer: %s, Item: %s, Date: %s\n", r->customer_name, r->item_name, r->return_date);

        printf("\n--- What to update? ---\n");
        printf("1. Customer Name\n");
        printf("2. Item Name\n");
        printf("3. Return Date\n");
        printf("4. Cancel Update\n");
        printf("Enter your choice: ");
        scanf("%d", &update_choice);
        while(getchar() != '\n');
        
        switch (update_choice) {
            case 1:
                printf("Enter new Customer Name: ");
                scanf(" %[^\n]", r->customer_name);
                break;
            case 2:
                printf("Enter new Item Name: ");
                scanf(" %[^\n]", r->item_name);
                break;
            case 3:
                printf("Enter new Return Date (A.D.YYYY-MM-DD): ");
                scanf("%s", r->return_date);
                break;
            case 4:
                printf("Update cancelled.\n");
                return;
            default:
                printf("Invalid choice. Update cancelled.\n");
                return;
        }

        write_returns(all_returns, count);
        printf("Return updated successfully.\n");
    } else {
        printf("Return ID not found.\n");
    }
}

void delete_return() {
    char delete_id[MAX_FIELD_LENGTH];
    printf("Enter Return ID to delete: ");
    scanf("%s", delete_id);

    Return all_returns[MAX_RETURNS];
    int count = read_returns(all_returns, MAX_RETURNS);
    int found_index = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, delete_id) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        char confirm;
        Return *r = &all_returns[found_index];
        
        printf("\nFound Return: ID: %s, Customer: %s, Item: %s, Date: %s\n", 
               r->return_id, r->customer_name, r->item_name, r->return_date);
        printf("Are you sure you want to delete this return? (y/n): ");
        scanf(" %c", &confirm); 

        if (confirm == 'y' || confirm == 'Y') {
            for (int i = found_index; i < count - 1; i++) {
                all_returns[i] = all_returns[i + 1];
            }
            
            write_returns(all_returns, count - 1); 
            printf("Return ID '%s' deleted successfully.\n", delete_id);
        } else {
            printf("Deletion cancelled.\n");
        }
    } else {
        printf("Return ID not found.\n");
    }
}

void list_all_returns(){
    Return all_returns[MAX_RETURNS];
    int count = read_returns(all_returns, MAX_RETURNS);

    if (count == 0) {
        printf("No returns found.\n");
        return;
    }

    printf("\n--- All Returns (%d records) ---\n", count);
    printf("ID\t\t| Customer Name\t\t| Item Name\t\t| Return Date\n");
    printf("----------------|-----------------------|-----------------------|-------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s | %-21s | %-21s | %s\n",
               all_returns[i].return_id,
               all_returns[i].customer_name,
               all_returns[i].item_name,
               all_returns[i].return_date);
    }
    printf("--------------------------------------------------------------------------------\n");
}

void test_is_id_duplicate() {
    printf("\n[TEST] Testing is_id_duplicate()...\n");
    
    Return test_returns[3];
    strcpy(test_returns[0].return_id, "R001");
    strcpy(test_returns[1].return_id, "R002");
    strcpy(test_returns[2].return_id, "R003");
    
    if (is_id_duplicate("R002", test_returns, 3)) {
        printf("PASS: Duplicate ID detected correctly\n");
    } else {
        printf("FAIL: Should detect duplicate ID\n");
    }
    
    if (!is_id_duplicate("R999", test_returns, 3)) {
        printf("PASS: Non-duplicate ID detected correctly\n");
    } else {
        printf("FAIL: Should not detect duplicate for unique ID\n");
    }
}

void test_find_return_by_id() {
    printf("\n[TEST] Testing find_return_by_id()...\n");
    
    Return test_returns[3];
    strcpy(test_returns[0].return_id, "R001");
    strcpy(test_returns[1].return_id, "R002");
    strcpy(test_returns[2].return_id, "R003");
    
    int index = find_return_by_id("R002", test_returns, 3);
    if (index == 1) {
        printf("PASS: Found correct index for existing ID\n");
    } else {
        printf("FAIL: Should find index 1 for R002\n");
    }
    
    index = find_return_by_id("R999", test_returns, 3);
    if (index == -1) {
        printf("PASS: Returned -1 for non-existing ID\n");
    } else {
        printf("FAIL: Should return -1 for non-existing ID\n");
    }
}

void test_read_write_returns() {
    printf("\n[TEST] Testing read/write operations...\n");
    
    Return test_data[2];
    strcpy(test_data[0].return_id, "TEST001");
    strcpy(test_data[0].customer_name, "Test Customer 1");
    strcpy(test_data[0].item_name, "Test Item 1");
    strcpy(test_data[0].return_date, "2025-01-01");
    
    strcpy(test_data[1].return_id, "TEST002");
    strcpy(test_data[1].customer_name, "Test Customer 2");
    strcpy(test_data[1].item_name, "Test Item 2");
    strcpy(test_data[1].return_date, "2025-01-02");
    
    write_returns_to_file(TEST_FILENAME, test_data, 2);
    
    Return read_data[MAX_RETURNS];
    int count = read_returns_from_file(TEST_FILENAME, read_data, MAX_RETURNS);
    
    if (count == 2) {
        printf("PASS: Read correct number of records\n");
    } else {
        printf("FAIL: Should read 2 records, got %d\n", count);
    }
    
    if (strcmp(read_data[0].return_id, "TEST001") == 0 &&
        strcmp(read_data[1].customer_name, "Test Customer 2") == 0) {
        printf("PASS: Data integrity maintained\n");
    } else {
        printf("FAIL: Data corruption detected\n");
    }
    
    remove(TEST_FILENAME);
}

void run_unit_tests() {
    printf("\n========================================\n");
    printf("       RUNNING UNIT TESTS\n");
    printf("========================================\n");
    
    test_is_id_duplicate();
    test_find_return_by_id();
    test_read_write_returns();
    
    printf("\n========================================\n");
    printf("       UNIT TESTS COMPLETED\n");
    printf("========================================\n");
}

void run_end_to_end_tests() {
    printf("\n========================================\n");
    printf("       RUNNING END-TO-END TESTS\n");
    printf("========================================\n");
    
    Return test_returns[MAX_RETURNS];
    int count;
    
    printf("\n[END-TO-END] Backing up original data...\n");
    Return backup[MAX_RETURNS];
    int backup_count = read_returns(backup, MAX_RETURNS);
    
    printf("\n[END-TO-END TEST 1] Testing Add Operation...\n");
    Return new_return;
    strcpy(new_return.return_id, "END001");
    strcpy(new_return.customer_name, "END Test Customer");
    strcpy(new_return.item_name, "END Test Item");
    strcpy(new_return.return_date, "2025-10-08");
    
    count = read_returns(test_returns, MAX_RETURNS);
    test_returns[count] = new_return;
    write_returns(test_returns, count + 1);
    
    count = read_returns(test_returns, MAX_RETURNS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(test_returns[i].return_id, "END001") == 0) {
            found = 1;
            break;
        }
    }
    if (found) {
        printf("PASS: Record added successfully\n");
    } else {
        printf("FAIL: Record not found after add\n");
    }
    
    printf("\n[END-TO-END TEST 2] Testing Search Operation...\n");
    int search_index = find_return_by_id("END001", test_returns, count);
    if (search_index != -1 && strcmp(test_returns[search_index].customer_name, "END Test Customer") == 0) {
        printf("PASS: Record found with correct data\n");
    } else {
        printf("FAIL: Search operation failed\n");
    }
    
    printf("\n[END-TO-END TEST 3] Testing Update Operation...\n");
    if (search_index != -1) {
        strcpy(test_returns[search_index].customer_name, "Updated Customer");
        write_returns(test_returns, count);
        
        count = read_returns(test_returns, MAX_RETURNS);
        search_index = find_return_by_id("END001", test_returns, count);
        if (strcmp(test_returns[search_index].customer_name, "Updated Customer") == 0) {
            printf("PASS: Record updated successfully\n");
        } else {
            printf("FAIL: Update operation failed\n");
        }
    }
    
    printf("\n[END-TO-END TEST 4] Testing Delete Operation...\n");
    search_index = find_return_by_id("END001", test_returns, count);
    if (search_index != -1) {
        for (int i = search_index; i < count - 1; i++) {
            test_returns[i] = test_returns[i + 1];
        }
        write_returns(test_returns, count - 1);
        
        count = read_returns(test_returns, MAX_RETURNS);
        if (find_return_by_id("END001", test_returns, count) == -1) {
            printf("PASS: Record deleted successfully\n");
        } else {
            printf("FAIL: Delete operation failed\n");
        }
    }
    
    printf("\n[END-TO-END TEST 5] Testing Duplicate ID Prevention...\n");
    count = read_returns(test_returns, MAX_RETURNS);
    if (count > 0) {
        char existing_id[MAX_FIELD_LENGTH];
        strcpy(existing_id, test_returns[0].return_id);
        
        if (is_id_duplicate(existing_id, test_returns, count)) {
            printf("PASS: Duplicate ID detected correctly\n");
        } else {
            printf("FAIL: Failed to detect duplicate ID\n");
        }
    }
    
    printf("\n[END-TO-END] Restoring original data...\n");
    write_returns(backup, backup_count);
    printf("Original data restored\n");
    
    printf("\n========================================\n");
    printf("       END-TO-END TESTS COMPLETED\n");
    printf("========================================\n");
}

int main() {
    int choice;
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'); 
            choice = 0; 
            continue;
        }
        while(getchar() != '\n'); 
        
        switch (choice) {
            case 1:
                add_return();
                break;
            case 2:
                search_return();
                break;
            case 3:
                update_return();
                break;
            case 4:
                delete_return();
                break;
            case 5:
                list_all_returns();
                break;
            case 6:
                run_unit_tests();
                break;
            case 7:
                run_end_to_end_tests();
                break;
            case 8:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}
