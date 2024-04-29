#ifndef STUDENT_NPC_H
#define STUDENT_NPC_H

class Student {
public:
    float x, y, z; // Position in 3D space

    // Constructor
    Student(float x, float y, float z) : x(x), y(y), z(z) {}

    // Virtual update function
    virtual void update(float deltaTime) {
        // Default implementation does nothing (for static students)
    }

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Student() {}
};

class StaticStudent : public Student {
public:
    StaticStudent(float x, float y, float z) : Student(x, y, z) {}
};

class MovingStudent : public Student {
public:
    float sideLength;
    int currentSegment;
    float speed;
    float moveProgress;

    MovingStudent(float x, float y, float z, float sideLength, float speed);
    void update(float deltaTime) override;
};

#endif