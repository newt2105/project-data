#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define a struct node
struct Node {
    double latitude;
    double longitude;
    int capacity;
    double distance; 
    struct Node* left;
    struct Node* right;
};
typedef struct Node* node;

// Make a new node
node newNode(double latitude, double longitude, int capacity, double distance) {
    node temp = (node)malloc(sizeof(struct Node));
    temp->latitude = latitude;
    temp->longitude = longitude;
    temp->capacity = capacity;
    temp->distance = distance;
    temp->left = temp->right = NULL;
    return temp;
}

// Global variable for the root node
node root = NULL;

// Insert 1 node to tree
node insert(node root, double latitude, double longitude, int capacity, double distance) {
    if (root == NULL) {
        return newNode(latitude, longitude, capacity, distance);
    }

    if (distance < root->distance) {
        root->left = insert(root->left, latitude, longitude, capacity, distance);
    } else {
        root->right = insert(root->right, latitude, longitude, capacity, distance);
    }

    return root;
}

// Compute distance according to coordinates
double calculateDistance(double x1, double y1, double x2, double y2) {
    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance;
}

// update distance when 1 car input coordinates
node updateDistances(node root, double latitude, double longitude) {
    if (root == NULL) {
        return NULL;
    }

    double distance = calculateDistance(latitude, longitude, root->latitude, root->longitude);
    root->distance = distance;

    root->left = updateDistances(root->left, latitude, longitude);
    root->right = updateDistances(root->right, latitude, longitude);

    return root;
}

// Find the nearest car park with available slot
node findNextAvailableParking(node root, double latitude, double longitude) {
    if (root == NULL) {
        return NULL;
    }
	
	//define a nearest node = null
    node nearestAvailable = NULL;
    double minDistance = -1;

    node current = root;
    while (current != NULL) {
        double distance = calculateDistance(latitude, longitude, current->latitude, current->longitude);

        if (current->capacity > 0 && (nearestAvailable == NULL || distance < minDistance)) {
            nearestAvailable = current;
            minDistance = distance;
        }
        
		// inorder travesal to find next car park
        if (distance < current->distance) {
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
    root = updateDistances(root, latitude, longitude); // update the distance before insert car park into BST
    root = insert(root, latitude, longitude, 0, 0);

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
            } else if (confirmOption == 2) {
                printf("Reservation canceled.\n");
            } else {
                printf("Invalid option.\n");
            }
        } else {
            printf("Parking spot is now full. Finding the next available spot...\n");
        }
    } else {
        printf("No available parking nearby.\n");
    }
}

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

void welcomeScreen() {
	// a list of available car park
    root = insert(root, 1, 3, 0, 0);
    root = insert(root, 3, 4, 2, 0);
    root = insert(root, 12.345, 23.567, 10, 0);
    root = insert(root, 9.876, 18.234, 0, 0);
    root = insert(root, 11.234, 21.567, 3, 0);

    mainMenu();
}

int main() {
    welcomeScreen();
    return 0;
}
