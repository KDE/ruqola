/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textedittexttospeech_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef TEXTEDITTEXTTOSPEECH_TESTS_EXPORT
#define TEXTEDITTEXTTOSPEECH_TESTS_EXPORT TEXTEDITTEXTTOSPEECH_EXPORT
#endif
#else /* not compiling tests */
#define TEXTEDITTEXTTOSPEECH_TESTS_EXPORT
#endif
