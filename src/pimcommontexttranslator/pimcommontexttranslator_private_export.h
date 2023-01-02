/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "pimcommontexttranslator_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef PIMCOMMONTEXTTRANSLATOR_TESTS_EXPORT
#define PIMCOMMONTEXTTRANSLATOR_TESTS_EXPORT PIMCOMMONTEXTTRANSLATOR_EXPORT
#endif
#else /* not compiling tests */
#define PIMCOMMONTEXTTRANSLATOR_TESTS_EXPORT
#endif
