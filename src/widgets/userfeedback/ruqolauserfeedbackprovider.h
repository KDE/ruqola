/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <KUserFeedback/Provider>

class LIBRUQOLAWIDGETS_EXPORT RuqolaUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit RuqolaUserFeedbackProvider(QObject *parent = nullptr);
    ~RuqolaUserFeedbackProvider() override;
};
