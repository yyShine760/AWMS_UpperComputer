#ifndef CIRCULARQUEUE_HPP
#define CIRCULARQUEUE_HPP

#include <QVector>

template <typename T>
class CircularQueue {
public:
    CircularQueue(int maxSize = 10);
    void enqueue(const T& value);
    T dequeue();
    bool isEmpty() const;
    bool isFull() const;

private:
    QVector<T> queue;
    int maxSize;
    int front;
    int rear;

    void resizeQueue();
};

template <typename T>
CircularQueue<T>::CircularQueue(int maxSize) : maxSize(maxSize), front(0), rear(0) {
    queue.resize(maxSize);
}

template <typename T>
void CircularQueue<T>::enqueue(const T& value) {
    if (isFull()) {
        resizeQueue();
    }
    queue[rear] = value;
    rear = (rear + 1) % maxSize;
}

template <typename T>
T CircularQueue<T>::dequeue() {
    if (isEmpty()) {
        return T();
    }
    T value = queue[front];
    front = (front + 1) % maxSize;
    return value;
}

template <typename T>
bool CircularQueue<T>::isEmpty() const {
    return front == rear;
}

template <typename T>
bool CircularQueue<T>::isFull() const {
    return (rear + 1) % maxSize == front;
}

template <typename T>
void CircularQueue<T>::resizeQueue() {
    int currentSize = queue.size();
    queue.resize(currentSize * 2);
    if (rear < front) {
        for (int i = 0; i < front; ++i) {
            queue[currentSize + i] = queue[i];
        }
        rear = currentSize + rear;
    }
    maxSize *= 2;
}

#endif // CIRCULARQUEUE_HPP
