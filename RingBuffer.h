#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>

// TODO template if needed, for now float is enough
struct RingBuffer
{
  std::size_t size;
  std::vector<float> buffer;
  std::size_t index;

  RingBuffer(std::size_t size);
  void push_back(float element);
  float back() const;
};

#endif /* RINGBUFFER_H */
