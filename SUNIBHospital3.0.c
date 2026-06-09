#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_CYAN    "\033[36m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_RED     "\033[31m"
#define C_WHITE   "\033[97m"

// Universal Windows and UNIX (Suggested by Sonnet 4.6 Low)
// Cross-platform compatibility recap: (Recapped by GitHub Copilot PR Tools)
// - Windows uses Sleep(ms) and "cls".
// - Linux/macOS use usleep(ms * 1000) and "clear".
// - Call sleep_ms() and clear_screen() everywhere below.
#ifdef _WIN32
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define sleep_ms(ms) usleep((ms) * 1000)
#endif

#ifdef _WIN32
    #define clear_screen() system("cls")
#else
    #define clear_screen() system("clear")
#endif

typedef struct Node {
    int key;
    char name[100];
    char phoneNum[100];
    char email[100];
    int visitation;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int getHeight(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

int myMax(int a, int b) {
    return (a > b) ? a : b;
}

int balanceVal(Node* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* rightRot(Node* b) {
    Node* a  = b->left;
    Node* T2 = a->right;
    a->right = b;
    b->left  = T2;
    b->height = myMax(getHeight(b->left), getHeight(b->right)) + 1;
    a->height = myMax(getHeight(a->left), getHeight(a->right)) + 1;
    return a;
}

Node* leftRot(Node* a) {
    Node* b  = a->right;
    Node* T2 = b->left;
    b->left  = a;
    a->right = T2;
    a->height = myMax(getHeight(a->left), getHeight(a->right)) + 1;
    b->height = myMax(getHeight(b->left), getHeight(b->right)) + 1;
    return b;
}

Node* existingMember(Node* root, int patientCode) {
    if (root == NULL) return NULL;
    if (patientCode == root->key) return root;
    if (patientCode < root->key) return existingMember(root->left, patientCode);
    else return existingMember(root->right, patientCode);
}

Node* newNode(int key) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) { printf("Allocation failed\n"); exit(1); }
    bool validation;
    n->visitation = 0;

    do {
        validation = false;
        printf("  Input name[5-30][Mr. |Mrs. ]: ");
        fgets(n->name, sizeof(n->name), stdin);
        n->name[strcspn(n->name, "\n")] = '\0';
        int length = strlen(n->name);
        if (length >= 5 && length <= 30)
            if (strncmp(n->name, "Mr. ", 4) == 0 || strncmp(n->name, "Mrs. ", 5) == 0)
                validation = true;
    } while (!validation);

    do {
        validation = false;
        printf("  Input email[10-20][email format]: ");
        fgets(n->email, sizeof(n->email), stdin);
        n->email[strcspn(n->email, "\n")] = '\0';
        int length = strlen(n->email);
        if (length >= 10 && length <= 20) {
            int com  = length - strlen(".com");
            int coid = length - strlen(".co.id");
            if (strcmp(n->email + com, ".com") == 0 || strcmp(n->email + coid, ".co.id") == 0) {
                int atCount = 0;
                for (int i = 0; i < length; i++) if (n->email[i] == '@') atCount++;
                if (n->email[0] != '@' && atCount == 1) validation = true;
            }
        }
    } while (!validation);

    do {
        validation = false;
        int valAmount = 0;
        printf("  Input phone number[10-13][numeric]: ");
        fgets(n->phoneNum, sizeof(n->phoneNum), stdin);
        n->phoneNum[strcspn(n->phoneNum, "\n")] = '\0';
        int length = strlen(n->phoneNum);
        if (length >= 10 && length <= 13) {
            for (int i = 0; i < length; i++) if (isdigit(n->phoneNum[i])) valAmount++;
            if (valAmount == length) validation = true;
        }
    } while (!validation);

    n->key    = key;
    n->left   = NULL;
    n->right  = NULL;
    n->height = 1;
    return n;
}

Node* insert(Node* node, int key) {
    if (node == NULL) return newNode(key);
    if (key < node->key)       node->left  = insert(node->left,  key);
    else if (key > node->key)  node->right = insert(node->right, key);
    else return node;

    node->height = 1 + myMax(getHeight(node->left), getHeight(node->right));
    int balance  = balanceVal(node);

    if (balance > 1  && key < node->left->key)  return rightRot(node);
    if (balance < -1 && key > node->right->key) return leftRot(node);
    if (balance > 1  && key > node->left->key)  { node->left  = leftRot(node->left);  return rightRot(node); }
    if (balance < -1 && key < node->right->key) { node->right = rightRot(node->right); return leftRot(node); }
    return node;
}

void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void loadingAnimation(const char* msg, int durationMs) {
    char frames[] = {'|', '/', '-', '\\'};
    int steps = durationMs / 100;
    for (int i = 0; i < steps; i++) {
        printf("\r  " C_CYAN "[%c]" C_RESET " %s", frames[i % 4], msg);
        fflush(stdout);
        sleep_ms(100);
    }
    printf("\r  " C_GREEN "[v]" C_RESET " %s - Done!\n", msg);
}

void printHeader() {
    printf(C_CYAN C_BOLD);
    printf("  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    printf("  |                                           |\n");
    printf("  |        ____  _   _  _   _ ___ ____        |\n");
    printf("  |       / ___|| | | || \\ | |_ _| __ )       |\n");
    printf("  |       \\___ \\| | | ||  \\| || ||  _ \\       |\n");
    printf("  |        ___) | |_| || |\\  || || |_) |      |\n");
    printf("  |       |____/ \\___/ |_| \\_|___|____/       |\n");
    printf("  |                                           |\n");
    printf("  |        Pharmacy Management System v2      |\n");
    printf("  |          Powered by AVL Tree (C)          |\n");
    printf("  |                                           |\n");
    printf("  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    printf(C_RESET "\n");
}

void printDivider() {
    printf(C_CYAN "  +-----+----------+---------------------------+----------+\n" C_RESET);
}

void printSectionTitle(const char* title) {
    printf(C_CYAN "  +-------------------------------------------+\n" C_RESET);
    printf(C_YELLOW C_BOLD "    >> %s\n" C_RESET, title);
    printf(C_CYAN "  +-------------------------------------------+\n" C_RESET);
}

void prescript(Node* root) {
    bool validation;
    char medicine[10][20] = {
        "Paracetamol","Diapet","Oralit","Paratusin","OB Herbal",
        "Ibuprofen","Aspirin","Morphine","Cetirizine","Multivitamins"
    };
    int medicinePrice[10] = {5000,7000,3500,5500,6000,5500,8000,10000,4000,4500};
    int medicineChoice[15];
    int medicineAmount[15];
    int idx = 0;

    if (root->visitation >= 0) {
        root->visitation++;
        printf("  Number of visit(s) = %d\n", root->visitation);
    }

    printf("\n");
    printf(C_CYAN "  +==============================================+\n");
    printf("  ||  No  | Medicine Name        |    Price    ||\n");
    printf("  +==============================================+\n" C_RESET);
    for (int i = 0; i < 10; i++)
        printf("  || %-3d | %-20s |  Rp. %-6d ||\n", i+1, medicine[i], medicinePrice[i]);
    printf(C_CYAN "  +==============================================+\n" C_RESET);

    do {
        do {
            validation = false;
            printf("  Input medicine[1-10]: ");
            scanf("%d", &medicineChoice[idx]);
            if (medicineChoice[idx] >= 1 && medicineChoice[idx] <= 10) {
                validation = true;
                medicineChoice[idx]--;
            }
        } while (!validation);

        do {
            validation = false;
            printf("  Input quantity[>=1]: ");
            scanf("%d", &medicineAmount[idx]);
            if (medicineAmount[idx] >= 1) validation = true;
        } while (!validation);

        char warn;
        bool fbreak = false;
        do {
            printf("  Add another medicine?[y|n]: ");
            scanf(" %c", &warn);
            if (warn == 'n' || warn == 'y') fbreak = true;
        } while (!fbreak);

        if (warn == 'n') break;
        if (idx == 14)   { printf("  Medicine limit reached.\n"); break; }
        idx++;
    } while (true);

    float total = 0;
    int totalAmount = 0;
    printf(C_YELLOW "\n  Order Summary:\n" C_RESET);
    for (int i = 0; i <= idx; i++) {
        printf("  %-20s - %dx = Rp.%d\n",
            medicine[medicineChoice[i]],
            medicineAmount[i],
            medicinePrice[medicineChoice[i]] * medicineAmount[i]);
        total       += medicinePrice[medicineChoice[i]] * medicineAmount[i];
        totalAmount += medicineAmount[i];
    }

    if (root->visitation != -1) {
        int tmp = totalAmount;
        float discount = 0;
        while (tmp >= 5) { tmp -= 5; discount += 0.1f; }
        if (discount >= 0.5f) discount = 0.5f;
        total = total * (1 - discount);
        if (discount > 0)
            printf(C_GREEN "  Discount: %d%%\n" C_RESET, (int)(discount * 100));
    }
    printf(C_GREEN C_BOLD "  Total Price: Rp.%.2f\n" C_RESET, total);
}

void inorderAll(Node* root, int counter[]) {
    if (root == NULL) return;
    inorderAll(root->left, counter);
    counter[0]++;
    printf(C_WHITE "  | %-3d | " C_RESET, counter[0]);
    printf(C_GREEN "ID: %-5d" C_RESET " | ", root->key);
    printf("%-25s | ", root->name);
    printf("Visit: %d\n", root->visitation);
    inorderAll(root->right, counter);
}

void viewAll(Node* root) {
    clear_screen();
    printHeader();
    printSectionTitle("ALL REGISTERED PATIENTS");

    if (root == NULL) {
        printf(C_RED "\n  [!] No patients registered yet.\n" C_RESET);
        printf("\n  Press Enter to continue...");
        getchar();
        return;
    }

    printDivider();
    printf(C_CYAN "  | No  | Pat. ID  | Name                      | Visits   |\n" C_RESET);
    printDivider();

    int counter[1] = {0};
    inorderAll(root, counter);
    printDivider();
    printf(C_GREEN "\n  Total patients: %d\n" C_RESET, counter[0]);
    printf("\n  Press Enter to continue...");
    getchar();
}

void inorderPrefix(Node* root, const char* prefix, int prefixLen, int counter[]) {
    if (root == NULL) return;
    inorderPrefix(root->left, prefix, prefixLen, counter);
    char keyStr[20];
    sprintf(keyStr, "%d", root->key);
    if (strncmp(keyStr, prefix, prefixLen) == 0) {
        counter[0]++;
        printf(C_WHITE "  | %-3d | " C_RESET, counter[0]);
        printf(C_GREEN "ID: %-5d" C_RESET " | ", root->key);
        printf("%-25s | ", root->name);
        printf("Visit: %d\n", root->visitation);
    }
    inorderPrefix(root->right, prefix, prefixLen, counter);
}

void viewByPrefix(Node* root) {
    clear_screen();
    printHeader();
    printSectionTitle("VIEW PATIENTS BY ID PREFIX");

    char prefix[20];
    printf("  Input patient ID prefix: ");
    fgets(prefix, sizeof(prefix), stdin);
    prefix[strcspn(prefix, "\n")] = '\0';

    int prefixLen = strlen(prefix);
    int valid = (prefixLen > 0);
    for (int i = 0; i < prefixLen; i++)
        if (prefix[i] < '0' || prefix[i] > '9') { valid = 0; break; }

    if (!valid) {
        printf(C_RED "\n  [!] Invalid prefix. Numbers only.\n" C_RESET);
        printf("\n  Press Enter to continue...");
        getchar();
        return;
    }

    loadingAnimation("Searching prefix", 500);
    printf(C_CYAN "\n  Patients with ID starting with \"%s\":\n\n" C_RESET, prefix);
    printDivider();
    printf(C_CYAN "  | No  | Pat. ID  | Name                      | Visits   |\n" C_RESET);
    printDivider();

    int counter[1] = {0};
    inorderPrefix(root, prefix, prefixLen, counter);
    printDivider();

    if (counter[0] == 0)
        printf(C_RED "\n  [!] No patient found with prefix \"%s\".\n" C_RESET, prefix);
    else
        printf(C_GREEN "\n  Found: %d patient(s).\n" C_RESET, counter[0]);

    printf("\n  Press Enter to continue...");
    getchar();
}

void searchPatient(Node* root) {
    clear_screen();
    printHeader();
    printSectionTitle("SEARCH PATIENT");

    int patientCode, valid = 0;
    do {
        printf("  Input patient code to search [1-500]: ");
        if (scanf("%d", &patientCode) == 1 && patientCode >= 1 && patientCode <= 500) valid = 1;
        else printf(C_RED "  [!] Invalid. Try again.\n" C_RESET);
        int buf; while ((buf = getchar()) != '\n' && buf != EOF);
    } while (!valid);

    loadingAnimation("Searching patient", 500);
    Node* found = existingMember(root, patientCode);
    if (found == NULL) {
        printf(C_RED "\n  [!] Patient with code %d not found.\n" C_RESET, patientCode);
    } else {
        printf(C_GREEN "\n  Patient found:\n" C_RESET);
        printf("  ID: %d\n", found->key);
        printf("  Name: %s\n", found->name);
        printf("  Email: %s\n", found->email);
        printf("  Phone: %s\n", found->phoneNum);
        printf("  Visitations: %d\n", found->visitation);
    }

    printf("\n  Press Enter to continue...");
    getchar();
}

Node* minValueNode(Node* node) {
    Node* cur = node;
    while (cur->left != NULL) cur = cur->left;
    return cur;
}

Node* deleteNode(Node* root, int key) {
    if (root == NULL) return root;

    if      (key < root->key) root->left  = deleteNode(root->left,  key);
    else if (key > root->key) root->right = deleteNode(root->right, key);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) { temp = root; root = NULL; }
            else              *root = *temp;
            free(temp);
        } else {
            Node* successor = minValueNode(root->right);
            root->key        = successor->key;
            strcpy(root->name,     successor->name);
            strcpy(root->phoneNum, successor->phoneNum);
            strcpy(root->email,    successor->email);
            root->visitation = successor->visitation;
            root->right = deleteNode(root->right, successor->key);
        }
    }

    if (root == NULL) return root;
    root->height = 1 + myMax(getHeight(root->left), getHeight(root->right));
    int balance  = balanceVal(root);

    if (balance > 1  && balanceVal(root->left)  >= 0) return rightRot(root);
    if (balance > 1  && balanceVal(root->left)  <  0) { root->left  = leftRot(root->left);  return rightRot(root); }
    if (balance < -1 && balanceVal(root->right) <= 0) return leftRot(root);
    if (balance < -1 && balanceVal(root->right) >  0) { root->right = rightRot(root->right); return leftRot(root); }
    return root;
}

