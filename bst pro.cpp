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

typedef struct Node* node;
node root = NULL;

// Hàm tính khoảng cách từ xe tới 1 bãi đỗ
double calculateDistance(coordinate carLocation, coordinate parkingLotLocation) {
    return sqrt(pow(carLocation.x - parkingLotLocation.x, 2) + pow(carLocation.y - parkingLotLocation.y, 2));
}

// Hàm tạo một node mới
node createNode(parkinglot parkingLot) {
    node newNode = (node)malloc(sizeof(struct Node));
    newNode->data = parkingLot;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Hàm chèn một bãi đỗ vào cây BST theo thứ tự khoảng cách tăng dần
node insertByDistance(node root, parkinglot parkingLot, coordinate carLocation) {
    if (root == NULL) {
        return createNode(parkingLot);
    }

    double distanceNewLot = calculateDistance(carLocation, parkingLot.location);
    double distanceCurrentLot = calculateDistance(carLocation, root->data.location);

    if (distanceNewLot < distanceCurrentLot) {
        root->left = insertByDistance(root->left, parkingLot, carLocation);
    } else {
        root->right = insertByDistance(root->right, parkingLot, carLocation);
    }

    return root;
}

// Hàm tìm bãi đỗ gần nhất và còn chỗ
node findNearest(node root, coordinate carLocation) {
   
    node nearestParkingLot = NULL;

    if (root != NULL) {
        
        if (root->data.capacity > 0) {
            
            nearestParkingLot = root;
        }
		findNearest(root->left, carLocation);
		findNearest(root->right, carLocation);
    }

    return nearestParkingLot;
}

// Hàm xử lý các option
void processOption() {
    struct ParkingLot availableParkingLots[100]; // Giả sử số lượng tối đa bãi đỗ là 100
    int numAvailableParkingLots = 0; // Số lượng bãi đỗ có sẵn hiện tại
    coordinate carLocation;
    int option;

    // Khởi tạo 3 bãi đỗ có sẵn
    availableParkingLots[numAvailableParkingLots].location.x = 1;
    availableParkingLots[numAvailableParkingLots].location.y = 2;
    availableParkingLots[numAvailableParkingLots].capacity = 3;
    numAvailableParkingLots++;

    availableParkingLots[numAvailableParkingLots].location.x = 2;
    availableParkingLots[numAvailableParkingLots].location.y = 9;
    availableParkingLots[numAvailableParkingLots].capacity = 3;
    numAvailableParkingLots++;

    // Tạo BST và thêm bãi đỗ có sẵn vào BST
    node root = NULL;
    for (int i = 0; i < numAvailableParkingLots; i++) {
        root = insertByDistance(root, availableParkingLots[i], carLocation);
    }

    while (1) {
        printf("Option:\n1. Input your coordinate\n2. Exit\n");
        scanf("%d", &option);

        if (option == 1) {
            printf("\nYour coordinate (x y): ");
            scanf("%lf %lf", &carLocation.x, &carLocation.y);

            node nearestParkingLot = findNearest(root, carLocation);
            if (nearestParkingLot != NULL) {
                printf("The neareat has coordinate: (%.2lf, %.2lf) and has %d slot.\n",
                       nearestParkingLot->data.location.x, nearestParkingLot->data.location.y,
                       nearestParkingLot->data.capacity);

                printf("Confirm? (1: YES / 0: NO)? ");
                int choose;
                scanf("%d", &choose);
                if (choose == 1) {
                    if (nearestParkingLot->data.capacity > 0) {
                        nearestParkingLot->data.capacity--;
                        printf("Success!\n");
                    } else {
                        printf("Car park is full. Please check another!\n");
                    }
                } else if (choose == 0) {
                    printf(".\n");
                } else {
                    printf("Ivalid option!\n");
                }
            } else {
                printf("Not found!\n");
            }
        } else if (option == 2) {
        	printf("Exiting.....");
            break;
        } else {
            printf("Ivalid option.\n");
        }
    }

    // Giải phóng bộ nhớ của cây BST
    // Chú ý: Hàm giải phóng bộ nhớ được thêm vào để tránh memory leak
    free(root);
}

int main() {
    processOption();
    return 0;
}
