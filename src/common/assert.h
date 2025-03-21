#pragma once

#include <thread>

#include <assert.h>

extern std::thread::id main_thread_id;

// Asserts that the current thread is the main thread.
#define ASSERT_IS_MAIN_THREAD() assert(std::this_thread::get_id() == main_thread_id)

// Asserts that current line of code should never be reached.
#define UNREACHABLE() assert(false)