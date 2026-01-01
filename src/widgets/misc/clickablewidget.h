/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QLabel>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ClickableLabel : public QLabel
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

class LIBRUQOLAWIDGETS_TESTS_EXPORT ClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClickableWidget(const QString &userName, QWidget *parent = nullptr);
    ~ClickableWidget() override;

    [[nodiscard]] QString name() const;
    void setName(const QString &userName);

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &userId);

Q_SIGNALS:
    void removeClickableWidget(const QString &username);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemove();
    QString mName;
    QByteArray mIdentifier;
    QLabel *const mLabel;
    ClickableLabel *const mClickableLabel;
};
