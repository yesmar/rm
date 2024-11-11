// Copyright © 2018, Ramsey Dow. All rights reserved.
// SPDX short identifier: MIT

#pragma once

typedef void (*release_res)(void *res);

typedef struct {
    void *res; // Resource.
    release_res res_free; // Resource deallocator.
} resource_frame;

typedef struct {
    resource_frame *frames; // Resource frame stack.
    size_t capacity; // Maximum stack capacity.
    size_t count; // Number of frames on the stack.
} resource_manager;

int rm_push(resource_manager *rm, resource_frame *new_frame);
int rm_pop(resource_manager *rm, void **res, release_res *res_free);
void *rm_replace(resource_manager *rm, void *restrict res, void *restrict new_res);
int rm_free(resource_manager *rm, void *res);
void rm_free_manager(resource_manager *rm);
#define rm_ret(rm) do { rm_free_manager(rm); return; } while(0)
#define rm_ret_code(rm, ret) do { rm_free_manager(rm); return ret; } while(0)
