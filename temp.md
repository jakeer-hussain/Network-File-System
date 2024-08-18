# Assumptions
- Storage Server Acts as a TCP_Client. Because it is not mandatory that ssx communicate to ssy.
- Naming Server acts as a TCP

# Functions To Implement
- Writing a file/folder
- Reading a file
- Deleting a file/folder
- Creating a file/folder
- Listing all the files in all the storage servers
- Getting Additional Information About the file

### Naming Server -------- port 5000 ------- Client server
### Storage Server ------- port 3000 ------- Client Server
### Naming Server -------- port 4000 ------- Storage Server

#### We need Requirements
Server_sock
Client_sock



# Network File System (NFS) Implementation Readme

## Assumptions
1. Naming server can connect with multiple clients and servers.
2. Clients can connect to a single naming server or a single storage server.
3. Storage server can connect with a single client or a single naming server.

## Basic Idea
- **Naming Server to Client Connection:**
  - Naming server acts as a server, handling file requests (create and delete) from clients.

- **Storage Server to Client Connection:**
  - Storage server acts as a server, managing file accessing requests (get, create, delete, copy) from clients.

- **Naming Server to Storage Server Connection:**
  - Naming server acts as a server, and storage server acts as a client.
  - Requests for file create and delete from clients are sent from the naming server to the storage server.

## Major Components
### Clients
- Represent systems or users requesting access to files within the network file system.
- Primary interface for interacting with NFS, initiating various file-related operations.

### Naming Server
- Central hub orchestrating communication between clients and storage servers.
- Provides clients with crucial information about the specific storage server where a requested file or folder resides.
- Acts as a directory service, ensuring efficient and accurate file access.

### Storage Servers
- Responsible for physical storage and retrieval of files and folders.
- Manage data persistence and distribution across the network.

## File Operations Available to Clients
1. **Writing a File/Folder:**
   - Actively create and update the content of files and folders within NFS.

2. **Reading a File:**
   - Empowers clients to retrieve the contents of files stored within NFS.

3. **Deleting a File/Folder:**
   - Clients can remove files and folders from the network file system.

4. **Creating a File/Folder:**
   - NFS allows clients to generate new files and folders, facilitating expansion and organization.

5. **Listing All Files and Folders in a Folder:**
   - Clients can retrieve comprehensive listings of files and subfolders within a specified directory.

6. **Getting Additional Information:**
   - Clients can access supplementary information about specific files, including size, access rights, timestamps, and other metadata.

## Code Implementation Explanation

### `storage_server.c`
The `storage_server.c` file implements the functionalities of a storage server in the NFS system. Key functions include:
- `Reading`: Reads data from a specified file path.
- `Writing`: Writes data to a specified file path.
- `Creation`: Creates a file or folder based on the provided token.
- `Deletion`: Deletes a file or folder based on the provided token.
- `Client_connect`: Handles the connection with clients, processes client requests, and responds accordingly.
- The `main` function initializes file data, creates threads for the naming server and client connections, and joins threads.

### `naming_server.c`
The `naming_server.c` file implements the functionalities of the naming server in the NFS system. Key functions include:
- `Select_what_to_send`: Selects data to send based on the specified port.
- `Update_message`: Updates the message array for communication.
- `Initialize_message_array`: Initializes the message array for communication.
- `Add_Data`: Adds storage server data to the global array.
- `Print_Data`: Prints the current state of the files array.
- `Find_Location`: Finds the location of a specified string in the array.
- `Update_Ports_Availability`: Updates the availability status of ports.
- `Storage_server`: Handles the connection with storage servers, processes requests, and updates server availability.

### `clients.c`
The `clients.c` file implements the functionalities of clients in the NFS system. Key functions include:
- `Print`: Prints the files array.
- The `main` function establishes a connection with the naming server, sends client requests, and communicates with storage servers based on the operation.

### `functions.c`
The `functions.c` file contains common functions used in the NFS implementation. Key functions include:
- `Establish_Connection_as_Server`: Establishes a connection as a server.
- `Accept_Connection`: Accepts a connection.
- `Establish_Connection_as_Client`: Establishes a connection as a client.

This Readme provides an overview of the NFS codebase, highlighting key functionalities, data structures, and interactions between components. Developers can explore each source file for a deeper understanding and make modifications to enhance the system further.
