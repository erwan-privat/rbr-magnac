#include "ChartData.h"

namespace Data
{
  using std::size_t;

  const std::vector<Chart> charts
  {
    // { id, resolution in seconds, number of points }
    { "24h", 180, 480 },
    { "1h",    8, 450 },
    { "15min", 2, 450 },
  };

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
