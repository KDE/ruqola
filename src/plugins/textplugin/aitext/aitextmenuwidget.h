/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QMenu;
class AiTextMenuWidget : public QObject
{
    Q_OBJECT
public:
    explicit AiTextMenuWidget(QObject *parent = nullptr);
    ~AiTextMenuWidget() override;

    [[nodiscard]] QMenu *menu() const;

    [[nodiscard]] QString selectedText() const;
    void setSelectedText(const QString &newSelectedText);

private:
    void slotInitializeMenu();
    QString mSelectedText;
    QMenu *const mAiTextMenu;
};
