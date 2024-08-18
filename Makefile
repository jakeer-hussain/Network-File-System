main:
	gcc headers.h functions.c

add_naming:
	gcc STORAGE_SERVER/headers.h STORAGE_SERVER/functions.c STORAGE_SERVER/naming.c
	./a.out

add_server1:
	gcc STORAGE_SERVER/headers.h STORAGE_SERVER/functions.c STORAGE_SERVER/ss_server.c
	./a.out

add_client1:
	gcc STORAGE_SERVER/headers.h STORAGE_SERVER/functions.c STORAGE_SERVER/client.c
	./a.out

add_server2:
	gcc STORAGE_SERVER/headers.h STORAGE_SERVER/functions.c STORAGE_SERVER/ss_server2.c
	./a.out

add_client2:
	gcc STORAGE_SERVER/headers.h STORAGE_SERVER/functions.c STORAGE_SERVER/client2.c
	./a.out

add_c:
	gcc STORAGE_SERVER/Examples/T_c.c
	./a.out

add_t:
	gcc STORAGE_SERVER/Examples/T_s.c
	./a.out

add_ss1:
	gcc STORAGE_SERVER/Storages/f1/ss1.c STORAGE_SERVER/Storages/f1/functions.c STORAGE_SERVER/Storages/f1/headers.h
	./a.out

add_ss2:
	gcc STORAGE_SERVER/Storages/f2/ss2.c STORAGE_SERVER/Storages/f2/functions.c STORAGE_SERVER/Storages/f2/headers.h
	./a.out
