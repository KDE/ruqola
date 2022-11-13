/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "pimcommonautocorrection_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef PIMCOMMONAUTOCORRECTION_TESTS_EXPORT
#define PIMCOMMONAUTOCORRECTION_TESTS_EXPORT PIMCOMMONAUTOCORRECTION_EXPORT
#endif
#else /* not compiling tests */
#define PIMCOMMONAUTOCORRECTION_TESTS_EXPORT
#endif
