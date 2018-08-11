// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// Copyright © 2018, Ramsey Dow. All rights reserved.

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "rm.h"
#include "rm_p.h"

static const size_t kDefaultFactor = 4;

// PUBLIC API

// Allocate a new resource manager. If capacity is non-zero, the initial size of the stack will be set to its value. This will improve run time performance by reducing the number of allocations. If capacity is zero, the initial stack size will be set to the value of growth factor, the second parameter. Once the stack's capacity is reached, new allocations will occur after every growth_factor number of calls to the rm_push function. If set to zero, growth_factor will be assigned the default value (see the definition of kDefaultFactor, above). If an error occurs NULL will be returned and errno will be set appropriately. Otherwise, a handle to the new rm instance will returned. It is the caller's responsibility to call rm_free on the handle when it is no longer needed.
rm_handle *rm_alloc(size_t capacity, size_t growth_factor) {
  rm_handle *p = calloc(1, sizeof(rm_handle));
  if (p) {
    if (!growth_factor) growth_factor = kDefaultFactor;
    if (!capacity) capacity = growth_factor;
    p->frames = calloc(capacity, sizeof(rm_frame *));
    if (!p->frames) {
      rm_free(&p);
      return NULL;
    }
    p->capacity = capacity;
    p->growth_factor = growth_factor;
  }
  return p;
}

// Push a new resource frame into the resource manager stack. If any of the parameters are NULL, -1 will be returned and errno set to EINVAL. The resource manager's stack will be grown whenever it is full and an attempt is made to push a new resource. Because a catastrophic error will occur if any given resource is pushed a second time, the stack will be scanned to ensure the resource's address is not already tracked. If it is, -1 will be returned with errno set to EEXIST.
int rm_push(rm_handle *rm, void *res, release_res free_res) {
  if (!rm || !res || !free_res) {
    errno = EINVAL;
    return -1;
  }
  if (rm->capacity && rm->size) {
    for (size_t i = 0; i < rm->size; ++i) {
      if (rm->frames[i]->res == res) {
        errno = EEXIST;
        return -1;
      }
    }
  }
  if (rm->size && rm->size % rm->capacity == 0) {
    size_t new_capacity = rm->capacity + rm->growth_factor;
    rm_frame **p = realloc(rm->frames, sizeof(rm_frame) * new_capacity);
    if (!p) {
      return -1;
    }
    rm->frames = p;
    rm->capacity = new_capacity;
  }
  rm_frame *fp = rm_frame_alloc(res, free_res);
  if (!fp) {
    return -1;
  }
  rm->frames[rm->size] = fp;
  printf("+ %p\n", fp);
  rm->size++;
  return 0;
}

// Destroys the specified resource manager. The stack of resource frames will be traversed. Each frame will be destroyed once its associated resource has been passed to its release_res function. The pointer to the resource manager will be set to NULL once it has been successfully destroyed. Note that referring to a managed pointer after it has been released by calling rm_free is undefined behavior. It is not an error to pass a NULL pointer to this function.
void rm_free(rm_handle **rm) {
  if (rm && *rm) {
    while ((*rm)->size > 0) {
      rm_frame *fp = rm_pop(*rm);
      void *fpaddr = fp;
      rm_frame_free(&fp);
      printf("- %p\n", fpaddr);
    }
    free((*rm)->frames);
    (void)memset(*rm, 0, sizeof(rm_handle));
    free(*rm);
    *rm = NULL;
  }
}

// PRIVATE API

// Pops and returns the resource frame from the top of the stack, decrementing its size. If the resource manager is NULL, NULL will be returned with errno set to EINVAL. If the stack is empty, NULL will be returned with errno set to ENOENT.
rm_frame *rm_pop(rm_handle *rm) {
  if (!rm) {
    errno = EINVAL;
    return NULL;
  }
  if (!rm->size) {
    errno = ENOENT;
    return NULL;
  }
  size_t idx = rm->size-1;
  rm_frame *p = rm->frames[idx];
  rm->frames[idx] = NULL;
  rm->size--;
  return p;
}

// Allocate a new resource frame. If any of the parameters are NULL, -1 will be returned and errno set to EINVAL. Otherwise, the res and res_free pointers will be assigned to the new rm_frame instance. It is the caller's responsibility to call rm_free on the handle when it is no longer needed.
rm_frame *rm_frame_alloc(void *res, release_res res_free) {
  if (!res || !res_free) {
    errno = EINVAL;
    return NULL;
  }
  rm_frame *p = calloc(1, sizeof(rm_frame));
  if (!p) {
    return NULL;
  }
  p->res = res;
  p->res_free = res_free;
  return p;
}

// Destroys the specified resource frame. Given a non-NULL resource and release_res function, the resource will be released and set to NULL. The pointer to the resource frame will be set to NULL once it has been successfully destroyed. It is not an error to pass a NULL pointer to this function.
void rm_frame_free(rm_frame **fp) {
  if (fp && *fp) {
    if ((*fp)->res && (*fp)->res_free) {
      (*fp)->res_free((*fp)->res);
      (*fp)->res = NULL;
    }
    (void)memset(*fp, 0, sizeof(rm_frame));
    free(*fp);
    *fp = NULL;
  }
}
