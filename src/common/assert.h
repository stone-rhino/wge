#pragma once

#include <thread>

#include <assert.h>

extern std::thread::id main_thread_id;

#define ASSERT_IS_MAIN_THREAD() assert(std::this_thread::get_id() == main_thread_id)