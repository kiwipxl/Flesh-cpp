echo --clearing logcat--
call adb logcat -c

echo --starting activity--
call adb shell am start -n com.TempCompany.WormsGame/org.cocos2dx.cpp.AppActivity

echo --logging messages--
call adb logcat | find "cocos2d-x"

pause