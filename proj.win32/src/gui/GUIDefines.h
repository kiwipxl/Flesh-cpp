#ifndef _GUI_DEFINES_H_
#define _GUI_DEFINES_H_

#define BEGIN_GUI_NS    namespace gui {
#define END_GUI_NS      };

#define BEGIN_GAME_NS   namespace game {
#define END_GAME_NS     };

//forward declares begin
BEGIN_GUI_NS
class MessageBox;
typedef std::shared_ptr<MessageBox> MessageBoxPtr;

class Button;
typedef std::shared_ptr<Button> ButtonPtr;

typedef std::function<void()> ButtonClickCallback;

END_GUI_NS

namespace cocos2d {

    class Texture2D;
};
//forward declares end

#endif