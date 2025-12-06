#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>  // For getch() to mask password

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"
#define TEMP_FILE "temp.txt"
#define MAX_TRIES 3
#define MAX_STUDENTS 500

struct Student {
    int roll;
    char name[50];
    float marks;
};

struct User {
    char username[20];
    char password[20];
    char role[20];
    char lastLogin[40];
};

char currentRole[20];
char currentUser[20];

// Function Prototypes
int loginSystem();
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudent();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudents();
void topNStudents();
void changePassword();
void saveLastLogin();
void autoCreateFiles();
void filterSearchStudents();
void pause();
void countStudents();

// ------------------------- MAIN ----------------------------
int main() {
    autoCreateFiles();

    if (loginSystem()) {
        saveLastLogin();
        mainMenu();
    } else {
        printf("\nLogin Failed, Exiting...\n");
    }
    return 0;
}

// ------------------------- UTILITY ----------------------------
void pause() {
    printf("\nPress any key to continue...");
    getch();
}

void autoCreateFiles() {
    FILE *fp;

    // Create credentials file if missing - include a placeholder lastLogin "-" so fscanf works reliably
    fp = fopen(CREDENTIAL_FILE, "a+");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0) { // Empty file
            fprintf(fp, "admin 1234 ADMIN -\n");
            fprintf(fp, "staff staff123 STAFF -\n");
            fprintf(fp, "guest guest123 GUEST -\n");
        }
        fclose(fp);
    }

    // Create students file if missing
    fp = fopen(STUDENT_FILE, "a");
    if (fp) fclose(fp);
}

// ------------------------- LOGIN & PASSWORD ----------------------------
int loginSystem() {
    char username[20], password[40], ch;
    char fileUser[20], filePass[40], fileRole[20], lastLogin[40];
    FILE *fp;
    int tries = 0;

    while (tries < MAX_TRIES) {
        printf("\n====== Login Screen ======\n");
        printf("Username: ");
        scanf("%19s", username);
        printf("Password: ");

        // Password masking using getch
        int i = 0;
        while ((ch = getch()) != 13) { // Enter key (CR)
            if (ch == 8) { // Backspace
                if (i > 0) {
                    i--;
                    printf("\b \b");
                }
            } else if (ch == 0 || ch == 224) { // Arrow keys etc. skip
                getch();
                continue;
            } else {
                if (i < (int)(sizeof(password) - 1)) {
                    password[i++] = ch;
                    printf("*");
                }
            }
        }
        password[i] = '\0';
        printf("\n");

        fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credentials file not found!\n");
            return 0;
        }

        int found = 0;
        // Read 4 whitespace-separated fields
        while (fscanf(fp, "%19s %39s %19s %39s", fileUser, filePass, fileRole, lastLogin) == 4) {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
                strcpy(currentUser, username);
                strcpy(currentRole, fileRole);
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) return 1;
        tries++;
        printf("Invalid credentials! %d attempt(s) remaining.\n", MAX_TRIES - tries);
    }

    return 0; // Failed after max tries
}

void saveLastLogin() {
    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) return;

    FILE *temp = fopen(TEMP_FILE, "w");
    if (!temp) { fclose(fp); return; }

    char user[20], pass[40], role[20], last[40];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char now[40];
    sprintf(now, "%02d-%02d-%04d_%02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1,
            tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    while (fscanf(fp, "%19s %39s %19s %39s", user, pass, role, last) == 4) {
        if (strcmp(user, currentUser) == 0)
            fprintf(temp, "%s %s %s %s\n", user, pass, role, now);
        else
            fprintf(temp, "%s %s %s %s\n", user, pass, role, last);
    }

    fclose(fp);
    fclose(temp);
    remove(CREDENTIAL_FILE);
    rename(TEMP_FILE, CREDENTIAL_FILE);
}

// ------------------------- MAIN MENU ----------------------------
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

void adminMenu() {
    int choice;
    while (1) {
        printf("\n==== ADMIN MENU ====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Sort Students\n");
        printf("7. Top N Students\n");
        printf("8. Filter/Search Students\n");
        printf("9. Change Password\n");
        printf("10. Count Students\n");
        printf("11. Logout\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // flush invalid input
            printf("Invalid input.\n"); 
            pause(); 
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudent(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 7: topNStudents(); break;
            case 8: filterSearchStudents(); break;
            case 9: changePassword(); break;
            case 10: countStudents(); break;
            case 11: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); pause();
        }
    }
}

