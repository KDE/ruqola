/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "libruqolawidgets_private_export.h"

// see ./packages/ui-kit/src/blocks/layout/InputBlock.ts
class AutoGenerateInteractionUiViewActionable;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewInputBlock : public AutoGenerateInteractionUiViewBlockBase
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewInputBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewInputBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewInputBlock &other) const;

    [[nodiscard]] bool optional() const;
    void setOptional(bool newOptional);

    [[nodiscard]] AutoGenerateInteractionUiViewText label() const;
    void setLabel(const AutoGenerateInteractionUiViewText &newLabel);

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewActionable *element() const;
    void setElement(AutoGenerateInteractionUiViewActionable *newElement);

    void setErrorMessages(const QMap<QString, QString> &map) override;
    void assignState(const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> &info) override;

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeBlockState() const override;

private:
    AutoGenerateInteractionUiViewText mLabel;
    AutoGenerateInteractionUiViewActionable *mElement = nullptr;
    // TODO hint
    bool mOptional = false;
};

class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t);
