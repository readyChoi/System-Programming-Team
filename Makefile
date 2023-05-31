### Makefile

all: admin api_login.so login.so logout.so  main.so read_task.so share_task.so

admin: admin_code.c
	gcc -o admin admin_code.c -w -pthread

api_login.so: api_login_code.c
	gcc -shared -o api_login.so api_login_code.c

login.so: login_code.c
	gcc -shared -o login.so login_code.c

logout.so: logout_code.c
	gcc -shared -o logout.so logout_code.c

main.so: main_code.c
	gcc -shared -o main.so main_code.c

read_task.so: read_task_list.c
	gcc -shared -o read_task.so read_task_list.c

share_task.so: share_task.c
	gcc -shared -o share_task.so share_task.c
clean:
	rm admin api_login.so login.so logout.so  main.so read_task.so share_task.so