void deletePatient(Node** root) {
    clear_screen();
    printHeader();
    printSectionTitle("DELETE A PATIENT");

    if (*root == NULL) {
        printf(C_RED "\n  [!] No patients to delete.\n" C_RESET);
        printf("\n  Press Enter to continue...");
        getchar();
        return;
    }

    int patientCode, valid = 0;
    do {
        printf("  Input patient code to delete [1-500]: ");
        if (scanf("%d", &patientCode) == 1 && patientCode >= 1 && patientCode <= 500) valid = 1;
        else printf(C_RED "  [!] Invalid. Try again.\n" C_RESET);
        int buf; while ((buf = getchar()) != '\n' && buf != EOF);
    } while (!valid);

    Node* found = existingMember(*root, patientCode);
    if (found == NULL) {
        printf(C_RED "\n  [!] Patient with code %d not found.\n" C_RESET, patientCode);
    } else {
        printf(C_YELLOW "\n  Deleting: %s (ID: %d)...\n" C_RESET, found->name, found->key);
        loadingAnimation("Removing from AVL Tree", 700);
        *root = deleteNode(*root, patientCode);
        printf(C_GREEN "\n  [v] Patient successfully deleted.\n" C_RESET);
    }

    printf("\n  Press Enter to continue...");
    getchar();
}

