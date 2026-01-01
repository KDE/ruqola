/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT NewMessageIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit NewMessageIndicator(QWidget *parent = nullptr);
    ~NewMessageIndicator() override;

    void showNewMessageIndicator(bool visible);

private:
    KMessageWidget *const mMessageWidget;
};
