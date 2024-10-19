#include "ChartData.h"

namespace Data
{
  using std::size_t;

  Chart::Chart(const char* id, unsigned res, size_t size):
    id(id),
    res(res),
    size(size),
    buffers((size_t)Category::Count, RingBuffer(size))
  {}

  const RingBuffer& Chart::operator[](Category category) const
  {
    return buffers[(size_t)category];
  }

  RingBuffer& Chart::operator[](Category category)
  {
    return buffers[(size_t)category];
  }
}
