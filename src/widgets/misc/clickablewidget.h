/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QLabel>
#include <QWidget>

class LIBRUQOLAWIDGETS_EXPORT ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    ~ClickableLabel() override;

Q_SIGNALS:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class LIBRUQOLAWIDGETS_EXPORT ClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClickableWidget(const QString &userName, QWidget *parent = nullptr);
    ~ClickableWidget() override;

    [[nodiscard]] QString name() const;
    void setName(const QString &userName);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &userId);

Q_SIGNALS:
    void removeClickableWidget(const QString &username);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemove();
    QString mName;
    QString mIdentifier;
    QLabel *const mLabel;
    ClickableLabel *const mClickableLabel;
};
