#include "headers.h"

PtrSS_Array G_SS_array;
int Array_index = 0;

struct TrieNode *root;

bool Array[10000];

Port_message messages[MAX_SS_CONNECTS];

sem_t SS_Lock;
sem_t port_array;
sem_t message_array_lock;
sem_t update_lock;

char* Select_what_to_send(int port)
{
  sem_wait(&message_array_lock);
  char* retstr = EXIT_CODE;
  for(int i=0;i<MAX_SS_CONNECTS;i++){
    if(messages[i].port_number == port){
      retstr = messages[i].Message;
      break;
    }
  }
  sem_post(&message_array_lock);
  return retstr;
}


void update_message(char* message, int port)
{
  for(int i=0;i<MAX_SS_CONNECTS;i++){
    if(messages[i].port_number == port && messages[i].data_look == USED && strcpy(messages[i].Message, TESTING) != 0){
      bzero(messages[i].Message, Kilo_byte);
      strcpy(messages[i].Message,message);
      break;
    }
    else if(messages[i].port_number == port && messages[i].data_look == UNUSED )
    {
      messages[i].data_look = USED;
      break;
    }
  }
}

void initialize_message_array()
{
  for(int i=0; i<MAX_SS_CONNECTS; i++)
  {
    messages[i].port_number = 4000+i;
    strcpy(messages[i].Message,EXIT_CODE);
    messages[i].data_look = USED;
  }
}

void Add_Data(Storage_server_data Data)
{
    sem_wait(&SS_Lock);
    G_SS_array[Array_index] = Data;
    Array_index++;
    sem_post(&SS_Lock);
}

void Print_Data()
{
  printf(" ---- Files array Right Now ----\n");
  for (int i = 0; i < Array_index; i++)
  {
    printf("Port = %d | Location = %s | Working = %d \n",G_SS_array[i].port_number, G_SS_array[i].Location, Array[G_SS_array[i].port_number]);
  }
  printf("==============================================================\n\n");
}

int Find_Location (char* string){
  int retval = 0;
  sem_wait(&SS_Lock);
  for(int i=0;i<Array_index;i++){
    if(strncmp(G_SS_array[i].Location,string,strlen(G_SS_array[i].Location)) == 0 && Array[i] == true){
      retval = G_SS_array[i].port_number;
      break;
    }
  }
  sem_post(&SS_Lock);
  return retval;
}
void Update_Ports_Availability(int port, bool Status)
{
  sem_wait(&SS_Lock);
  Array[port] = Status;
  sem_post(&SS_Lock);
}




void *storage_server(void *arg)
{

  // storage server
  Connection_Details Details;
  strcpy(Details.IP_address, "127.0.0.1");
  int some = *(int *)arg;
  Details.port = some + 4000;
  // Details.port = 4001;

  Details = Establish_Connection_as_Server(Details);
  printf("connection Done %d\n", Details.port);

  char buffer[1024];
  while (1)
  {
    printf("Searching ...\n");
    Details = Accept_Connection(Details);
    printf("### Storage Server Found on some port\n");


    // Recieving Path from 
    // int size;
    // recv(Details.Comm_socket, &size, sizeof(size), 0);
    // send(Details.Comm_socket, "Hello", 5, 0);
    // serverfiles file_array[MAX_FILES_DATA];
    // storage_files po = file_array;
    // recv(Details.Comm_socket, po, sizeof(file_array) * MAX_FILES_IN_SS, 0);
    // printf("EXECUTED\n");

    
    // Storing the Port and the Path in a Dummy Variable
    // Add_Data_to_tries(file_array);


    Update_Ports_Availability(Details.port, true);
    // Print_Data();

    //testing place
    update_message("Hello World", Details.port);
    
    while (1)
    {
      bzero(buffer, 1024);
      // sem_wait(&update_lock);
      // printf("Hukka\n");
      strcpy(buffer, Select_what_to_send(Details.port)); // Here we are sending the port number of Storage Server to the Client
      update_message(TESTING, Details.port);
      // sem_post(&update_lock);
      send(Details.Comm_socket, buffer, strlen(buffer), 0);
      // printf("Hukka\n");
      printf("[->]: %s\n", buffer);

      if (strcmp(buffer, EXIT_CODE) == 0)
      {
        close(Details.Comm_socket);
        break;
      }

      fd_set read_fds;
      struct timeval timeout;
      FD_ZERO(&read_fds);
      FD_SET(Details.Client_socket, &read_fds);

      timeout.tv_sec = 5;
      timeout.tv_usec = 0;

      if(select(Details.Client_socket + 1, &read_fds, NULL, NULL, &timeout)<0)
      {
        close(Details.Client_socket);
        break;
      }

      recv(Details.Comm_socket, buffer, sizeof(buffer), 0);
    }

    update_message(EXIT_CODE, Details.port);
    Update_Ports_Availability(Details.port, false);
  printf("[+]Storage server disconnected.\n\n");
  }
}





