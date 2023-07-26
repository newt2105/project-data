#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Coordinate { // Coordinate of car and carpark
    double x;
    double y;
};
typedef struct Coordinate coordinate;

struct ParkingLot { // define of car park
    struct Coordinate location;
    int capacity;
};
typedef struct ParkingLot parkinglot;

struct Node { // define of node in BST
    parkinglot data;
    struct Node* left;
    struct Node* right;
};

typedef struct Node* tree;
tree t = NULL;

// Calculate distance from car to car park
double Distance(coordinate carLocation, coordinate parkingLotLocation) {
    return sqrt(pow(carLocation.x - parkingLotLocation.x, 2) + pow(carLocation.y - parkingLotLocation.y, 2));
}

// Hàm tạo một node mới
tree createNode(parkinglot parkingLot) {
    tree newNode = (tree)malloc(sizeof(struct Node));
    newNode->data = parkingLot;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}	

// Insert depend on distance from car to car park
tree insertByDistance(tree t, parkinglot parkingLot, coordinate carLocation) {
    if (t == NULL) {
        if (parkingLot.capacity > 0) {
            return createNode(parkingLot);
        }
        return NULL; // if capacity = 0, not insert
    }

    double distanceNewLot = Distance(carLocation, parkingLot.location);
    double distanceCurrentLot = Distance(carLocation, t->data.location);

    if (distanceNewLot < distanceCurrentLot) {
        // insert to the left 
        if (parkingLot.capacity > 0) {
            t->left = insertByDistance(t->left, parkingLot, carLocation);
        }
    } else {
        //  insert to the right 
        if (parkingLot.capacity > 0) {
            t->right = insertByDistance(t->right, parkingLot, carLocation);
        }
    }

    return t;
}



void inorderTraversal(tree t) { // test function
    if (t != NULL) {
        inorderTraversal(t->left);
        printf("Coordinate: (%.2lf, %.2lf), Capacity: %d\n", t->data.location.x, t->data.location.y, t->data.capacity);
        inorderTraversal(t->right);
    }
}

// Find nearest car park
tree findLeftMostNode(tree t) {
    if (t == NULL) {
        return NULL;
    }

    // find left most child
    while (t->left != NULL) {
        t = t->left;
    }

    return t;
}

// update capacity
void updateCapacity(tree t, coordinate chosenLocation, struct ParkingLot* carpark, int num) {
    if (t == NULL)
        return;
    updateCapacity(t->left, chosenLocation, carpark, num);

    
    if (t->data.location.x == chosenLocation.x && t->data.location.y == chosenLocation.y) {
        if (t->data.capacity > 0) {
            t->data.capacity--;
            printf("\n");
            
            // find index of the car park in array and remove 1 capacity
            for (int i = 0; i < num; i++) {
                if (carpark[i].location.x == chosenLocation.x && carpark[i].location.y == chosenLocation.y) {
                    carpark[i].capacity--;
                    break;
                }
            }
        } else {
            printf("\n");
        }
    }

    updateCapacity(t->right, chosenLocation, carpark, num);
}

// delete old tree before make a new one
void deleteTree(tree t) {
    if (t == NULL)
        return;
    deleteTree(t->left);
    deleteTree(t->right);
    free(t);
}

void processOption() {
    struct ParkingLot carpark[100]; // array of car park
    int num = 0; // number of carpark

    int option;

    // initial car park
    carpark[num].location.x = 2;
    carpark[num].location.y = 2;
    carpark[num].capacity = 1;
    num++;

    carpark[num].location.x = 9;
    carpark[num].location.y = 9;
    carpark[num].capacity = 6;
    num++;

    carpark[num].location.x = 1;
    carpark[num].location.y = 1;
    carpark[num].capacity = 1;
    num++;

    carpark[num].location.x = 3;
    carpark[num].location.y = 3;
    carpark[num].capacity = 1;
    num++;

    while (1) {
        coordinate carLocation;
        printf("Option:\n1. Input your coordinate\n2. Exit\n");
        scanf("%d", &option);

        if (option == 1) {
            printf("\nYour coordinate (x y): ");
            scanf("%lf %lf", &carLocation.x, &carLocation.y);

            for (int i = 0; i < num; i++) {
                t = insertByDistance(t, carpark[i], carLocation);
            }

            inorderTraversal(t);

            tree nearest = findLeftMostNode(t);


            if (nearest != NULL) {
                printf("The nearest parking lot with available slots: Coordinate: (%.2lf, %.2lf), Capacity: %d\n",
                       nearest->data.location.x, nearest->data.location.y, nearest->data.capacity);

                printf("Confirm? (1: YES / 0: NO)? ");
                int choose;
                scanf("%d", &choose);
                if (choose == 1) {
                    updateCapacity(t, nearest->data.location, carpark, num);
                } else if (choose == 0) {
                    printf(".\n");
                } else {
                    printf("Invalid option!\n");
                }
            } else {
                printf("No available parking lot found!\n");
            }
            
            deleteTree(t); // Xóa cây cũ sau khi xe đã chọn
            t = NULL; // Thiết lập t lại thành NULL để tránh trỏ đến địa chỉ đã được giải phóng
        } else if (option == 2) {
            printf("Exiting.....");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

int main() {
    processOption();
    return 0;
}
