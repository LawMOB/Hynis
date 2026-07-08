// MobileGlues - gl/sync_guard.h
// Copyright (c) 2025-2026 MobileGL-Dev
// Licensed under the GNU Lesser General Public License v2.1:
//   https://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt
// SPDX-License-Identifier: LGPL-2.1-only
// End of Source File Header

#ifndef MOBILEGLUES_SYNC_GUARD_H
#define MOBILEGLUES_SYNC_GUARD_H

#include "../includes.h"
#include "glcorearb.h"
#include <GL/gl.h>

#ifdef __cplusplus
extern "C"
{
#endif

    GLAPI GLAPIENTRY void glDeleteSync(GLsync sync);
    GLAPI GLAPIENTRY GLenum glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

#ifdef __cplusplus
}
#endif

#endif // MOBILEGLUES_SYNC_GUARD_H
