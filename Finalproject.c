#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "returns.csv"
#define MAX_LINE_LENGTH 256
#define MAX_FIELD_LENGTH 50

// โครงสร้างข้อมูลสำหรับบันทึกข้อมูลการส่งคืน
typedef struct {
    char return_id[MAX_FIELD_LENGTH];
    char customer_name[MAX_FIELD_LENGTH];
    char item_name[MAX_FIELD_LENGTH];
    char return_date[MAX_FIELD_LENGTH];
} Return;

// อ่านข้อมูลทั้งหมดจากไฟล์ CSV
int read_returns(Return *returns, int max_returns) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && count < max_returns) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", 
               returns[count].return_id, 
               returns[count].customer_name, 
               returns[count].item_name, 
               returns[count].return_date);
        count++;
    }
    fclose(file);
    return count;
}

// บันทึกข้อมูลทั้งหมดลงในไฟล์ CSV
void write_returns(Return *returns, int count) {
    FILE *file = fopen(FILENAME, "w");
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

//เมนู
void display_menu() {
    printf("\n--- Product Return Management System ---\n");
    printf("1. Add a new return\n");
    printf("2. Search for a return\n");
    printf("3. Update a return\n");
    printf("4. Delete a return\n");
    printf("5. List all returns\n");
    printf("6. Exit\n");
    printf("----------------------------------------\n");
    printf("Enter your choice: ");
}

//เพิ่มข้อมูล
void add_return() {
    Return new_return;
    printf("Enter Return ID: ");
    scanf("%s", new_return.return_id);
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", new_return.customer_name);
    printf("Enter Item Name: ");
    scanf(" %[^\n]", new_return.item_name);
    printf("Enter Return Date (YYYY-MM-DD): ");
    scanf("%s", new_return.return_date);
    
    Return all_returns[100];
    int count = read_returns(all_returns, 100);
    all_returns[count] = new_return;
    write_returns(all_returns, count + 1);
    
    printf("Return added successfully.\n");
}

//หาข้อมูล
void search_return() {
    char query[MAX_FIELD_LENGTH];
    printf("Enter Return ID or Customer Name to search: ");
    scanf(" %[^\n]", query);

    Return all_returns[100];
    int count = read_returns(all_returns, 100);
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

//อัพเดทข้อมูล
void update_return() {
        char search_id[MAX_FIELD_LENGTH];
    printf("Enter Return ID to update: ");
    scanf("%s", search_id);

    Return all_returns[100];
    int count = read_returns(all_returns, 100);
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, search_id) == 0) {
            printf("Found! Current date is %s\n", all_returns[i].return_date);
            printf("Enter new Return Date (YYYY-MM-DD): ");
            scanf("%s", all_returns[i].return_date);
            found = 1;
            break;
        }
    }

    if (found) {
        write_returns(all_returns, count);
        printf("Return updated successfully.\n");
    } else {
        printf("Return ID not found.\n");
    }

}

//ลบ
void delete_return() {
        char delete_id[MAX_FIELD_LENGTH];
    printf("Enter Return ID to delete: ");
    scanf("%s", delete_id);

    Return all_returns[100];
    int count = read_returns(all_returns, 100);
    int found_index = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, delete_id) == 0) {
            found_index = i;
            break;
        }
    }
}

//แสดงทั้งหมด
void list_all_returns(){
        Return all_returns[100];
    int count = read_returns(all_returns, 100);

    if (count == 0) {
        printf("No returns found.\n");
        return;
    }

    printf("\n--- All Returns ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %s, Customer: %s, Item: %s, Date: %s\n",
               all_returns[i].return_id,
               all_returns[i].customer_name,
               all_returns[i].item_name,
               all_returns[i].return_date);
    }
    printf("-------------------\n");

}

int main() {
    int choice;
    do {
        display_menu();
        scanf("%d", &choice);
        while(getchar() != '\n'); // Clear input buffer
        
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
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}