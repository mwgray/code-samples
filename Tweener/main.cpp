#include <iostream>
#include <iomanip>

using namespace std;

#include "Tweener/Tweener.h"
using namespace core;

auto globalTime = 0.0f;

class QuickTest {
public:
    bool isComplete;

    QuickTest() {
        isComplete = false;
        value = 200.0f;

        cout << "At time: " << globalTime << ", tween was created: " << value << endl;

        Tweener.addTween("TestTween")
                .delay(0.5)
                .time(1)
                .type(EaseOutExpo)  // quick at first, slow towards end
                .param(&value, 25)
                .onStart(this, &QuickTest::onStart)
                .onUpdate(this, &QuickTest::onUpdate)
                .onComplete(this, &QuickTest::onComplete);
    }

    void onStart() {
        cout << "At time: " << globalTime << ", Tween has started: " << value << endl;
    }

    void onUpdate() {
        cout << "At time: " << globalTime << ", Tween has updated: " << value << endl;
    }

    void onComplete() {
        cout << "At time: " << globalTime << ", Tween has completed: " << value << endl;
        isComplete = true;
    }

    float value;
};

int main() {

    QuickTest test;

    auto deltaSeconds = 1.0f/60.0f;

    cout << std::fixed << std::setw( 3 ) << std::setprecision( 3 );

    while(!test.isComplete) {

        globalTime += deltaSeconds;

        Tweener.update(deltaSeconds);

    }

    return 0;
}