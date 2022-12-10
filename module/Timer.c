#include "../collect/Collection.h"

/**
 * Timer线程
 * @param args
 * @return 是否有键盘事件
 */
void *timeThread(void *args) {
    if (timer.currentTimeMills - timer.previousTimeMills >= currentGameMode.speed * speedMultiply) {
        timer.previousTimeMills = GetTickCount();
        timer.currentTimeMills = timer.previousTimeMills;
    } else {
        DWORD delta = timer.currentTimeMills - timer.previousTimeMills;
        timer.currentTimeMills = GetTickCount();
        timer.previousTimeMills = timer.currentTimeMills - delta;
    }
    while (true) {
        if (speedMultiply != 1.0 && GetTickCount() - speedMultiplyEnabledTime >= 10000) {
            speedMultiply = 1.0; //恢复Speed
            if (enablePreview) refreshPreview();
        }
        if (timer.currentTimeMills - timer.previousTimeMills >= currentGameMode.speed * speedMultiply) {
            pthread_exit("true");
            break;
        } else {
            timer.currentTimeMills = GetTickCount();
            if (whenBreakTimer()) {
                pthread_exit("false");
                break;
            }
        }
    }
}