/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#ifdef USE_KUSERFEEDBACK_QT6
#include <KUserFeedbackQt6/Provider>
#else
#include <KUserFeedback/Provider>
#endif
class LIBRUQOLAWIDGETS_EXPORT RuqolaUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit RuqolaUserFeedbackProvider(QObject *parent = nullptr);
    ~RuqolaUserFeedbackProvider() override;
};
