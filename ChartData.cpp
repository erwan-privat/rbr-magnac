#include "ChartData.h"

namespace Data
{
  using std::size_t;

  Map charts = {
    { Key::D_24H,   { "24h", 180, 480 } },
    { Key::D_1H,    { "1h",    8, 450 } },
    { Key::D_15MIN, { "15min", 2, 450 } },
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
