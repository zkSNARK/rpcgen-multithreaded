//
// Created by cmgoebel on 11/15/19.
//

#pragma once

#include <assert.h>

/**
 * An example which has no arguements that it passes to the server
 * but a single argument (int) in returned.  No memory is needed
 * to be allocated for this simple return type.
 */
void handle_initw(CLIENT *clnt) {
  int result_1;

  enum clnt_stat retval_1 = initw_1(&result_1, clnt);
  if (retval_1 != RPC_SUCCESS || result_1 == 0) {
    clnt_perror(clnt, "call failed");
  } else {
    printf("Successfully called initw.\n");
  }
}

/**
 * In this example we pass a simple char* (C style string) to the
 * server.  The server stores it into a local buffer, and then
 * the server returns an integer.
 *
 * I'm not entirely clear as to why we are allowed to pass a C style
 * string type here, but I can guess that something in the xdr creation
 * phase recognizes this as a \0 terminated string and then packs an
 * xdr buffer up to the \0.  Most of the other types seem to need a
 * buffer and a len, and xdr even provides us with string<> which is
 * a variable length string... but in this case we don't need to use it
 * to send data into xdr.  More experiments would need to be done to
 * see if we can return a string in a similar fashion from the server.
 */
void handle_insertw(CLIENT *clnt) {
  char *insertw_1_arg1 = "hello";
  int result_2;

  enum clnt_stat retval_2 = insertw_1(insertw_1_arg1, &result_2, clnt);
  if (retval_2 != RPC_SUCCESS) {
    clnt_perror(clnt, "call failed");
  } else {
    printf("Successfully added work to dict. %d words currently in "
           "dict\n",
           result_2);
  }
}

/**
 * This example shows how to use the 'int_ptr' xdr type, which is
 * just a variable length int buffer or 'int*' in C.
 *
 * Notice how in the client we need to allocate the return buffer.
 * Both the server and the client will need to allocate their result
 * buffers.
 *
 * Also note that it is absolutely critical that you fill out both the
 * *_val and *_len fields in the custom struct that rpcgen creates for
 * us.  If you skip the len field, everything will fail.
 */
void handle_array_example(CLIENT *clnt) {
  int test_sz = 10;
  int_ptr result, arr1, arr2;

  result.int_ptr_val = malloc(sizeof(int) * test_sz);
  result.int_ptr_len = test_sz;

  arr1.int_ptr_val = malloc(sizeof(int) * test_sz);
  arr1.int_ptr_len = test_sz;

  arr2.int_ptr_val = malloc(sizeof(int) * test_sz);
  arr2.int_ptr_len = test_sz;

  enum clnt_stat retval_3 = array_example_1(arr1, arr2, &result, clnt);
  if (retval_3 != RPC_SUCCESS) {
    clnt_perror(clnt, "call failed");
  } else {
    for (int i = 0; i < test_sz; ++i) {
      assert(arr1.int_ptr_val[i] + arr2.int_ptr_val[i] ==
             result.int_ptr_val[i]);
    }
    printf("Successfully added two arrays of ints in the server.\n");
  }
}

/**
 * In this example the server just takes in the matrix we start with
 * and adds 42 to each of the elements;
 *
 * Note that this our '2D' matrix is really just a big 1D buffer, so
 * in normal circumstances you would need to use a 2D through 1D
 * indexing strategy.  Here I just ignore that and treat the matrix
 * type as a 1D array.
 *
 * I do this because I am not interested in using the matrix... I am
 * only interested in demonstrating the use of the custom 'matrix'
 * structure.
 */
void handle_matrix_example(CLIENT *clnt) {
  matrix R, M; // R is result, M is what we create on client

  R.nrow = R.ncol = 10;
  R.data.int_ptr_len = R.nrow * R.ncol;
  R.data.int_ptr_val = malloc(sizeof(int) * R.data.int_ptr_len);

  M.nrow = M.ncol = 10;
  M.data.int_ptr_len = M.nrow * M.ncol;
  M.data.int_ptr_val = malloc(sizeof(int) * M.data.int_ptr_len);

  for (int i = 0; i < M.data.int_ptr_len; ++i) {
    M.data.int_ptr_val[i] = i;
  }

  enum clnt_stat retval_4 = matrix_example_1(M, &R, clnt);
  if (retval_4 != RPC_SUCCESS) {
    clnt_perror(clnt, "call failed");
  } else {
    for (int i = 0; i < M.data.int_ptr_len; ++i) {
      assert(M.data.int_ptr_val[i] + 42 == R.data.int_ptr_val[i]);
    }
    printf("Successfully called the matrix example.\n");
  }
}
