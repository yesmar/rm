// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// Copyright © 2018, Ramsey Dow. All rights reserved.
// SPDX short identifier: MIT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rm.h"

int fn(void) {
  resource_frame frames[6] = {{0}}; // Initialize the resource frame stack.
  resource_manager rm = { .frames = frames, .capacity = 6, .count = 0 };
  
  char *a = strdup("foo");
  resource_frame frame_a = { .res = a, .res_free = free };
  if (rm_push(&rm, &frame_a) < 0) {
    perror("rm_push a");
    rm_ret_code(&rm, -1);
  }
  
  char *b = strdup("bar");
  resource_frame frame_b = { .res = b, .res_free = free };
  if (rm_push(&rm, &frame_b) < 0) {
    perror("rm_push b");
    rm_ret_code(&rm, -1);
  }
  
  char *c = strdup("baz");
  resource_frame frame_c = { .res = c, .res_free = free };
  if (rm_push(&rm, &frame_c) < 0) {
    perror("rm_push c");
    rm_ret_code(&rm, -1);
  }
  
  char *d = strdup("qux");
  resource_frame frame_d = { .res = d, .res_free = free };
  if (rm_push(&rm, &frame_d) < 0) {
    perror("rm_push d");
    rm_ret_code(&rm, -1);
  }
  
  char *e = strdup("quux");
  resource_frame frame_e = { .res = e, .res_free = free };
  if (rm_push(&rm, &frame_e) < 0) {
    perror("rm_push e");
    rm_ret_code(&rm, -1);
  }
  
  // Test push of duplicate resource:
  //if (rm_push(&rm, &frame_e) < 0) {
  //  perror("rm_push e redux");
  //  rm_ret_code(&rm, -1);
  //}
  
  char *f = strdup("quuux");
  resource_frame frame_f = { .res = f, .res_free = free };
  if (rm_push(&rm, &frame_f) < 0) {
    perror("rm_push f");
    rm_ret_code(&rm, -1);
  }
   
  // Test resource frame stack overflow:
  //char *g = strdup("quuuux");
  //resource_frame frame_g = { .res =g, .res_free = free };
  //if (rm_push(&rm, &frame_g) < 0) {
  //  perror("rm_push g");
  //  rm_ret_code(&rm, -1);
  //}
  
  // Test resource replacement:
  //char *z = rm_replace(&rm, f, strdup("zee"));
  //if (z) {
  //  printf("replaced %s\n", z);
  //  free(z);
  //  z = NULL;
  //} else {
  //  perror("rm_replace");
  //  rm_ret_code(&rm, -1);
  //}

  rm_ret_code(&rm, 0);
}

int main(int __unused ac, char ** __unused av) {
  exit(fn() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
