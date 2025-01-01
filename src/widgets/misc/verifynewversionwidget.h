/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QObject>
class QAction;
class LIBRUQOLAWIDGETS_TESTS_EXPORT VerifyNewVersionWidget : public QObject
{
    Q_OBJECT
public:
    explicit VerifyNewVersionWidget(QObject *parent = nullptr);
    ~VerifyNewVersionWidget() override;

    [[nodiscard]] bool canVerifyNewVersion() const;

    [[nodiscard]] QAction *verifyNewVersionAction();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVerifyNewVersion();
    QAction *mVerifyNewVersionAction = nullptr;
};
