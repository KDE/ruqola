/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

