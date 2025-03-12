/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiview.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUi : public QObject
{
    Q_OBJECT
public:
    enum class TypeUi : uint8_t {
        Unknown,
        ModalDialogUpdate,
        ModalDialogOpen,
        ContextualBarOpen,
        ContextualBarUpdate,
    };
    Q_ENUM(TypeUi)

    explicit AutoGenerateInteractionUi(RocketChatAccount *account, QObject *parent = nullptr);
    ~AutoGenerateInteractionUi() override;
    [[nodiscard]] bool parseInteractionUi(const QJsonObject &json);

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

    [[nodiscard]] QJsonObject generateJson() const;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT AutoGenerateInteractionUi::TypeUi convertTypeUiFromString(const QString &str) const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActionChanged(const AutoGenerateInteractionUtil::ViewBlockActionUserInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCloseButtonClicked(const QJsonObject &payload, const QString &appId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSubmitButtonClicked();
    QByteArray mAppId;
    QByteArray mTriggerId;
    TypeUi mTypeUi = TypeUi::Unknown;
    AutoGenerateInteractionUiView *mView = nullptr;
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUi)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUi &t);
