/* 
 * Copyright © 2012 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Segovia <benjamin.segovia@intel.com>
 */

#include "utest_helper.hpp"

static void compiler_copy_buffer_row(void)
{
  uint32_t *src_buffer = NULL;
  int *data_buffer = NULL;
  const int row = 8192;
  const int row_n = 2;
  const int n =  row * row_n;
  int status = 0;

  CALL (cl_test_init, "test_copy_buffer_row.cl", "test_copy_buffer_row", SOURCE);

  // Create the input data
  src_buffer = (uint32_t *) malloc(sizeof(uint32_t) * n);
  for (int32_t i = 0; i < n; ++i) src_buffer[i] = i;
  data_buffer = (int *) malloc(sizeof(int) * 2);
  data_buffer[0] = row;
  data_buffer[1] = n;

  // Allocate all buffers
  OCL_CREATE_BUFFER(buf[0], CL_MEM_COPY_HOST_PTR, n * sizeof(uint32_t), src_buffer);
  OCL_CREATE_BUFFER(buf[1], 0, n * sizeof(uint32_t), NULL);
  OCL_CREATE_BUFFER(buf[2], CL_MEM_COPY_HOST_PTR, 2 * sizeof(uint32_t), data_buffer);
  free(src_buffer);
  free(data_buffer);

  // Run the code
  OCL_SET_ARG(0, sizeof(cl_mem), &buf[0]);
  OCL_SET_ARG(1, sizeof(cl_mem), &buf[1]);
  OCL_SET_ARG(2, sizeof(cl_mem), &buf[2]);
  globals[0] = n;
  locals[0] = 16;
  OCL_NDRANGE(1);

  // Check results
  OCL_MAP_BUFFER(0);
  OCL_MAP_BUFFER(1);
  for (int32_t i = 0; i < n; ++i)
    assert(((uint32_t*)buf_data[0])[i] == ((uint32_t*)buf_data[1])[i]);
  OCL_UNMAP_BUFFER(0);
  OCL_UNMAP_BUFFER(1);
error:
  cl_release_buffers();
  cl_report_error(status);
  cl_test_destroy();

}

UTEST_REGISTER(compiler_copy_buffer_row)

