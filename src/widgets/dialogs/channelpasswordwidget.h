/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelPasswordWidget(QWidget *parent = nullptr);
    ~ChannelPasswordWidget() override;

    [[nodiscard]] QString password() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void slotPasswordChanged(const QString &str);
    KPasswordLineEdit *const mPasswordLineEdit;
};
