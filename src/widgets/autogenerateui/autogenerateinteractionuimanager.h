/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QJsonObject>
#include <QObject>

class AutoGenerateInteractionUiManager : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiManager(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiManager() override;

    [[nodiscard]] QWidget *parentWidget() const;
    void setParentWidget(QWidget *newParentWidget);

    void addNewUi(const QJsonObject &obj);

private:
    QWidget *mParentWidget = nullptr;
    QList<QJsonObject> mListJsonObject;
};
