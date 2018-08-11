// Copyright © 2018, Ramsey Dow. All rights reserved.

#pragma once

#include <stddef.h>
#include "rm.h"

typedef struct {
  void *res; // Resource.
  release_res res_free; // Resource deallocator.
} rm_frame;

struct rm {
  rm_frame **frames; // Resource frame stack.
  size_t size; // Number of frames on the stack.
  size_t capacity; // Current capacity of the stack.
  size_t growth_factor; // How much we grow the stack by.
};

rm_frame *rm_pop(rm_handle *rm);
rm_frame *rm_frame_alloc(void *res, release_res res_free);
void rm_frame_free(rm_frame **fp);
