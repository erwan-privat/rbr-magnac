#include "ChartData.h"

namespace Data
{
  using std::size_t;

  std::vector<Chart> charts {
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

  const RingBuffer& Chart::at(Category category) const
  {
    return buffers[(size_t)category];
  }

  RingBuffer& Chart::at(Category category)
  {
    return buffers[(size_t)category];
  }
}
