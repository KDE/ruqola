/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiview.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUi : public QObject
{
    Q_OBJECT
public:
    enum class TypeUi : uint8_t {
        Unknown,
        ModalDialog,
        ContextualBar,
    };
    Q_ENUM(TypeUi)

    explicit AutoGenerateInteractionUi(QObject *parent = nullptr);
    ~AutoGenerateInteractionUi() override;
    void parseInteractionUi(const QJsonObject &json);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] TypeUi typeUi() const;
    void setTypeUi(TypeUi newTypeUi);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUi &other) const;

    [[nodiscard]] QByteArray triggerId() const;
    void setTriggerId(const QByteArray &newTriggerId);

    [[nodiscard]] AutoGenerateInteractionUiView *view() const;
    void setView(AutoGenerateInteractionUiView *newView);

    [[nodiscard]] QWidget *generateWidget(QWidget *parent = nullptr);

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT AutoGenerateInteractionUi::TypeUi convertTypeUiFromString(const QString &str) const;
    QByteArray mAppId;
    QByteArray mTriggerId;
    TypeUi mTypeUi = TypeUi::Unknown;
    AutoGenerateInteractionUiView *mView = nullptr;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUi)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUi &t);
