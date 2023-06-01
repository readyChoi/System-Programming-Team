all: admin api_login.so login.so logout.so main_code.so read_task.so share_task.so

admin: admin_code.c
	gcc -o admin admin_code.c -w -pthread

api_login.so: api_login_code.c
	gcc -shared -o api_login.so api_login_code.c -fPIC

login.so: login_code.c
	gcc -shared -o login.so login_code.c -fPIC

logout.so: logout_code.c
	gcc -shared -o logout.so logout_code.c -fPIC

main_code.so: main_code.c
	gcc -shared -o main_code.so main_code.c -fPIC

read_task.so: read_task_list.c
	gcc -shared -o read_task.so read_task_list.c -fPIC

share_task.so: share_task.c
	gcc -shared -o share_task.so share_task.c -fPIC

clean:
	rm admin api_login.so login.so logout.so main.so read_task.so share_task.so
