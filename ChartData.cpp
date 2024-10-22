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

  const std::vector<const char*> labels
  {
    "p1_hp",
    "p1_hc",
    "p2_hp",
    "p2_hc",
  };

  Chart::Chart(const char* id, unsigned res, size_t size):
    id(id),
    res(res),
    size(size),
    buffers((size_t)Category::Count, RingBuffer(size))
  {}

  const RingBuffer& Chart::ring_for(Category category) const
  {
    return buffers[(size_t)category];
  }

  RingBuffer& Chart::ring_for(Category category)
  {
    return buffers[(size_t)category];
  }

  const char* Chart::label_for(Category category) const
  {
    return labels[(size_t)category];
  }
}
