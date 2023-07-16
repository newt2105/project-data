#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// define a struct node
struct Node {
    double latitude;
    double longitude;
    int capacity;
    struct Node* left;
    struct Node* right;
};
typedef struct Node* node;

// make a new node
node newNode(double latitude, double longitude, int capacity) {
    node temp = (node)malloc(sizeof(Node));
    temp->latitude = latitude;
    temp->longitude = longitude;
    temp->capacity = capacity;
    temp->left = temp->right = NULL;
    return temp;
}

// insert 1 node to tree
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
    double minDistance = -99999;

    node current = root;
    while (current != NULL) {
        double distance = calculateDistance(latitude, longitude, current->latitude, current->longitude);

        if (current->capacity > 0 && (nearestAvailable == NULL || distance < minDistance)) {
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

// update capacity
void updateCapacity(node parking) {
    parking->capacity -= 1;
}

// Hàm chính
int main() {
    node root = NULL;

    // insert car park 
    root = insert(root, 1, 3, 1);
    root = insert(root, 3, 4, 2);
    root = insert(root, 12.345, 23.567, 10);
    root = insert(root, 9.876, 18.234, 1);
    root = insert(root, 11.234, 21.567, 3);

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

            node nearestAvailableParking = findNextAvailableParking(root, latitude, longitude);

            if (nearestAvailableParking != NULL) {
                printf("Nearest available parking: Latitude = %.3f, Longitude = %.3f\n", nearestAvailableParking->latitude, nearestAvailableParking->longitude);
                printf("Available capacity: %d\n", nearestAvailableParking->capacity);
                printf("\n");

                int confirmOption;
                printf("Options:\n");
                printf("1. Confirm\n");
                printf("2. Cancel\n");
                printf("Choose an option: ");
                scanf("%d", &confirmOption);

                if (confirmOption == 1) {
                    if (nearestAvailableParking->capacity == 1) {
                        nearestAvailableParking->capacity -= 1;
                        printf("Parking spot updated successfully. Capacity: 0\n");
                        printf("\n");
                    } else if (nearestAvailableParking->capacity > 1) {
                        nearestAvailableParking->capacity -= 1;
                        printf("Parking spot updated successfully. Capacity: %d\n", nearestAvailableParking->capacity);
                        printf("\n");
                    } else {
                        printf("Parking spot is full. Finding the next available spot...\n");
                        printf("\n");
                        nearestAvailableParking = findNextAvailableParking(root, latitude, longitude);

                        if (nearestAvailableParking == NULL) {
                            printf("No available parking nearby.\n");
                            printf("\n");
                        } else {
                            printf("Nearest available parking: Latitude = %.3f, Longitude = %.3f\n", nearestAvailableParking->latitude, nearestAvailableParking->longitude);
                            printf("Available capacity: %d\n", nearestAvailableParking->capacity);
                            printf("\n");
                        }
                    }
                } else if (confirmOption == 2) {
                    printf("Reservation canceled.\n");
                    printf("\n");
                } else {
                    printf("Invalid option.\n");
                    printf("\n");
                }
            } else {
                printf("No available parking nearby.\n");
                printf("\n");
            }
        } else if (option == 2) {
            printf("Exiting...\n");
            printf("\n");
            break;
        } else {
            printf("Invalid option.\n");
            printf("\n");
        }
    }

    return 0;
}
