/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_export.h"
class KTextEdit;
class QLabel;
class LIBRUQOLAWIDGETS_EXPORT ReportUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReportUserWidget(QWidget *parent = nullptr);
    ~ReportUserWidget() override;
    [[nodiscard]] QString message() const;

    void setUserName(const QString &userName);

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    KTextEdit *const mMessageLineEdit;
    QLabel *const mUserName;
};
