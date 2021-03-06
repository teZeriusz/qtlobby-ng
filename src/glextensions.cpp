/****************************************************************************
 **
 ** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the demonstration applications of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** No Commercial Usage
 ** This file contains pre-release code and may not be distributed.
 ** You may use this file in accordance with the terms and conditions
 ** contained in the either Technology Preview License Agreement or the
 ** Beta Release License Agreement.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain
 ** additional rights. These rights are described in the Nokia Qt LGPL
 ** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at http://www.qtsoftware.com/contact.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "glextensions.h"

#define RESOLVE_GL_FUNC(f) ok &= bool((f = (_gl##f) context->getProcAddress(QLatin1String("gl" #f))));

bool GLExtensionFunctions::resolve(const QGLContext *context) {
    bool ok = true;

    RESOLVE_GL_FUNC(CreateShaderObjectARB)
    RESOLVE_GL_FUNC(ShaderSourceARB)
    RESOLVE_GL_FUNC(CompileShaderARB)
    RESOLVE_GL_FUNC(GetObjectParameterivARB)
    RESOLVE_GL_FUNC(DeleteObjectARB)
    RESOLVE_GL_FUNC(GetInfoLogARB)
    RESOLVE_GL_FUNC(CreateProgramObjectARB)
    RESOLVE_GL_FUNC(AttachObjectARB)
    RESOLVE_GL_FUNC(DetachObjectARB)
    RESOLVE_GL_FUNC(LinkProgramARB)
    RESOLVE_GL_FUNC(UseProgramObjectARB)
    RESOLVE_GL_FUNC(GetUniformLocationARB)
    RESOLVE_GL_FUNC(Uniform1iARB)
    RESOLVE_GL_FUNC(Uniform1fARB)
    RESOLVE_GL_FUNC(Uniform2fARB)
    RESOLVE_GL_FUNC(Uniform4fARB)
    RESOLVE_GL_FUNC(UniformMatrix4fvARB)

    RESOLVE_GL_FUNC(GenFramebuffersEXT)
    RESOLVE_GL_FUNC(GenRenderbuffersEXT)
    RESOLVE_GL_FUNC(BindRenderbufferEXT)
    RESOLVE_GL_FUNC(RenderbufferStorageEXT)
    RESOLVE_GL_FUNC(DeleteFramebuffersEXT)
    RESOLVE_GL_FUNC(DeleteRenderbuffersEXT)
    RESOLVE_GL_FUNC(BindFramebufferEXT)
    RESOLVE_GL_FUNC(FramebufferTexture2DEXT)
    RESOLVE_GL_FUNC(FramebufferRenderbufferEXT)
    RESOLVE_GL_FUNC(CheckFramebufferStatusEXT)

    RESOLVE_GL_FUNC(ActiveTexture)
    RESOLVE_GL_FUNC(TexImage3D)

    RESOLVE_GL_FUNC(GenBuffers)
    RESOLVE_GL_FUNC(BindBuffer)
    RESOLVE_GL_FUNC(BufferData)
    RESOLVE_GL_FUNC(DeleteBuffers)
    RESOLVE_GL_FUNC(MapBuffer)
    RESOLVE_GL_FUNC(UnmapBuffer)

    return ok;
}

bool GLExtensionFunctions::glslSupported() {
    return CreateShaderObjectARB
           && CreateShaderObjectARB
           && ShaderSourceARB
           && CompileShaderARB
           && GetObjectParameterivARB
           && DeleteObjectARB
           && GetInfoLogARB
           && CreateProgramObjectARB
           && AttachObjectARB
           && DetachObjectARB
           && LinkProgramARB
           && UseProgramObjectARB
           && GetUniformLocationARB
           && Uniform1iARB
           && Uniform1fARB
           && Uniform4fARB
           && UniformMatrix4fvARB;
}

bool GLExtensionFunctions::fboSupported() {
    return GenFramebuffersEXT
           && GenRenderbuffersEXT
           && BindRenderbufferEXT
           && RenderbufferStorageEXT
           && DeleteFramebuffersEXT
           && DeleteRenderbuffersEXT
           && BindFramebufferEXT
           && FramebufferTexture2DEXT
           && FramebufferRenderbufferEXT
           && CheckFramebufferStatusEXT;
}

bool GLExtensionFunctions::openGL15Supported() {
    return ActiveTexture
           && TexImage3D
           && GenBuffers
           && BindBuffer
           && BufferData
           && DeleteBuffers
           && MapBuffer
           && UnmapBuffer;
}

#undef RESOLVE_GL_FUNC
