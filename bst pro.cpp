#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Coordinate {
    double x;
    double y;
};
typedef struct Coordinate coordinate;

struct ParkingLot {
    struct Coordinate location;
    int capacity;
};
typedef struct ParkingLot parkinglot;

struct Node {
    parkinglot data;
    struct Node* left;
    struct Node* right;
};

typedef struct Node* tree;
tree t = NULL;

// Hàm tính khoảng cách từ xe tới 1 bãi đỗ
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

// Hàm chèn một bãi đỗ vào cây BST theo thứ tự khoảng cách tăng dần
tree insertByDistance(tree t, parkinglot parkingLot, coordinate carLocation) {
    if (t == NULL) {
        return createNode(parkingLot);
    }

    double distanceNewLot = Distance(carLocation, parkingLot.location);
    double distanceCurrentLot = Distance(carLocation, t->data.location);

    if (distanceNewLot < distanceCurrentLot) {
        t->left = insertByDistance(t->left, parkingLot, carLocation);
    } else {
        t->right = insertByDistance(t->right, parkingLot, carLocation);
    }

    return t;
}

void inorderTraversal(tree t) {
    if (t != NULL) {
        inorderTraversal(t->left);
        printf("Coordinate: (%.2lf, %.2lf), Capacity: %d\n", t->data.location.x, t->data.location.y, t->data.capacity);
        inorderTraversal(t->right);
    }
}

tree findLeftMostNodeWithCapacity(tree t) {
    if (t == NULL) {
        return NULL;
    }

    tree result = NULL;
    while (t != NULL) {
        if (t->data.capacity > 0) {
            result = t;
            break;
        }
        t = t->left;
    }

    return result;
}
void Free( tree t )
{
    if ( t == NULL )
        return;
    Free( t->left );
    Free( t->right );
    free( t );
}




void processOption() {
    struct ParkingLot carpark[100]; // Giả sử số lượng tối đa bãi đỗ là 100
    int num = 0; // Số lượng bãi đỗ có sẵn 
    
    int option;

    // khởi tại bãi đỗ
    carpark[num].location.x = 2;
    carpark[num].location.y = 2;
    carpark[num].capacity = 0;
    num++;

    carpark[num].location.x = 9;
    carpark[num].location.y = 9;
    carpark[num].capacity = 3;
    num++;

    carpark[num].location.x = 1;
    carpark[num].location.y = 1;
    carpark[num].capacity = 3;
    num++;
    
    carpark[num].location.x = 3;
    carpark[num].location.y = 3;
    carpark[num].capacity = 3;
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

            tree nearest = findLeftMostNodeWithCapacity(t);
            while (nearest != NULL && nearest->data.capacity == 0) {
                nearest = findLeftMostNodeWithCapacity(nearest->right);
            }

            if (nearest != NULL) {
                printf("The nearest parking lot with available slots: Coordinate: (%.2lf, %.2lf), Capacity: %d\n", 
                       nearest->data.location.x, nearest->data.location.y, nearest->data.capacity);

                printf("Confirm? (1: YES / 0: NO)? ");
                int choose;
                scanf("%d", &choose);
                if (choose == 1) {
                    if (nearest->data.capacity > 0) {
                        nearest->data.capacity--;
                        printf("Success!\n");
                    } else {
                        printf("Car park is full. Please check another!\n");
                    }
                 
                } else if (choose == 0) {
                    printf(".\n");
                } else {
                    printf("Invalid option!\n");
                }
            } else {
                printf("No available parking lot found!\n");
            }
        } else if (option == 2) {
            printf("Exiting.....");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
       Free(t);
}


int main() {
    processOption();
    return 0;
}