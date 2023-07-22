#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Khai báo cấu trúc Node đại diện cho một bãi đỗ xe
struct Node {
    double latitude; // Tọa độ vĩ độ của bãi đỗ xe
    double longitude; // Tọa độ kinh độ của bãi đỗ xe
    int capacity; // Số chỗ đỗ còn trống trong bãi đỗ xe
    double distance; // Khoảng cách từ xe đến bãi đỗ xe
    struct Node* left; // Con trỏ trỏ tới cây con bên trái của nút hiện tại
    struct Node* right; // Con trỏ trỏ tới cây con bên phải của nút hiện tại
};
typedef struct Node* node;

// Hàm tạo một nút mới với thông tin tọa độ, sức chứa và khoảng cách
node newNode(double latitude, double longitude, int capacity) {
    node temp = (node)malloc(sizeof(struct Node));
    temp->latitude = latitude;
    temp->longitude = longitude;
    temp->capacity = capacity;
    temp->distance = 0; // Khởi tạo khoảng cách ban đầu là 0
    temp->left = temp->right = NULL; // Khởi tạo cây con là NULL
    return temp;
}

// Con trỏ root đại diện cho cây tìm kiếm nhị phân chứa thông tin các bãi đỗ xe
node root = NULL;


// Hàm tính khoảng cách giữa hai điểm dựa trên tọa độ địa lý
double calculateDistance(double x1, double y1, double x2, double y2) {
    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance;
}


// Hàm chèn một bãi đỗ xe mới vào cây tìm kiếm nhị phân dựa vào khoảng cách
node insert(node root, double latitude, double longitude, int capacity) {
    if (root == NULL) {
        return newNode(latitude, longitude, capacity);
    }

    // Tính khoảng cách từ bãi đỗ xe mới tới nút hiện tại trong cây
    double distance = calculateDistance(latitude, longitude, root->latitude, root->longitude);

    if (distance < root->distance) {
        root->left = insert(root->left, latitude, longitude, capacity);
    } else {
        root->right = insert(root->right, latitude, longitude, capacity);
    }

    return root;
}


// Hàm cập nhật lại khoảng cách từ xe đến các bãi đỗ xe trong cây
node updateDistances(node root, double latitude, double longitude) {
    if (root == NULL) {
        return NULL;
    }

    // Tính khoảng cách từ xe đến bãi đỗ xe hiện tại
    double distance = calculateDistance(latitude, longitude, root->latitude, root->longitude);
    root->distance = distance;

    // Cập nhật lại khoảng cách cho cây con bên trái và cây con bên phải của nút hiện tại
    root->left = updateDistances(root->left, latitude, longitude);
    root->right = updateDistances(root->right, latitude, longitude);

    return root;
}

// Tìm bãi đỗ xe gần nhất và có chỗ trống để đặt chỗ
node findNextAvailableParking(node root, double latitude, double longitude) {
    if (root == NULL) {
        return NULL;
    }

    // Khởi tạo thông tin bãi đỗ xe gần nhất và khoảng cách tới nó
    node nearestAvailable = NULL;
    double minDistance = -1;

    node current = root;
    while (current != NULL) {
        // Tính khoảng cách từ xe đến bãi đỗ xe hiện tại
        double distance = calculateDistance(latitude, longitude, current->latitude, current->longitude);

        // Nếu bãi đỗ xe có chỗ trống và khoảng cách nhỏ hơn khoảng cách hiện tại, cập nhật thông tin
        if (current->capacity > 0 && (nearestAvailable == NULL || distance < minDistance)) {
            nearestAvailable = current;
            minDistance = distance;
        }

        // Tiếp tục duyệt cây bên trái nếu khoảng cách nhỏ hơn khoảng cách hiện tại, ngược lại duyệt cây bên phải
        if (distance < current->distance) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nearestAvailable;
}

// Hàm cập nhật lại sức chứa của bãi đỗ xe sau khi đặt chỗ thành công
void updateCapacity(node parking) {
    parking->capacity -= 1;
}

// Hàm xử lý đặt chỗ bãi đỗ xe dựa vào tọa độ của xe
void handleReservation(double latitude, double longitude) {
    // Cập nhật lại khoảng cách từ xe đến các bãi đỗ xe trong cây
    root = updateDistances(root, latitude, longitude);
    // Chèn bãi đỗ xe mới vào cây dựa vào khoảng cách đã cập nhật
    root = insert(root, latitude, longitude, 0);

    // Tìm bãi đỗ xe gần nhất và có chỗ trống để đặt chỗ
    node nearestAvailableParking = findNextAvailableParking(root, latitude, longitude);

    if (nearestAvailableParking != NULL) {
        // In ra thông tin bãi đỗ xe gần nhất và số chỗ trống còn lại
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
                // Nếu chọn xác nhận, cập nhật lại sức chứa của bãi đỗ xe
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

// Hàm hiển thị menu chính cho người dùng
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

            // Xử lý đặt chỗ bãi đỗ xe dựa vào tọa độ của xe
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

// Hàm hiển thị màn hình chào mừng và khởi tạo các bãi đỗ xe ban đầu
void welcomeScreen() {
    root = insert(root, 1, 3, 0);
    root = insert(root, 3, 4, 2);
    root = insert(root, 12.345, 23.567, 10);
    root = insert(root, 9.876, 18.234, 0);
    root = insert(root, 11.234, 21.567, 3);

    // Hiển thị menu chính cho người dùng
    mainMenu();
}

int main() {
    // Hiển thị màn hình chào mừng và khởi tạo các bãi đỗ xe ban đầu
    welcomeScreen();
    return 0;
}
