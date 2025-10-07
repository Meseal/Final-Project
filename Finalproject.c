#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "returns.csv"
#define MAX_LINE_LENGTH 256
#define MAX_FIELD_LENGTH 50
#define MAX_RETURNS 100 

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

// ฟังก์ชันตรวจสอบว่า Return ID มั้ย
int is_id_duplicate(const char *return_id, const Return *all_returns, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(all_returns[i].return_id, return_id) == 0) {
            return 1;
        }
    }
    return 0;
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

//หาข้อมูล
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

//อัพเดทข้อมูล
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

//ลบ
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

//แสดงทั้งหมด
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

// Main Function
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
                printf("Exiting program. Goodbye! 👋\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}