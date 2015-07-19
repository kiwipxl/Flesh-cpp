#!/usr/bin/python

import sys;
import db;
import socket_manage;
import message;
from client import *;

if __name__ == "__main__":
    db.init();
    socket_manage.init();
    socket_manage.listen("192.168.0.5", 4222);