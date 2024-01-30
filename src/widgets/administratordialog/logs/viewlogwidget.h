/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ViewLogPlainTextEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ViewLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewLogWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ViewLogWidget() override;
    void initialize();

protected:
    void showEvent(QShowEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStdoutQueueDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotInsertStdOutInfo(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void insertLine(const QString &str);
    QStringList mStdoutBeforeLoadingHistory;
    ViewLogPlainTextEdit *const mPlainTextEdit;
    RocketChatAccount *const mRocketChatAccount;
    quint64 mStdoutIdentifier = 0;
    bool mWasInitialized = false;
    bool mHistoryStdoutLoaded = false;
};
