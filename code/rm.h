// Copyright © 2018, Ramsey Dow. All rights reserved.

#pragma once

typedef struct rm rm_handle;
typedef void (*release_res)(void *res);

rm_handle *rm_alloc(size_t capacity, size_t growth_factor);
int rm_push(rm_handle *rm, void *res, release_res free_res);
void rm_free(rm_handle **rm);
#define rm_ret(rm) do { rm_free(&(rm)); return; } while(0)
#define rm_ret_code(rm, ret) do { rm_free(&(rm)); return ret; } while(0)
