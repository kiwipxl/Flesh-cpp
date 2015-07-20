#!/usr/bin/python

import sys;
import db;
import socket_manage;
import message;
from client import *;

if __name__ == "__main__":
    db.init();
    socket_manage.init();
    socket_manage.socket_loop("0.0.0.0", 4222);