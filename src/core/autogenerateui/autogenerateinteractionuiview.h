/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/autogenerateinteractionuiviewblocks.h"
#include "libruqolacore_export.h"
#include <QJsonObject>

// Rocket.Chat/packages/ui-kit/src/surfaces/modal/ModalView.ts
// TODO make it private export symbol ?
class LIBRUQOLACORE_EXPORT AutoGenerateInteractionUiView
{
public:
    AutoGenerateInteractionUiView();
    ~AutoGenerateInteractionUiView();

    void parseView(const QJsonObject &json);

    [[nodiscard]] QByteArray id() const;
    void setId(const QByteArray &newId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiView &other) const;

    [[nodiscard]] bool showIcon() const;
    void setShowIcon(bool newShowIcon);

    [[nodiscard]] AutoGenerateInteractionUiViewButtonElement closeButton() const;
    void setCloseButton(const AutoGenerateInteractionUiViewButtonElement &newCloseButton);

    [[nodiscard]] AutoGenerateInteractionUiViewButtonElement submitButton() const;
    void setSubmitButton(const AutoGenerateInteractionUiViewButtonElement &newSubmitButton);

    [[nodiscard]] AutoGenerateInteractionUiViewText title() const;
    void setTitle(const AutoGenerateInteractionUiViewText &newTitle);

private:
    QByteArray mId;
    bool mShowIcon = false;
    AutoGenerateInteractionUiViewButtonElement mCloseButton;
    AutoGenerateInteractionUiViewButtonElement mSubmitButton;
    AutoGenerateInteractionUiViewText mTitle;
    AutoGenerateInteractionUiViewBlocks mBlocks;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiView)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiView, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t);
