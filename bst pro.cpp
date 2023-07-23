#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Coordinate {
    double x;
    double y;
};
typedef Coordinate coordinate;

struct ParkingLot {
    struct Coordinate location;
    int capacity;
};
typedef ParkingLot parkinglot;

struct Node {
    parkinglot data;
    struct Node* left;
    struct Node* right;
};

typedef struct Node* node;

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
node insertByDistance(node root, parkinglot parkingLot,coordinate carLocation) {
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

// Hàm duyệt cây BST theo thứ tự inorder và tìm bãi đỗ gần nhất và còn chỗ
void findNearestInorder(node root, coordinate carLocation, double* minDistance, node* nearestParkingLot) {
    if (root == NULL) {
        return;
    }

    findNearestInorder(root->left, carLocation, minDistance, nearestParkingLot);

    double distance = calculateDistance(carLocation, root->data.location);
    if (distance < *minDistance && root->data.capacity > 0) {
        *minDistance = distance;
        *nearestParkingLot = root;
    }

    findNearestInorder(root->right, carLocation, minDistance, nearestParkingLot);
}

// Hàm tìm bãi đỗ gần nhất
node findNearest(node root, coordinate carLocation) {
    double minDistance = INFINITY;
    node nearestParkingLot = NULL;
    findNearestInorder(root, carLocation, &minDistance, &nearestParkingLot);
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
        printf("Option:\n1. Nhap toa do xe\n2. Thoat\n");
        scanf("%d", &option);

        if (option == 1) {
            printf("Nhap toa do xe (x y): ");
            scanf("%lf %lf", &carLocation.x, &carLocation.y);

            node nearestParkingLot = findNearest(root, carLocation);
            if (nearestParkingLot != NULL) {
                printf("Bai do gan nhat co toa do: (%.2lf, %.2lf) va con %d cho trong\n",
                       nearestParkingLot->data.location.x, nearestParkingLot->data.location.y,
                       nearestParkingLot->data.capacity);

                printf("Xac nhan? (1: Co / 0: Khong)? ");
                int choose;
                scanf("%d", &choose);
                if (choose == 1) {
                    if (nearestParkingLot->data.capacity > 0) {
                        nearestParkingLot->data.capacity--;
                        printf("Thanh cong.\n");
                    } else {
                        printf("Bai do da day, vui long tim bai do khac!\n");
                    }
                } else if (choose == 0) {
                    printf(".\n");
                } else {
                    printf("Lua chon khong hop le!\n");
                }
            } else {
                printf("Khong tim thay bai do gan nhat hoac tat ca cac bai do da day!\n");
            }
        } else if (option == 2) {
            break;
        } else {
            printf("Lua chon khong hop le! Vui long chon lai.\n");
        }
    }


}


int main() {
    processOption();
    return 0;
}
