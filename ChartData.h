#ifndef CHARTDATA_H
#define CHARTDATA_H

#include "RingBuffer.h"
#include <unordered_map>

namespace Data
{
  enum class Category : unsigned char
  {
    P1_HP,
    P1_HC,
    P2_HP,
    P2_HC,
    Count,
  };

  // const std::unordered_map<Category, const char*> CatNames
  // {
  //   { P1_HP, "p1_hp" },
  //   { P1_HC, "p1_hc" },
  //   { P2_HP, "p2_hp" },
  //   { P2_HC, "p2_hc" },
  // };

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

  extern const std::vector<Chart> charts;
}

#endif /* CHARTDATA_H */
