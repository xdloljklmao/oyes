#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node {
    int size;
    int p_id;
    bool allocated;
    struct Node* left;
    struct Node* right;
} Node;

bool valid_power(int n) {
    if (n <= 0) return false;
    while (n % 2 == 0) {
        n /= 2;
    }
    return n == 1;
}

Node* createNode(int size) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->size = size;
    node->allocated = false;
    node->p_id = -1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void merge(Node* root) {
    if (!root || !root->left || !root->right) return;

    merge(root->left);
    merge(root->right);

    if (!root->left->allocated && !root->right->allocated &&
        !root->left->left && !root->right->left &&
        !root->left->right && !root->right->right) {
        free(root->left);
        free(root->right);
        root->left = NULL;
        root->right = NULL;
    }
}

bool allocate(Node* root, int size, int pid) {
    if (root->allocated || root->size < size)
        return false;

    if (root->size == size && !root->left && !root->right) {
        root->allocated = true;
        root->p_id = pid;
        return true;
    }

    if (!root->left && !root->right) {
        int halfsize = root->size / 2;
        root->left = createNode(halfsize);
        root->right = createNode(halfsize);
    }

    if (allocate(root->left, size, pid)) return true;
    return allocate(root->right, size, pid);
}

bool deallocate(Node* root, int pid) {
    if (!root) return false;

    if (root->allocated && root->p_id == pid) {
        root->allocated = false;
        root->p_id = -1;
        return true;
    }

    if (deallocate(root->left, pid)) {
        merge(root);
        return true;
    }
    if (deallocate(root->right, pid)) {
        merge(root);
        return true;
    }

    return false;
}

void TreePrint(Node* root, int depth) {
    if (!root) return;

    for (int i = 0; i < depth; i++) {
        if (i == depth - 1) {
            printf("|--");
        } else {
            printf("|  ");
        }
    }
    printf("[%d]", root->size);

    if (root->allocated) {
        printf(" -> Allocated to P%d [%d]", root->p_id, root->size);
    }
    printf("\n");

    TreePrint(root->left, depth + 1);
    TreePrint(root->right, depth + 1);
}

int ext_frag(Node* root) {
    if (!root) return 0;

    if (!root->allocated && !root->left && !root->right) {
        return root->size;
    }

    return ext_frag(root->left) + ext_frag(root->right);
}

int main() {
    int mem_size;
    int choice;
    int proc_id, proc_size;

    do {
        printf("Enter total memory size (power of 2): ");
        scanf("%d", &mem_size);
        if (!valid_power(mem_size)) {
            printf("Please enter a valid size that is a power of 2.\n");
        }
    } while (!valid_power(mem_size));

    Node* root = createNode(mem_size);

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Allocate a process\n");
        printf("2. Deallocate a process\n");
        printf("3. View final tree and exit\n\n");
        printf("DISCALIMER: Please enter processes and requests in an orderly manner!\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter Process Number: ");
            scanf("%d", &proc_id);

            printf("Enter size for Process P%d: ", proc_id);
            scanf("%d", &proc_size);

            int req_size = 1;
            while (req_size < proc_size) req_size *= 2;

            if (!allocate(root, req_size, proc_id)) {
                printf("Process P%d could not be allocated.\n", proc_id);
            } else {
                printf("Process P%d allocated %d KB.\n", proc_id, req_size);
            }

        } else if (choice == 2) {
            printf("Enter Process Number to Deallocate: ");
            scanf("%d", &proc_id);

            if (deallocate(root, proc_id)) {
                printf("Process P%d deallocated.\n", proc_id);
            } else {
                printf("Process P%d was not allocated or not found.\n", proc_id);
            }

        } else if (choice == 3) {
            printf("\n<< Buddy System Allocation Tree >>\n");
            printf("Root ");
            TreePrint(root, 0);

            int ext_frag = ext_frag(root);
            printf("\nTotal External Fragmentation: %d KB\n", ext_frag);

            break;
        } else {
            printf("Invalid choice. Please select 1, 2 or 3.\n");
        }
    }

    return 0;
}
