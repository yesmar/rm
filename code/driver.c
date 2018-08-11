// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// Copyright © 2018, Ramsey Dow. All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rm.h"

int fn(void) {
  rm_handle *rm = rm_alloc(0, 0);
  if (!rm) {
    perror("rm_alloc");
    return -1;
  }

  char *a = strdup("foo");
  if (rm_push(rm, a, free) < 0) rm_ret_code(rm, -1);

  char *b = strdup("bar");
  if (rm_push(rm, b, free) < 0) rm_ret_code(rm, -1);

  char *c = strdup("baz");
  if (rm_push(rm, c, free) < 0) rm_ret_code(rm, -1);

  char *d = strdup("qux");
  if (rm_push(rm, d, free) < 0) rm_ret_code(rm, -1);

  char *e = strdup("quux");
  if (rm_push(rm, e, free) < 0) rm_ret_code(rm, -1);

  char *f = strdup("quuux");
  if (rm_push(rm, f, free) < 0) rm_ret_code(rm, -1);

  char *z = rm_replace(rm, f, strdup("zee"));
  if (z) {
    printf("replaced %s\n", z);
    free(z);
    z = NULL;
  } else {
    perror("rm_replace");
    rm_ret_code(rm, -1);
  }

  rm_ret_code(rm, 0);
}

int main(int __unused ac, char ** __unused av) {
  exit(fn() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
