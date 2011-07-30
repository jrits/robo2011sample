#include <cstdio>
#include <cassert>
#include "RingBuffer.hpp"
// /usr/bin/g++ TestRingBuffer.cpp


// �P�̃e�X�g�p�f�[�^
#define SIZE 3
static int buf[SIZE] = {0};

// �P�̃e�X�g�������֐�(�e�X�g���\�b�h���ɌĂ΂��)
void testRingBuffer_setUp()
{
    for (int i = 0; i < SIZE; i++) {
        buf[i] = 0;
    }
}

// �P�̃e�X�g�I���֐�(�e�X�g���\�b�h���ɌĂ΂��)
void testRingBuffer_tearDown()
{
}

void testRingBuffer_pushAndGet()
{
    testRingBuffer_setUp();
    RingBuffer<int> buffer(buf, SIZE);
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    assert(buffer.get(0) == 3);
    assert(buffer.get(-1) == 2);
    assert(buffer.get(-2) == 1);
    assert(buffer.get(-3) == 3);
    assert(buffer.get(1) == 1);
    assert(buffer.get(2) == 2);
    assert(buffer.get(3) == 3);
    buffer.push(4);
    assert(buffer.get(0) == 4);
    assert(buffer.get(-1) == 3);
    assert(buffer.get(-2) == 2);
    assert(buffer.get(-3) == 4);
    testRingBuffer_tearDown();
}

int main()
{
    testRingBuffer_pushAndGet();
    printf("OK!\n");
}
