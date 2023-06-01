from fastapi import FastAPI
from pydantic import BaseModel
from ctypes import CDLL, c_int, c_char_p
from fastapi import Response
import ctypes
import json
from fastapi.middleware.cors import CORSMiddleware


main_libc = CDLL("./main_code.so")
api_login_libc = CDLL("./api_login.so")
share_libc = CDLL("./share_task.so")
read_task = CDLL("./read_task.so")

createNew = main_libc.createNewTaskText
createNew.argtypes = [c_int, c_int, c_int, c_int, c_int, c_char_p, c_char_p, c_char_p]
createNew.restype = None  # 반환 타입 설정

origins = ["http://localhost", "http://localhost:8000"]

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/")  # GET 메소드로 가장 루트 url로 접속할 경우
async def root():  # root() 함수를 실행하고
    return {"message": "Hello World"}  # Hello World란 메시지 반환합니다.


import datetime


def split_json_date(json_date):
    dt = datetime.datetime.fromisoformat(json_date)
    year = dt.year
    month = dt.month
    day = dt.day
    hour = dt.hour
    minute = dt.minute
    second = dt.second

    return year, month, day, hour, minute, second


class CreateTaskRequest(BaseModel):
    type: int
    date: str
    title: str
    content: str
    username: str


@app.post("/task", response_model=None)
def createTask(request: CreateTaskRequest):
    year, month, day, hour, minute, second = split_json_date(request.date)
    # date 처리 필요
    createNew(
        request.type,
        year,
        month,
        day,
        hour,
        request.title.encode(),
        request.content.encode(),
        request.username.encode(),
    )

    return Response(status_code=200)


class LoginNameRequest(BaseModel):
    id: str


api_login = api_login_libc.api_login_fun
api_login.argtypes = [c_char_p]
api_login.restype = None


@app.post("/login", response_model=None)
def login(request: LoginNameRequest):
    api_login(request.id.encode())
    return Response(status_code=200)


api_logout = api_login_libc.api_logout_fun
api_logout.argtypes = [c_char_p]
api_logout.restype = None


@app.post("/logout", response_model=None)
def login(request: LoginNameRequest):
    api_logout(request.id.encode())
    return Response(status_code=200)


import re


def replace_directory(string):
    pattern = r"\./(\w+)"
    replacement = r"./Done"
    new_string = re.sub(pattern, replacement, string)
    return new_string


taskToDone = main_libc.taskToDone
taskToDone.argtypes = [c_char_p, c_char_p]
taskToDone.restype = None  # 반환 타입 설정


@app.post("/task/{filepath}/complete")
def complete_task(filepath: str):
    taskToDone(filepath.encode(), replace_directory(filepath).encode())

    return Response(status_code=200)


class ShareRequest(BaseModel):
    username: str
    taskpath: str


shareTask = share_libc.create_link
shareTask.argtypes = [c_char_p, c_char_p]
shareTask.restype = None  # 반환 타입 설정


@app.post("/task/share")
def share_task(request: ShareRequest):
    path = request.taskpath.replace("+", "/")
    shareTask(request.username.encode(), path.encode())

    return Response(status_code=200)


class Task(ctypes.Structure):
    _fields_ = [
        ("time", ctypes.c_char * 13),
        ("filename", ctypes.c_char * 51),
        ("contents", ctypes.c_char * 201),
        ("group_user", ctypes.c_char * 51),
        ("category", ctypes.c_char * 20),
        ("schedule_check", ctypes.c_char * 20),
        ("flag", ctypes.c_char * 5),
    ]


readTask = read_task.read_dir
readTask.argtypes = [c_char_p]
readTask.restype = ctypes.POINTER(Task)


def get_task_list(dir_path):
    result = readTask(dir_path.encode())
    tasks = []

    index = 0
    while True:
        task = result[index]
        if task.filename:
            task_time_decoded = task.time.decode().rstrip("\n")
            datetime_obj = datetime.datetime.fromtimestamp(int(task_time_decoded))
            json_time = datetime_obj.isoformat()
            tasks.append(
                {
                    "title": task.filename.decode(),
                    "content": task.contents.decode(),
                    "date": json_time,
                    "together": task.group_user.decode(),
                    "category": task.category.decode(),
                    "schedule_check": task.schedule_check.decode(),
                }
            )
            index += 1
        else:
            break

    return tasks


@app.get("/tasks/{name}")
def getFamilyTask(name: str):
    tasks = get_task_list(name)
    return {"tasks": tasks}
