#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

size_t search(size_t begin, size_t end)
{
    while(begin < end)
    {
      size_t mid = begin + (end - begin) / 2;
      char *m = mmap(NULL, mid, PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);

      if (m == MAP_FAILED)
      {
          end = mid;
      }
      else
      {
          begin = mid + 1;
      }
    }

    return end;
}

int main()
{
    size_t size = 128e16;

    printf("%zu\n", search(0, size));

    return 0;
}
