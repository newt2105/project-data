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
node findNextAvailableParking(node root, double latitude, double longitude) {
    if (root == NULL) {
        return NULL;
    }

    node nearestAvailable = NULL;
    double minDistance = INFINITY;

    node current = root;
    while (current != NULL) {
        double distance = calculateDistance(latitude, longitude, current->latitude, current->longitude);

        if ((current->capacity > 0 && (nearestAvailable == NULL || distance < minDistance)) || (current->capacity > 0 && nearestAvailable != NULL && nearestAvailable->capacity == 0)) {
            nearestAvailable = current;
            minDistance = distance;
        }

        if (latitude < current->latitude) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nearestAvailable;
}




// Update capacity
void updateCapacity(node parking) {
    parking->capacity -= 1;
}

// Option to handle reservation
void handleReservation(double latitude, double longitude) {
    while (1) {
        node nearestAvailableParking = findNextAvailableParking(root, latitude, longitude);

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
                    break;

                    if (nearestAvailableParking->capacity == 0) {
                        printf("Parking spot is now full. Finding the next available spot...\n");
                        continue; // Allow user to input next coordinates
                    } else {
                        break; // Exit loop if parking is confirmed and still available
                    }
                } else if (confirmOption == 2) {
                    printf("Reservation canceled.\n");
                    break; // Exit loop if reservation is canceled
                } else {
                    printf("Invalid option.\n");
                }
            } else {
                printf("Parking spot is already full. Finding the next available spot...\n");
                continue; // Allow user to input next coordinates
            }
        } else {
            printf("No available parking nearby.\n");
            break; // Exit loop if no available parking nearby
        }
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
