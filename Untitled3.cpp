#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 150

struct studInfo {
    int studNr;
    char name[12];
    int age;
};

struct node {
    struct studInfo info;
    struct node *link;
};

typedef struct node* NODEPTR;

struct studInfo allStudent[MAX];
int studentCount = 0;

NODEPTR headNum = NULL;
NODEPTR headName = NULL;


void fileIntoArray();
NODEPTR insertSortedByNumber(NODEPTR head, struct studInfo student);
NODEPTR insertSortedByName(NODEPTR head, struct studInfo student);
NODEPTR deleteByNumber(NODEPTR head, int studNr);
NODEPTR deleteByName(NODEPTR head, char name[]);
void listStudents(NODEPTR head, int batchSize);
void insertNewStudent();
void deleteStudent();
void searchByName();
void listOldYoung();
void listAgeDistribution();
void menu();

int main() {
    fileIntoArray();
    for (int i = 0; i < studentCount; i++) {
        headNum = insertSortedByNumber(headNum, allStudent[i]);
        headName = insertSortedByName(headName, allStudent[i]);
    }

    menu();
    return 0;
}
void fileIntoArray() {
    FILE *fp = fopen("student.txt", "r");
    if (!fp) {
        printf("Cannot open student.txt\n");
        exit(1);
    }

    while (fscanf(fp, "%d %s %d", &allStudent[studentCount].studNr,
           allStudent[studentCount].name, &allStudent[studentCount].age) == 3) {
        studentCount++;
    }
    fclose(fp);
}

NODEPTR insertSortedByNumber(NODEPTR head, struct studInfo student) {
    NODEPTR newNode = (NODEPTR)malloc(sizeof(struct node));
    newNode->info = student;
    newNode->link = NULL;

    if (head == NULL || student.studNr < head->info.studNr) {
        newNode->link = head;
        return newNode;
    }

    NODEPTR current = head;
    while (current->link != NULL && current->link->info.studNr < student.studNr)
        current = current->link;

    newNode->link = current->link;
    current->link = newNode;
    return head;
}


NODEPTR insertSortedByName(NODEPTR head, struct studInfo student) {
    NODEPTR newNode = (NODEPTR)malloc(sizeof(struct node));
    newNode->info = student;
    newNode->link = NULL;

    if (head == NULL || strcmp(student.name, head->info.name) < 0) {
        newNode->link = head;
        return newNode;
    }

    NODEPTR current = head;
    while (current->link != NULL && strcmp(current->link->info.name, student.name) < 0)
        current = current->link;

    newNode->link = current->link;
    current->link = newNode;
    return head;
}

void listStudents(NODEPTR head, int batchSize) {
    int count = 0;
    while (head != NULL) {
        printf("%5d %-12s %2d\n", head->info.studNr, head->info.name, head->info.age);
        count++;
        if (count % batchSize == 0) {
            printf("Press Enter to continue...\n");
            getchar(); getchar();
        }
        head = head->link;
    }
}

void insertNewStudent() {
    struct studInfo newStud;
    printf("Enter new student (Number Name Age): ");
    scanf("%d %s %d", &newStud.studNr, newStud.name, &newStud.age);

    headNum = insertSortedByNumber(headNum, newStud);
    headName = insertSortedByName(headName, newStud);

    printf("\nUpdated list sorted by Number:\n");
    listStudents(headNum, 30);
    
    
}


void deleteStudent() {
    int studNr;
    printf("Enter student number to delete: ");
    scanf("%d", &studNr);

    NODEPTR current = headNum;
    int found = 0;
    char name[12];

    while (current != NULL) {
        if (current->info.studNr == studNr) {
            strcpy(name, current->info.name);
            found = 1;
            break;
        }
        current = current->link;
    }

    if (!found) {
        printf("Student not found.\n");
        return;
    }

    headNum = deleteByNumber(headNum, studNr);
    headName = deleteByName(headName, name);

    printf("\nList after deletion (Sorted by Number):\n");
    listStudents(headNum, 30);

    printf("\nList after deletion (Sorted by Name):\n");
    listStudents(headName, 25);
}

NODEPTR deleteByNumber(NODEPTR head, int studNr) {
    if (head == NULL) return NULL;
    if (head->info.studNr == studNr) {
        NODEPTR temp = head->link;
        free(head);
        return temp;
    }

    NODEPTR current = head;
    while (current->link != NULL && current->link->info.studNr != studNr)
        current = current->link;

    if (current->link != NULL) {
        NODEPTR temp = current->link;
        current->link = temp->link;
        free(temp);
    }
    return head;
}

NODEPTR deleteByName(NODEPTR head, char name[]) {
    if (head == NULL) return NULL;
    if (strcmp(head->info.name, name) == 0) {
        NODEPTR temp = head->link;
        free(head);
        return temp;
    }

    NODEPTR current = head;
    while (current->link != NULL && strcmp(current->link->info.name, name) != 0)
        current = current->link;

    if (current->link != NULL) {
        NODEPTR temp = current->link;
        current->link = temp->link;
        free(temp);
    }
    return head;
}

void searchByName() {
    char name[12];
    printf("Enter student name to search: ");
    scanf("%s", name);

    NODEPTR current = headName;
    while (current != NULL) {
        if (strcmp(current->info.name, name) == 0) {
            printf("Found: %d %s %d\n", current->info.studNr, current->info.name, current->info.age);
            return;
        }
        current = current->link;
    }
    printf("Student not found.\n");
}

void listOldYoung() {
    if (headNum == NULL) return;

    NODEPTR current = headNum;
    int youngestAge = current->info.age;
    int oldestAge = current->info.age;

    while (current != NULL) {
        if (current->info.age < youngestAge)
            youngestAge = current->info.age;
        if (current->info.age > oldestAge)
            oldestAge = current->info.age;
        current = current->link;
    }

    
    printf("Youngest Student(s):\n");
    current = headNum;
    while (current != NULL) {
        if (current->info.age == youngestAge)
            printf("%5d %-12s %2d\n", current->info.studNr, current->info.name, current->info.age);
        current = current->link;
    }

    
    printf("Oldest Student(s):\n");
    current = headNum;
    while (current != NULL) {
        if (current->info.age == oldestAge)
            printf("%5d %-12s %2d\n", current->info.studNr, current->info.name, current->info.age);
        current = current->link;
    }
}

void listAgeDistribution() {
    int ageDist[41] = {0};
    NODEPTR current = headNum;

    while (current != NULL) {
        if (current->info.age >= 15 && current->info.age <= 40)
            ageDist[current->info.age]++;
        current = current->link;
    }

    printf("Age Distribution :\n");
    for (int i = 15; i <= 40; i++) {
        if (ageDist[i] > 0)
            printf("Age %2d: %d student(s)\n", i, ageDist[i]);
			}
}

void menu() {
    int choice;

    do {
        printf("\n--- MENU ---\n");
        printf("1. List by Student Number \n");
        printf("2. List by Student Name \n");
        printf("3. Insert New Student\n");
        printf("4. Delete Student\n");
        printf("5. Search by Name\n");
        printf("6. Youngest & Oldest + Age Distribution\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: listStudents(headNum, 30); break;
            case 2: listStudents(headName, 25); break;
            case 3: insertNewStudent(); break;
            case 4: deleteStudent(); break;
            case 5: searchByName(); break;
            case 6: listOldYoung(); 
			        listAgeDistribution(); 
					break;
            case 7: printf("Exiting...\n");
			        break;
            default: printf("Invalid choice.\n");
         }  
     } while (choice != 7);
}
