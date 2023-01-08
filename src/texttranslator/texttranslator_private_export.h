/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "texttranslator_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef TEXTTRANSLATOR_TESTS_EXPORT
#define TEXTTRANSLATOR_TESTS_EXPORT TEXTTRANSLATOR_EXPORT
#endif
#else /* not compiling tests */
#define TEXTTRANSLATOR_TESTS_EXPORT
#endif
