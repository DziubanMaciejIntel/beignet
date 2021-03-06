#include "utest_helper.hpp"

void compiler_mul24(void)
{
  const int n = 32;
  int src1[n], src2[n];

  // Setup kernel and buffers
  OCL_CREATE_KERNEL("compiler_mul24");
  OCL_CREATE_BUFFER(buf[0], 0, n * sizeof(int), NULL);
  OCL_CREATE_BUFFER(buf[1], 0, n * sizeof(int), NULL);
  OCL_CREATE_BUFFER(buf[2], 0, n * sizeof(int), NULL);
  OCL_SET_ARG(0, sizeof(cl_mem), &buf[0]);
  OCL_SET_ARG(1, sizeof(cl_mem), &buf[1]);
  OCL_SET_ARG(2, sizeof(cl_mem), &buf[2]);
  globals[0] = n;
  locals[0] = 16;

  OCL_MAP_BUFFER(0);
  OCL_MAP_BUFFER(1);
  for (int i = 0; i < n; ++i) {
    src1[i] = ((int*)buf_data[0])[i] = rand();
    src2[i] = ((int*)buf_data[1])[i] = rand();
  }
  OCL_UNMAP_BUFFER(0);
  OCL_UNMAP_BUFFER(1);

  OCL_NDRANGE(1);

  OCL_MAP_BUFFER(2);
  for (int i = 0; i < n; ++i)
    OCL_ASSERT(((int*)buf_data[2])[i] == (src1[i]) * (src2[i]));
  OCL_UNMAP_BUFFER(2);
}

MAKE_UTEST_FROM_FUNCTION(compiler_mul24);