void staffMenu() {
    int choice;
    while (1) {
        printf("\n==== STAFF MENU ====\n");
        printf("1. Display All Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student Details\n");
        printf("4. Sort Students\n");
        printf("5. Top N Students\n");
        printf("6. Filter/Search Students\n");
        printf("7. Change Password\n");
        printf("8. Logout\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // flush invalid input
            printf("Invalid input.\n");
            pause();
            continue;
        }

        switch (choice) {
            case 1: displayStudent(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: sortStudents(); break;
            case 5: topNStudents(); break;
            case 6: filterSearchStudents(); break;
            case 7: changePassword(); break;
            case 8: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); pause();
        }
    }
}



// ------------------------- GUEST MENU ----------------------------
void guestMenu() {
    int choice;
    while (1) {
        printf("\n==== GUEST MENU ====\n");
        printf("1. Display All Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n"); pause(); continue;
        }

        switch (choice) {
            case 1: displayStudent(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); pause();
        }
    }
}

// ------------------------- STUDENT FUNCTIONS ----------------------------
void addStudent() {
    if (strcmp(currentRole, "GUEST") == 0) {
        printf("Permission Denied!\n"); pause();
        return;
    }

    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) { printf("Error opening students file\n"); pause(); return; }

    printf("Enter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name (no spaces): ");
    scanf("%49s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student Added Successfully!\n"); pause();
}

void displayStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("No student records found!\n"); pause(); return; }

    printf("\n===== STUDENT LIST =====\n");
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp); pause();
}

void searchStudent() {
    int r, found = 0;
    struct Student s;
    printf("Enter Roll No to Search: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("No records found!\n"); pause(); return; }

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == r) {
            printf("\nRecord Found!\nRoll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Record Not Found.\n");
    pause();
}

void updateStudent() {
    if (strcmp(currentRole, "GUEST") == 0) {
        printf("Permission Denied!\n"); pause();
        return;
    }

    int r, found = 0;
    struct Student s;
    printf("Enter Roll No to Update: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        printf("Error opening file!\n"); pause(); return;
    }

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == r) {
            found = 1;
            printf("Enter New Name (no spaces): "); scanf("%49s", s.name);
            printf("Enter New Marks: "); scanf("%f", &s.marks);
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp); fclose(temp);
    remove(STUDENT_FILE);
    rename(TEMP_FILE, STUDENT_FILE);

    if (found) printf("Record Updated Successfully!\n");
    else printf("Record Not Found.\n");
    pause();
}

void deleteStudent() {
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf("Only ADMIN can delete records!\n"); pause();
        return;
    }

    int r, found = 0;
    struct Student s;
    printf("Enter Roll No to Delete: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        printf("Error opening file!\n"); pause(); return;
    }

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll != r)
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        else
            found = 1;
    }

    fclose(fp); fclose(temp);
    remove(STUDENT_FILE);
    rename(TEMP_FILE, STUDENT_FILE);

    if (found) printf("Record Deleted Successfully!\n");
    else printf("Record Not Found.\n");
    pause();
}

// ------------------------- SORT ----------------------------
void sortStudents() {
    struct Student students[MAX_STUDENTS];
    int n = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("No records found!\n"); pause(); return; }

    while (n < MAX_STUDENTS && fscanf(fp, "%d %49s %f", &students[n].roll, students[n].name, &students[n].marks) == 3) {
        n++;
    }
    fclose(fp);

    if (n == 0) { printf("No students to sort.\n"); pause(); return; }

    int choice;
    printf("Sort by: 1. Roll (asc)  2. Name (asc)  3. Marks (asc)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            int swapFlag = 0;
            if (choice == 1 && students[i].roll > students[j].roll) swapFlag = 1;
            else if (choice == 2 && strcmp(students[i].name, students[j].name) > 0) swapFlag = 1;
            else if (choice == 3 && students[i].marks > students[j].marks) swapFlag = 1;

            if (swapFlag) {
                struct Student tmp = students[i];
                students[i] = students[j];
                students[j] = tmp;
            }
        }
    }

    printf("\n===== Sorted Student List =====\n");
    for (i = 0; i < n; i++)
        printf("Roll: %d | Name: %s | Marks: %.2f\n", students[i].roll, students[i].name, students[i].marks);

    // Ask user if they want to overwrite file with sorted order
    char ans;
    printf("\nOverwrite students file with this sorted order? (y/n): ");
    getchar(); // consume newline
    ans = getchar();
    if (ans == 'y' || ans == 'Y') {
        FILE *fw = fopen(STUDENT_FILE, "w");
        if (fw) {
            for (i = 0; i < n; i++)
                fprintf(fw, "%d %s %.2f\n", students[i].roll, students[i].name, students[i].marks);
            fclose(fw);
            printf("File updated with sorted order.\n");
        } else {
            printf("Error writing to file.\n");
        }
    }

    pause();
}

