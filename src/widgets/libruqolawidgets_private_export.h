/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef LIBRUQOLAWIDGETS_TESTS_EXPORT
#define LIBRUQOLAWIDGETS_TESTS_EXPORT LIBRUQOLAWIDGETS_EXPORT
#endif
#else /* not compiling tests */
#define LIBRUQOLAWIDGETS_TESTS_EXPORT
#endif
