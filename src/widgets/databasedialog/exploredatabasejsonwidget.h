/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "messages/message.h"
#include <QWidget>
class JsonPlainTextEditWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseJsonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseJsonWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseJsonWidget() override;

    void slotLoadedMessages(QList<Message> listMessages);

private:
    JsonPlainTextEditWidget *const mPlainTextEdit;
};
