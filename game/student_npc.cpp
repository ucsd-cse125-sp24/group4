#include "../include/student_npc.h"

MovingStudent::MovingStudent(float x, float y, float z, float sideLength, float speed)
    : Student(x, y, z), sideLength(sideLength), currentSegment(0), moveProgress(0.0f), speed(speed) {}

void MovingStudent::update(float deltaTime) {
    float distance = speed * deltaTime;
    moveProgress += distance;

    if (moveProgress >= sideLength) {
        moveProgress -= sideLength;
        currentSegment = (currentSegment + 1) % 4;
    }

    switch (currentSegment) {
        case 0:
            x += distance;
            break;
        case 1:
            y += distance;
            break;
        case 2:
            x -= distance;
            break;
        case 3:
            y -= distance;
            break;
    }
}
