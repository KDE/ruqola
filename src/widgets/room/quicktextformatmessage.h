/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QFrame>

class LIBRUQOLAWIDGETS_TESTS_EXPORT QuickTextFormatMessage : public QFrame
{
    Q_OBJECT
public:
    explicit QuickTextFormatMessage(QWidget *parent = nullptr);
    ~QuickTextFormatMessage() override;

Q_SIGNALS:
    void blockQuoteRequested();
};
