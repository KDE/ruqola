/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class JsonPlainTextEditWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowDebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowDebugWidget(QWidget *parent = nullptr);
    ~ShowDebugWidget() override;

    void setPlainText(const QString &text);

private:
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
};
