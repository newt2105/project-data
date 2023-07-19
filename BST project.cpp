#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define a struct node
struct Node {
    double latitude;
    double longitude;
    int capacity;
    struct Node* left;
    struct Node* right;
};
typedef struct Node* node;

// Global variable for the root node
node root = NULL;

// Make a new node
node newNode(double latitude, double longitude, int capacity) {
    node temp = (node)malloc(sizeof(struct Node));
    temp->latitude = latitude;
    temp->longitude = longitude;
    temp->capacity = capacity;
    temp->left = temp->right = NULL;
    return temp;
}

// Insert 1 node to tree based on distance
node insertByDistance(node root, double latitude, double longitude, int capacity) {
    if (root == NULL) {
        return newNode(latitude, longitude, capacity);
    }

    // Calculate distance to the current root node
    double currentDistance = sqrt(pow(root->latitude - latitude, 2) + pow(root->longitude - longitude, 2));

    // Calculate distance to the left child (if exists)
    double leftDistance = -1;
    if (root->left) {
        leftDistance = sqrt(pow(root->left->latitude - latitude, 2) + pow(root->left->longitude - longitude, 2));
    }

    // Calculate distance to the right child (if exists)
    double rightDistance = -1;
    if (root->right) {
        rightDistance = sqrt(pow(root->right->latitude - latitude, 2) + pow(root->right->longitude - longitude, 2));
    }

    // Determine which child to go down based on distance
    if (leftDistance == -1 || currentDistance < leftDistance) {
        root->left = insertByDistance(root->left, latitude, longitude, capacity);
    } else if (rightDistance == -1 || currentDistance < rightDistance) {
        root->right = insertByDistance(root->right, latitude, longitude, capacity);
    } else {
        root->left = insertByDistance(root->left, latitude, longitude, capacity);
    }

    return root;
}

// Insert 1 node to tree
node insert(node root, double latitude, double longitude, int capacity) {
    if (root == NULL) {
        return newNode(latitude, longitude, capacity);
    }

    if (latitude < root->latitude) {
        root->left = insert(root->left, latitude, longitude, capacity);
    } else if (latitude > root->latitude) {
        root->right = insert(root->right, latitude, longitude, capacity);
    }

    return root;
}

// Compute distance according to coordinates
double calculateDistance(double x1, double y1, double x2, double y2) {
    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance;
}

// Find the nearest car park with available slot
node findNextAvailableParking(node root, double latitude, double longitude, node* nearestAvailable) {
    if (root != NULL) {
        findNextAvailableParking(root->left, latitude, longitude, nearestAvailable);

        double distance = calculateDistance(latitude, longitude, root->latitude, root->longitude);

        if (root->capacity > 0 && (*nearestAvailable == NULL || distance < calculateDistance(latitude, longitude, (*nearestAvailable)->latitude, (*nearestAvailable)->longitude))) {
            *nearestAvailable = root;
        }

        findNextAvailableParking(root->right, latitude, longitude, nearestAvailable);
    }
}

// Update capacity
void updateCapacity(node parking) {
    parking->capacity -= 1;
}

// Option to handle reservation
void handleReservation(double latitude, double longitude) {
    node nearestAvailableParking = NULL;
    findNextAvailableParking(root, latitude, longitude, &nearestAvailableParking);

    if (nearestAvailableParking != NULL) {
        printf("Nearest available parking: Latitude = %.3f, Longitude = %.3f\n", nearestAvailableParking->latitude, nearestAvailableParking->longitude);
        printf("Available capacity: %d\n", nearestAvailableParking->capacity);

        if (nearestAvailableParking->capacity > 0) {
            int confirmOption;
            printf("Options:\n");
            printf("1. Confirm\n");
            printf("2. Cancel\n");
            printf("Choose an option: ");
            scanf("%d", &confirmOption);

            if (confirmOption == 1) {
                updateCapacity(nearestAvailableParking);
                printf("Parking spot updated successfully. Capacity: %d\n", nearestAvailableParking->capacity);
            } else if (confirmOption == 2) {
                printf("Reservation canceled.\n");
            } else {
                printf("Invalid option.\n");
            }
        } else {
            printf("Parking spot is already full. Finding the next available spot...\n");
        }
    } else {
        printf("No available parking nearby.\n");
    }
}

// Main menu function
void mainMenu() {
    int option;
    double latitude, longitude;

    while (1) {
        printf("Options:\n");
        printf("1. Enter your current coordinates\n");
        printf("2. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        if (option == 1) {
            printf("Enter your current latitude: ");
            scanf("%lf", &latitude);
            printf("Enter your current longitude: ");
            scanf("%lf", &longitude);

            handleReservation(latitude, longitude);
            printf("\n");
        } else if (option == 2) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid option.\n");
            printf("\n");
        }
    }
}

// Welcome screen function
void welcomeScreen() {
    // Insert car parks
    root = insert(root, 1, 3, 0);
    root = insert(root, 3, 4, 0);
    root = insert(root, 12.345, 23.567, 10);
    root = insert(root, 9.876, 18.234, 1);
    root = insert(root, 11.234, 21.567, 3);

    mainMenu();
}

// Main function
int main() {
    welcomeScreen();
    return 0;
}