void *client_connect(void *arg)
{
  // client connection
  Connection_Details Details;
  strcpy(Details.IP_address, "127.0.0.1");
  Details.port = 5000 + *(int*)arg;   // assigns port according to the thread

  Details = Establish_Connection_as_Server(Details);
  printf("--- Server for the Client has been created at the port : %d ---\n", Details.port);
  char buffer[Kilo_byte];
  client_input Input;                   // Taking[storing] client requests
  ptrinput pointer = &Input;            // Pointer to client_input,used for recv,send

  while (1)
  {
    Details = Accept_Connection(Details);
    printf("--- Client Came at port : %d ---\n", Details.port);

    bzero(buffer, 1024);
    recv(Details.Comm_socket, pointer, sizeof(Input), 0);
    printf("[<-]: %s ::: %s\n", Input.Operation,Input.path);
    
    if(strcmp(Input.Operation,"READ") == 0 || strcmp(Input.Operation,"WRITE") == 0){
      // int port = Find_Location(Input.path);
      int port = 3000;
      send(Details.Comm_socket, &port, sizeof(int) , 0);
      printf("--- Send to the Client ---\n");
    }
    else if(strcmp(Input.Operation, "CREATE") == 0 || strcmp(Input.Operation, "DELETE") == 0)
    {
      int port = 4000;
      char temper[1000] = "";
      strcat(temper, Input.Operation);
      strcat(temper, " ");
      strcat(temper, Input.path);
      // sem_wait(&update_lock);
      update_message(temper, port);
      while(strcmp(Select_what_to_send(4000), TESTING)!=0)
      {
        sleep(1);
      }
      char buff[20] = "Operation Done"; 
      send(Details.Comm_socket, buff, strlen(buff) , 0);
      // sem_post(&update_lock);
    }

    else if(strcmp(Input.Operation, "COPY") == 0)
    {
      // Copy Operation
      char BufferX[1000];
      send(Details.Comm_socket, "Hey", 3, 0);
      recv(Details.Comm_socket, BufferX, sizeof(BufferX), 0);
      char* token = strtok(BufferX, " ");
      char src[100];
      strcpy(src, token);
      token = strtok(NULL, " ");
      char des[100];
      strcpy(des, token);

      char Source[1000];
      // strcpy(Source, "COPY_READ#");

      int sport = 4000, dport = 4001;

      char str[20];
      sprintf(Source, "COPY_READ#%d#%s#%s", dport, src, des);
      // sprintf(str, "%d#", dport);
      // strcat(Source, str);
      // strcat(Source, src);
      // strcat(Source, "#");
      // strcat(Source, des);    // COPY_READ # port # src_path # des_path

      char Destination[1000]; // COPY_WRITE # port # des # src
      sprintf(Destination, "COPY_WRITE#%d#%s#%s", sport, src, des);
      
      update_message(Source, sport);
      sleep(1);
      update_message(Destination, dport);

    }

    close(Details.Comm_socket);
    // printf("[+]Client disconnected.\n\n");
  }
}

int main()
{
  root = createNode("root", 0, ACCESIBLE);
  initialize_message_array();

  G_SS_array = (PtrSS_Array) malloc (sizeof(Storage_server_data) * MAX_SS_CONNECTS);

  sem_init(&SS_Lock,0,1);
  sem_init(&port_array,0,1);
  sem_init(&message_array_lock, 0, 1);
  sem_init(&update_lock, 0, 1);
  pthread_t ss[2], client[2];
  int a[2];
  for(int i=0;i<2;i++){
    a[i] = i;
    int *p1 = &a[i];
    pthread_create(&ss[i], NULL, storage_server, p1);
    pthread_create(&client[i], NULL, client_connect, p1);
  }
  for(int i=0;i<2;i++){
    pthread_join(ss[i], NULL);
    pthread_join(client[i], NULL);
  }
  return 0;
}
