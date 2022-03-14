#include "LruStack.h"

int LruStack::getLru() {
    return priority.empty() ? -1 : priority.back();
}

void LruStack::setMru(int n) {
    priority.remove(n);
    priority.push_front(n);
}

int LruStack::getSize() {
    return priority.size();
}