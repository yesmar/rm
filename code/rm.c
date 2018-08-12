// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// Copyright © 2018, Ramsey Dow. All rights reserved.
// SPDX short identifier: MIT

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "rm.h"

// Push a new resource frame into the resource manager stack. If any of the parameters are NULL, -1 will be returned and errno set to EINVAL. Any attempt to add frames beyond the stack's capacity will result in -1 being returned with errno set to EFBIG. Because a catastrophic error will occur if any given resource is pushed a second time, the stack will be scanned to ensure the resource's address is not already tracked. If it is, -1 will be returned with errno set to EEXIST.
int rm_push(resource_manager *rm, resource_frame *frame) {
  if (!rm || !rm->capacity || !rm->frames || !frame || !frame->res || !frame->res_free) {
    errno = EINVAL;
    return -1;
  }
  if (rm->count == rm->capacity) {
    errno = EFBIG;
    return -1;
  }
  for (size_t i = 0; i < rm->count; ++i) {
    if (rm->frames[i].res == frame->res) {
      errno = EEXIST;
      return -1;
    }
  }
  size_t idx = rm->count;
  rm->frames[idx].res = frame->res;
  rm->frames[idx].res_free = frame->res_free;
  printf("+ %p\n", rm->frames[idx].res);
  if (rm->count+1 <= rm->capacity) {
    ++rm->count;
  }
  return 0;
}

// Pops and returns the resource and and a pointer to its associated free function from the top of the stack, decrementing its size. If the resource manager is NULL, -1 will be returned with errno set to EINVAL. If the stack is empty, NULL will be returned with errno set to ENOENT.
int rm_pop(resource_manager *rm, void **res, release_res *res_free) {
  if (!rm || !rm->frames || !res || !res_free) {
    errno = EINVAL;
    return -1;
  }
  if (!rm->count) {
    errno = ENOENT;
    return -1;
  }
  size_t idx = rm->count-1;
  *res = rm->frames[idx].res;
  *res_free = rm->frames[idx].res_free;
  printf("- %p\n", rm->frames[idx].res);
  (void)memset(&rm->frames[idx], 0, sizeof(resource_frame));
  rm->count--;
  return 0;
}

// Replace res with new_res, preserving the containing resource frame. NULL will be returned with errno set to EINVAL if any of the parameters are NULL. If the resource can't be found then NULL will be returned with errno set to ENOENT. It is the caller's responsibility to guarantee that res and new_res are of the same type, i.e. make use of the same deallocator. It is likewise the caller's responsibility to properly deallocate the returned resource.
void *rm_replace(resource_manager *rm, void *res, void *new_res) {
  if (!rm || !rm->frames || !res || !new_res) {
    errno = EINVAL;
    return NULL;
  }
  for (size_t i = 0; i < rm->count; ++i) {
    if (rm->frames[i].res == res) {
      void *ret = rm->frames[i].res;
      rm->frames[i].res = new_res;
      return ret;
    }
  }
  errno = ENOENT;
  return NULL;

}

// Traverse the stack of resource frames, passing each tracked resource to its associated deallocation function. The resource manager will then be zeroed out. Note that referring to a managed pointer after it has been released by calling rm_free is undefined behavior.
void rm_free(resource_manager *rm) {
  if (rm) {
    if (rm->count && rm->frames) {
      for (ssize_t i = rm->count-1; i >= 0; --i) {
        void *resp = NULL;
        release_res freep = NULL;
        if (rm_pop(rm, &resp, &freep) < 0) {
          perror("rm_free");
          return;
        }
        freep(resp);
      }
    }
    (void)memset(rm, 0, sizeof(resource_manager));
  }
}
