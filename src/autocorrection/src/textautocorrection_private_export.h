/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textautocorrection_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef TEXTAUTOCORRECTION_TESTS_EXPORT
#define TEXTAUTOCORRECTION_TESTS_EXPORT TEXTAUTOCORRECTION_EXPORT
#endif
#else /* not compiling tests */
#define TEXTAUTOCORRECTION_TESTS_EXPORT
#endif