// ------------------------- TOP N ----------------------------
void topNStudents() {
    struct Student students[MAX_STUDENTS];
    int n = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("No records found!\n"); pause(); return; }

    while (n < MAX_STUDENTS && fscanf(fp, "%d %49s %f", &students[n].roll, students[n].name, &students[n].marks) == 3)
        n++;
    fclose(fp);

    if (n == 0) { printf("No students available.\n"); pause(); return; }

    int N;
    printf("Enter N (top N students): ");
    scanf("%d", &N);

    // Sort by marks descending
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (students[i].marks < students[j].marks) {
                struct Student tmp = students[i];
                students[i] = students[j];
                students[j] = tmp;
            }
        }
    }

    printf("\n===== Top %d Students =====\n", N);
    for (i = 0; i < N && i < n; i++)
        printf("Roll: %d | Name: %s | Marks: %.2f\n", students[i].roll, students[i].name, students[i].marks);

    pause();
}

// ------------------------- CHANGE PASSWORD ----------------------------
void changePassword() {
    char oldPass[40], newPass[40], confirm[40];
    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        printf("Error opening credentials file.\n"); pause(); return;
    }

    printf("Enter Old Password: ");
    scanf("%39s", oldPass);

    int verified = 0;
    char user[20], pass[40], role[20], last[40];
    while (fscanf(fp, "%19s %39s %19s %39s", user, pass, role, last) == 4) {
        if (strcmp(user, currentUser) == 0) {
            if (strcmp(oldPass, pass) != 0) {
                printf("Incorrect Old Password!\n");
                verified = -1;
                fprintf(temp, "%s %s %s %s\n", user, pass, role, last);
            } else {
                verified = 1;
                printf("Enter New Password: "); scanf("%39s", newPass);
                printf("Confirm New Password: "); scanf("%39s", confirm);
                if (strcmp(newPass, confirm) != 0) {
                    printf("Passwords do not match!\n");
                    fprintf(temp, "%s %s %s %s\n", user, pass, role, last);
                } else {
                    fprintf(temp, "%s %s %s %s\n", user, newPass, role, last);
                }
            }
        } else {
            fprintf(temp, "%s %s %s %s\n", user, pass, role, last);
        }
    }

    fclose(fp); fclose(temp);
    remove(CREDENTIAL_FILE);
    rename(TEMP_FILE, CREDENTIAL_FILE);

    if (verified == 1) printf("Password Changed Successfully!\n");
    pause();
}

// ------------------------- FILTER/SEARCH ----------------------------
void filterSearchStudents() {
    int choice;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("No records found!\n"); pause(); return; }

    printf("Filter/Search by: 1. Marks Range  2. Name Substring\n");
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); pause(); fclose(fp); return; }

    if (choice == 1) {
        float min, max;
        printf("Enter Min Marks: "); scanf("%f", &min);
        printf("Enter Max Marks: "); scanf("%f", &max);
        printf("\n===== Students in Range =====\n");
        while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
            if (s.marks >= min && s.marks <= max)
                printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    } else if (choice == 2) {
        char substr[50];
        printf("Enter Name Substring: "); scanf("%49s", substr);
        printf("\n===== Students Matching =====\n");
        while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
            if (strstr(s.name, substr) != NULL)
                printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    } else {
        printf("Invalid choice.\n");
    }
    fclose(fp); pause();
}


// ------------------------- COUNT STUDENTS ----------------------------
void countStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("No records found!\n"); pause(); return; }

    int count = 0;
    struct Student s;
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) count++;
    fclose(fp);

    printf("Total Students: %d\n", count);
    pause();
}
