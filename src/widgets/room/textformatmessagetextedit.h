/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QFrame>

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextFormatMessageTextEdit : public QFrame
{
    Q_OBJECT
public:
    explicit TextFormatMessageTextEdit(QWidget *parent = nullptr);
    ~TextFormatMessageTextEdit() override;

Q_SIGNALS:
    void blockQuoteRequested();
};
