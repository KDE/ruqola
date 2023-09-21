/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_export.h"
class KTextEdit;
class QLabel;
class LIBRUQOLAWIDGETS_EXPORT ReportMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReportMessageWidget(QWidget *parent = nullptr);
    ~ReportMessageWidget() override;
    [[nodiscard]] QString message() const;

    void setPreviewMessage(const QString &msg);
Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    KTextEdit *const mMessageLineEdit;
    QLabel *const mMessagePreview;
};
