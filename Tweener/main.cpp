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
                .delay(0.5)                                 // wait 0.5 seconds before starting the tween
                .time(1)                                    // take 1 second to complete the tween
                .type(EaseOutExpo)                          // quick at first, slow towards end
                .param(&value, 25)                          // tween the float stored by value to 25 over the course of the tween
                .onStart(this, &QuickTest::onStart)         // when the tween starts(after the delay timer), call onStart
                .onUpdate(this, &QuickTest::onUpdate)       // when ever the tween changes the value, call onUpdate
                .onComplete(this, &QuickTest::onComplete);  // when the tween reaches the target value of 25, call onComplete
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