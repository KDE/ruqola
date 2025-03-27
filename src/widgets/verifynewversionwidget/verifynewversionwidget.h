/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT VerifyNewVersionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VerifyNewVersionWidget(QWidget *parent = nullptr);
    ~VerifyNewVersionWidget() override;

    void checkNewVersion();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFoundNewVersion(const QUrl &url, bool found);
    LIBRUQOLAWIDGETS_NO_EXPORT void checkCurrentVersion();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFoundNextVersion(bool found);
    QLabel *const mCheckVersionResultLabel;
};
