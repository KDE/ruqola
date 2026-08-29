/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "dialogs/searchmessagecommand.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageCommandButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchMessageCommandButtonWidget(QWidget *parent = nullptr);
    ~SearchMessageCommandButtonWidget() override;

Q_SIGNALS:
    void insertSearchString(bool needSpace, const QString &, bool unique);

private:
    struct ButtonInfo {
        QString identifier;
        QString i18n;
        QString toolTip;
        bool unique = false;
        bool needSpace = false;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QList<SearchMessageCommandButtonWidget::ButtonInfo> fillCommandLineText() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPushButton *createPushButton(const ButtonInfo &info);
    [[nodiscard]] static LIBRUQOLAWIDGETS_NO_EXPORT SearchMessageCommandButtonWidget::ButtonInfo
    createButtonInfo(SearchMessageCommand::SearchMessageCommandType type, const QString &title, const QString &tooltip);
};