void menu() {
    clear_screen();
    printHeader();
    printf(C_CYAN "  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n" C_RESET);
    printf(C_YELLOW C_BOLD "  [||]  1 ||  Insert Patient + Prescription  [||]\n" C_RESET);
    printf(C_WHITE        "  [||]  2 ||  View All Patients             [||]\n" C_RESET);
    printf(C_WHITE        "  [||]  3 ||  View Patients by ID Prefix    [||]\n" C_RESET);
    printf(C_WHITE        "  [||]  4 ||  Search Patient                [||]\n" C_RESET);
    printf(C_WHITE        "  [||]  5 ||  Delete a Patient              [||]\n" C_RESET);
    printf(C_RED          "  [||]  6 ||  Exit                          [||]\n" C_RESET);
    printf(C_CYAN "  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n" C_RESET);
    printf("\n  " C_WHITE "Input your choice > " C_RESET);
}

int main() {
    enableANSI();
    Node* root = NULL;
    int choice;

    do {
        menu();
        scanf("%d", &choice);
        int buf; while ((buf = getchar()) != '\n' && buf != EOF);

        switch (choice) {
            case 1: {
                clear_screen();
                printHeader();
                printSectionTitle("INSERT PATIENT");
                int patientCode;
                bool validation = false;
                do {
                    printf("  Input patient code [1-500]: ");
                    if (scanf("%d", &patientCode) == 1 && patientCode >= 1 && patientCode <= 500)
                        validation = true;
                    else printf(C_RED "  [!] Invalid.\n" C_RESET);
                    int b; while ((b = getchar()) != '\n' && b != EOF);
                } while (!validation);

                Node* member = existingMember(root, patientCode);
                if (member != NULL) {
                    printf(C_GREEN "  Welcome back, %s!\n" C_RESET, member->name);
                    prescript(member);
                } else {
                    char warn; bool fbreak = false;
                    do {
                        printf("  Register membership?[y|n]: ");
                        scanf(" %c", &warn);
                        if (warn == 'n' || warn == 'y') fbreak = true;
                    } while (!fbreak);
                    int b; while ((b = getchar()) != '\n' && b != EOF);

                    if (warn == 'y') {
                        loadingAnimation("Registering patient", 600);
                        root = insert(root, patientCode);
                        Node* newUser = existingMember(root, patientCode);
                        prescript(newUser);
                    } else {
                        Node nonCustomer;
                        nonCustomer.visitation = -1;
                        prescript(&nonCustomer);
                    }
                }
                printf(C_YELLOW "\n  Press Enter to continue..." C_RESET);
                getchar();
                getchar();
                break;
            }
            case 2:
                viewAll(root);
                break;
            case 3:
                viewByPrefix(root);
                break;
            case 4:
                searchPatient(root);
                // printf(C_YELLOW "\n  [Search - coming from teammate]\n" C_RESET);
                // printf("  Press Enter to continue...");
                // getchar();
                break;
            case 5:
                deletePatient(&root);
                break;
            case 6:
                clear_screen();
                printHeader();
                printf(C_GREEN C_BOLD "\n  Thank you for using SUNIB Pharmacy!\n  Have a nice day :)\n\n" C_RESET);
                sleep_ms(1500);
                break;
            default:
                printf(C_RED "\n  [!] Invalid choice.\n" C_RESET);
                sleep_ms(700);
                break;
        }
    } while (choice != 6);

    return 0;
}
