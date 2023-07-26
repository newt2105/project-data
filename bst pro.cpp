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

tree insertByDistance(tree t, parkinglot parkingLot, coordinate carLocation) {
    if (t == NULL) {
        if (parkingLot.capacity > 0) {
            return createNode(parkingLot);
        }
        return NULL; // Không chèn nếu capacity = 0
    }

    double distanceNewLot = Distance(carLocation, parkingLot.location);
    double distanceCurrentLot = Distance(carLocation, t->data.location);

    if (distanceNewLot < distanceCurrentLot) {
        // Chèn vào cây bên trái nếu khoảng cách mới nhỏ hơn
        if (parkingLot.capacity > 0) {
            t->left = insertByDistance(t->left, parkingLot, carLocation);
        }
    } else {
        // Chèn vào cây bên phải nếu khoảng cách mới lớn hơn hoặc bằng
        if (parkingLot.capacity > 0) {
            t->right = insertByDistance(t->right, parkingLot, carLocation);
        }
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

tree findLeftMostNode(tree t) {
    if (t == NULL) {
        return NULL;
    }

    // Keep going left until there's no left child left
    while (t->left != NULL) {
        t = t->left;
    }

    return t;
}


void updateCapacity(tree t, coordinate chosenLocation, struct ParkingLot* carpark, int num) {
    if (t == NULL)
        return;
    updateCapacity(t->left, chosenLocation, carpark, num);


            
    // Tìm index của bãi đỗ trong carpark và trừ capacity
    for (int i = 0; i < num; i++) {
        if (carpark[i].location.x == chosenLocation.x && carpark[i].location.y == chosenLocation.y) {
            carpark[i].capacity--;
            break;
        }
    }

    updateCapacity(t->right, chosenLocation, carpark, num);
}

void deleteTree(tree t) {
    if (t == NULL)
        return;
    deleteTree(t->left);
    deleteTree(t->right);
    free(t);
}

void processOption() {
    struct ParkingLot carpark[100]; // Giả sử số lượng tối đa bãi đỗ là 100
    int num = 0; // Số lượng bãi đỗ có sẵn 

    int option;

    // Khởi tạo bãi đỗ ban đầu
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
