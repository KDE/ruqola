/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ActionElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ActionElementWidget(QWidget *widget, const QByteArray &actionId, QWidget *parent = nullptr);
    ~ActionElementWidget() override;

    void setErrorMessages(const QString &msg);

    void clearError();

    [[nodiscard]] QByteArray actionId() const;
    void setActionId(const QByteArray &newActionId);

    void setErrorMessages(const QMap<QString, QString> &map);

private:
    QByteArray mActionId;
    QLabel *const mErrorLabel;
};
