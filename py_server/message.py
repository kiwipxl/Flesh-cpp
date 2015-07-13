import sys;

class Enum(tuple): __getattr__ = tuple.index;

if (sys.version_info > (3,)):
    buffer = memoryview;

MSG_ID = Enum(["UNKNOWN", "USER_ID", "SEND_USER_PASS"]);

class Message:
    msg_id = MSG_ID.UNKNOWN;

msg = Message();
print(str(msg.msg_id) + ", " + str(MSG_ID[msg.msg_id]));
