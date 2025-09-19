#include "support/range/range_table.h"

bool in_uint32_range_table(const uint32_range_t* table, size_t count, uint32_t cp)
{
    size_t left = 0, right = count;
    while(left < right)
    {
        size_t mid = left + (right - left) / 2;
        if(cp < table[mid].start)
        {
            right = mid;
        }
        else if(cp > table[mid].end)
        {
            left = mid + 1;
        }
        else
        {
            return true; // cp is in the range
        }
    }
    return false;
}