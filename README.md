# Network File System (NFS) Readme

## Assumptions

1. **Naming Server Connections:**
   - Naming server can connect with multiple clients and servers.
   - Clients connect to the naming server on port 5000.
   - Storage servers connect to the naming server on port 4000.

2. **Client Connections:**
   - Clients can connect to a single naming server on port 5000.
   - Clients can also connect to a single storage server on port 3000.

3. **Storage Server Connections:**
   - Storage servers can connect to a single client on port 3000.
   - Storage servers can also connect to a single naming server on port 4000.

## Basic Idea

1. **Naming Server - Client Connection:**
   - Naming server acts as the server.
   - File requests (create and delete) are sent from the client to the naming server on port 5000.

2. **Storage Server - Client Connection:**
   - Storage server acts as the server.
   - File accessing requests (get, create, delete, copy) are sent from the client to the storage server on port 3000.

3. **Naming Server - Storage Server Connection:**
   - Naming server acts as the server.
   - Storage server acts as the client.
   - Requests for file creation and deletion from the client are sent from the naming server to the storage server on port 4000.

## Major Components

The NFS architecture consists of three major components:

1. **Clients:**
   - Represent systems or users requesting access to files within the network file system.
   - Primary interface for interacting with NFS.
   - Initiate file-related operations such as reading, writing, deleting, etc.

2. **Naming Server:**
   - Central hub orchestrating communication between clients and storage servers.
   - Provides clients with information about the specific storage server where a requested file resides.
   - Acts as a directory service, ensuring efficient and accurate file access.

3. **Storage Servers:**
   - Responsible for physical storage and retrieval of files and folders.
   - Manage data persistence and distribution across the network.
   - Ensure secure and efficient file storage.

## File Operations

Clients in the NFS ecosystem can perform various file operations:

1. **Writing a File/Folder:**
   - Actively create and update the content of files and folders.
   - Ensures dynamic repository.

2. **Reading a File:**
   - Retrieve the contents of files stored within the NFS.
   - Grants clients access to the information they seek.

3. **Deleting a File/Folder:**
   - Remove files and folders from the network file system.
   - Contributes to efficient space management.

4. **Creating a File/Folder:**
   - Generate new files and folders.
   - Facilitates the expansion and organization of the file system.

5. **Listing All Files and Folders in a Folder:**
   - Retrieve comprehensive listings of files and subfolders within a specified directory.
   - Aids in efficient file system exploration and management.

6. **Getting Additional Information:**
   - Access supplementary information about specific files.
   - Includes details such as file size, access rights, timestamps, and other metadata.

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

