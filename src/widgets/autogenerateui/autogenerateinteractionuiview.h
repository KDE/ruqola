/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionutil.h"
#include "autogenerateui/autogenerateinteractionuiviewblocks.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>

// Rocket.Chat/packages/ui-kit/src/surfaces/modal/ModalView.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiView : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiView(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiView() override;

    void parseView(const QJsonObject &json);

    [[nodiscard]] QByteArray id() const;
    void setId(const QByteArray &newId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiView &other) const;

    [[nodiscard]] bool showIcon() const;
    void setShowIcon(bool newShowIcon);

    [[nodiscard]] AutoGenerateInteractionUiViewButtonElement *closeButton() const;
    void setCloseButton(AutoGenerateInteractionUiViewButtonElement *newCloseButton);

    [[nodiscard]] AutoGenerateInteractionUiViewButtonElement *submitButton() const;
    void setSubmitButton(AutoGenerateInteractionUiViewButtonElement *newSubmitButton);

    [[nodiscard]] AutoGenerateInteractionUiViewText title() const;
    void setTitle(const AutoGenerateInteractionUiViewText &newTitle);

    [[nodiscard]] AutoGenerateInteractionUiViewBlocks *blocks() const;
    void setBlocks(AutoGenerateInteractionUiViewBlocks *newBlocks);

    void generateWidget(QWidget *parent);

    [[nodiscard]] QJsonObject serialize(bool generateState = false) const;
    [[nodiscard]] QByteArray type() const;
    void setType(const QByteArray &newType);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);
    void setErrorMessages(const QMap<QString, QString> &map);

    void clear();
    [[nodiscard]] QJsonObject serializeState() const;
    void assignState(const QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> &map);
    [[nodiscard]] QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> createStateInfos() const;
Q_SIGNALS:
    void submitButtonClicked(const QJsonObject &payload, const QString &appId);
    void closeButtonClicked(const QJsonObject &payload, const QString &appId);
    void actionChanged(const AutoGenerateInteractionUtil::ViewBlockActionUserInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActionChanged(const QByteArray &blockId, const QByteArray &actionId, const QString &value);
    QByteArray mId;
    QByteArray mType;
    QByteArray mAppId;
    bool mShowIcon = false;
    AutoGenerateInteractionUiViewButtonElement *mCloseButton = nullptr;
    AutoGenerateInteractionUiViewButtonElement *mSubmitButton = nullptr;
    AutoGenerateInteractionUiViewText mTitle;
    AutoGenerateInteractionUiViewBlocks *mBlocks = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t);
