/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT
#define LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT LIBROCKETCHATRESTAPI_QT_EXPORT
#endif
#else /* not compiling tests */
#define LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT
#endif
