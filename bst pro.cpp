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
double Distance(coordinate carLocation, coordinate parkingLotLocation) {
    return sqrt(pow(carLocation.x - parkingLotLocation.x, 2) + pow(carLocation.y - parkingLotLocation.y, 2));
}


// Hàm chèn một bãi đỗ vào cây BST theo thứ tự khoảng cách tăng dần
node insertByDistance(node root, parkinglot parkingLot, coordinate carLocation) {
    if (root == NULL) {
        return root;
    }

    double distanceNewLot = Distance(carLocation, parkingLot.location);
    double distanceCurrentLot = Distance(carLocation, root->data.location);

    if (distanceNewLot < distanceCurrentLot) {
        root->left = insertByDistance(root->left, parkingLot, carLocation);
    } else {
        root->right = insertByDistance(root->right, parkingLot, carLocation);
    }

    return root;
}

// Hàm tìm bãi đỗ gần nhất và còn chỗ
node findNearest(node root, coordinate carLocation) {
   
    node nearest = NULL;

    if (root != NULL) {
        
        if (root->data.capacity > 0) {
            
            nearest = root;
        }
		findNearest(root->left, carLocation);
		findNearest(root->right, carLocation);
    }

    return nearest;
}

// Hàm xử lý các option
void processOption() {
    struct ParkingLot carpark[100]; // Giả sử số lượng tối đa bãi đỗ là 100
    int num = 0; // Số lượng bãi đỗ có sẵn 
    
    int option;

    // khởi tại bãi đỗ
    carpark[num ].location.x = 1;
    carpark[num ].location.y = 2;
    carpark[num ].capacity = 3;
    num ++;

    carpark[num ].location.x = 1;
    carpark[num ].location.y = 2;
    carpark[num ].capacity = 3;
    num ++;


 


    while (1) {
    	coordinate carLocation;
        printf("Option:\n1. Input your coordinate\n2. Exit\n");
        scanf("%d", &option);

        if (option == 1) {
            printf("\nYour coordinate (x y): ");
            scanf("%lf %lf", &carLocation.x, &carLocation.y);
            for (int i = 0; i < num; i++) {
	        root = insertByDistance(root, carpark[i], carLocation);
 		   }

            node nearest = findNearest(root, carLocation);
            if (nearest != NULL) {
                printf("The neareat has coordinate: (%.2lf, %.2lf) and has %d slot.\n",
                       nearest->data.location.x, nearest->data.location.y,
                       nearest->data.capacity);

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


}

int main() {
    processOption();
    return 0;
}
