// MobileGlues - gl/sync_guard.cpp
// Copyright (c) 2025-2026 MobileGL-Dev
// Licensed under the GNU Lesser General Public License v2.1:
//   https://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt
// SPDX-License-Identifier: LGPL-2.1-only
// End of Source File Header

#include "sync_guard.h"
#include "../gles/loader.h"
#include "log.h"
#include "mg.h"

#define DEBUG 0

#include <chrono>
#include <future>
#include <memory>
#include <thread>

static constexpr auto SYNC_GUARD_TIMEOUT = std::chrono::seconds(2);

void glDeleteSync(GLsync sync) {
    LOG()
    if (sync == nullptr) {
        return;
    }

    auto promise = std::make_shared<std::promise<void>>();
    std::future<void> fut = promise->get_future();

    std::thread worker([sync, promise]() {
        GLES.glDeleteSync(sync);
        promise->set_value();
    });
    worker.detach();

    if (fut.wait_for(SYNC_GUARD_TIMEOUT) == std::future_status::timeout) {
        LOG_E("glDeleteSync: driver did not return within %lldms - abandoning wait to avoid "
              "freezing the render thread (this looks like an ANGLE-level fence bug, not a "
              "MobileGlues one). The sync object may leak.",
              (long long)std::chrono::duration_cast<std::chrono::milliseconds>(SYNC_GUARD_TIMEOUT).count())
    }
}

GLenum glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    LOG()
    if (sync == nullptr) {
        return GL_WAIT_FAILED;
    }

    auto promise = std::make_shared<std::promise<GLenum>>();
    std::future<GLenum> fut = promise->get_future();

    std::thread worker([sync, flags, timeout, promise]() {
        GLenum result = GLES.glClientWaitSync(sync, flags, timeout);
        promise->set_value(result);
    });
    worker.detach();

    auto status = fut.wait_for(SYNC_GUARD_TIMEOUT);
    if (status == std::future_status::timeout) {
        LOG_E("glClientWaitSync: driver did not return within %lldms - abandoning wait to avoid "
              "freezing the render thread (this looks like an ANGLE-level fence bug, not a "
              "MobileGlues one). Reporting GL_TIMEOUT_EXPIRED to the caller.",
              (long long)std::chrono::duration_cast<std::chrono::milliseconds>(SYNC_GUARD_TIMEOUT).count())
        return GL_TIMEOUT_EXPIRED;
    }

    return fut.get();
}
