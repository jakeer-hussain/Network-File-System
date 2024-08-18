#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <dirent.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/stat.h>


#define Kilo_byte 1024
#define EXIT_CODE "###"
#define MAX_SS_CONNECTS 10
#define TESTING "KV"
#define MAX_FILES_DATA 100
#define MAX_FILES_IN_SS 5
#define MAX_PATH 270
#define READ_PERMIT 200
#define WRITE_PERMIT 300
#define READ_WRITE 400

#define UNUSED 1
#define USED 0

struct SS_files {
    char path[Kilo_byte];
    int permissions;
    int size;
};

typedef struct SS_files serverfiles;
typedef serverfiles* storage_files;

struct input {
    char Operation[10];
    char path[Kilo_byte];
};

typedef struct input client_input;
typedef client_input* ptrinput;

struct get_files {
    char file_path[100];
};

typedef struct get_files printfiles;
typedef printfiles* getfiles;

struct Connection_Details {
    int port;
    char IP_address[15];
    int Server_socket;
    int Client_socket;
    int Comm_socket;
};

typedef struct Connection_Details Connection_Details;

struct Storage_server_data {
    int port_number;
    char Location[Kilo_byte];
    // bool Working;
};

struct Port_message {
    int port_number;
    char Message[Kilo_byte];
    int data_look;
};

typedef struct Port_message Port_message;
typedef Port_message* Ptr_message;
typedef struct Storage_server_data Storage_server_data;
typedef struct Storage_server_data* PtrSS_Array;

Connection_Details Establish_Connection_as_Server (Connection_Details Details);

Connection_Details Establish_Connection_as_Client (Connection_Details Details);

Connection_Details Accept_Connection (Connection_Details Details);

void Close_connection (Connection_Details Details);

void Add_Data(Storage_server_data Data);

int Find_Location (char* string);

void Update_Ports_Availability(int port, bool Status);
void Print_Data();