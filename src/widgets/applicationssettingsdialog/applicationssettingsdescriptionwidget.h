/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTextBrowser;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDescriptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsDescriptionWidget() override;

    void setDescription(const QString &desc);

private:
    QTextBrowser *const mTextBrowser;
};
