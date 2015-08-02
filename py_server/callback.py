import debug;
import _MID;

NONE = 0;
UNIQUE_ID = 1;
MID = 2;
MID_LOOP = 3;

class MessageCallback:

    func = None;
    id = 0;
    mid = _MID.UNKNOWN;
    type = NONE;
    num_callbacks_left = 0;