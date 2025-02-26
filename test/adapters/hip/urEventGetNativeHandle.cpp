// Copyright (C) 2022-2023 Intel Corporation
// Part of the Unified-Runtime Project, under the Apache License v2.0 with LLVM Exceptions.
// See LICENSE.TXT
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "fixtures.h"

using urHipEventGetNativeHandleTest = uur::urQueueTest;
UUR_INSTANTIATE_DEVICE_TEST_SUITE_P(urHipEventGetNativeHandleTest);

TEST_P(urHipEventGetNativeHandleTest, Success) {
    constexpr size_t buffer_size = 1024;
    ur_mem_handle_t mem = nullptr;
    ASSERT_SUCCESS(urMemBufferCreate(context, UR_MEM_FLAG_READ_WRITE,
                                     buffer_size, nullptr, &mem));

    ur_event_handle_t event = nullptr;
    uint8_t pattern = 6;
    ASSERT_SUCCESS(urEnqueueMemBufferFill(queue, mem, &pattern, sizeof(pattern),
                                          0, buffer_size, 0, nullptr, &event));

    ur_native_handle_t native_event = nullptr;
    ASSERT_SUCCESS(urEventGetNativeHandle(event, &native_event));
    hipEvent_t hip_event = reinterpret_cast<hipEvent_t>(native_event);

    ASSERT_SUCCESS_HIP(hipEventSynchronize(hip_event));

    ASSERT_SUCCESS(urEventRelease(event));
    ASSERT_SUCCESS(urMemRelease(mem));
}
