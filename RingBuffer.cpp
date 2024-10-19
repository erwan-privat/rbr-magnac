#include "RingBuffer.h"

RingBuffer::RingBuffer(std::size_t size)
  : size(size), buffer(size), index(0)
{}

void RingBuffer::push_back(float element)
{
  buffer[index] = element;
  index = (index + 1) % size;
}

float RingBuffer::back() const
{
  return buffer[index];
}
