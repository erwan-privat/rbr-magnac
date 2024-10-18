#ifndef CHARTDATA_H
#define CHARTDATA_H

#include "RingBuffer.h"
#include <unordered_map>

namespace Data
{
  enum class Key : unsigned char
  {
    D_24H,
    D_1H,
    D_15MIN,
  };

  enum class Category : unsigned char
  {
    P1_HP,
    P1_HC,
    P2_HP,
    P2_HC,
    Count,
  };

  struct Chart
  {
    const char* id;     // javascript/html id
    const unsigned res; // resolution in seconds
    std::size_t size;   // number of points
    std::vector<RingBuffer> buffers;

    Chart(const char* id, unsigned res, std::size_t size);

    const RingBuffer& operator[](Category category) const;
    RingBuffer& operator[](Category category);
  };

  static inline std::unordered_map<Key, Chart> charts {
    { Key::D_24H,   { "24h", 180, 480 } },
    { Key::D_1H,    { "1h",    8, 450 } },
    { Key::D_15MIN, { "15min", 2, 450 } },
  };
}

#endif /* CHARTDATA_H */
