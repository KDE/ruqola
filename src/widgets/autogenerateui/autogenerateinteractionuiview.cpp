/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiview.h"
#include <QDialogButtonBox>
#include <QJsonArray>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiView::AutoGenerateInteractionUiView(QObject *parent)
    : QObject(parent)
{
}

AutoGenerateInteractionUiView::~AutoGenerateInteractionUiView()
{
    delete mCloseButton;
    delete mSubmitButton;
}

void AutoGenerateInteractionUiView::parseView(const QJsonObject &json)
{
    if (json.contains("close"_L1)) {
        mCloseButton = new AutoGenerateInteractionUiViewButtonElement;
        mCloseButton->parse(json["close"_L1].toObject());
    }
    if (json.contains("submit"_L1)) {
        mSubmitButton = new AutoGenerateInteractionUiViewButtonElement;
        mSubmitButton->parse(json["submit"_L1].toObject());
    }
    mShowIcon = json["showIcon"_L1].toBool();
    mId = json["id"_L1].toString().toLatin1();
    mAppId = json["appId"_L1].toString().toLatin1();
    mType = json["type"_L1].toString().toLatin1();
    mTitle.parse(json["title"_L1].toObject());
    if (!mBlocks) {
        mBlocks = new AutoGenerateInteractionUiViewBlocks(this);
    }
    mBlocks->parse(json["blocks"_L1].toArray());
}

QByteArray AutoGenerateInteractionUiView::id() const
{
    return mId;
}

void AutoGenerateInteractionUiView::setId(const QByteArray &newId)
{
    mId = newId;
}

bool AutoGenerateInteractionUiView::operator==(const AutoGenerateInteractionUiView &other) const
{
    return other.id() == id() && other.showIcon() == showIcon() && *other.closeButton() == *closeButton() && *other.submitButton() == *submitButton()
        && other.title() == title() && other.blocks() == blocks() && other.appId() == appId() && other.type() == type();
}

bool AutoGenerateInteractionUiView::showIcon() const
{
    return mShowIcon;
}

void AutoGenerateInteractionUiView::setShowIcon(bool newShowIcon)
{
    mShowIcon = newShowIcon;
}

AutoGenerateInteractionUiViewButtonElement *AutoGenerateInteractionUiView::closeButton() const
{
    return mCloseButton;
}

void AutoGenerateInteractionUiView::setCloseButton(AutoGenerateInteractionUiViewButtonElement *newCloseButton)
{
    mCloseButton = newCloseButton;
}

AutoGenerateInteractionUiViewButtonElement *AutoGenerateInteractionUiView::submitButton() const
{
    return mSubmitButton;
}

void AutoGenerateInteractionUiView::setSubmitButton(AutoGenerateInteractionUiViewButtonElement *newSubmitButton)
{
    mSubmitButton = newSubmitButton;
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiView::title() const
{
    return mTitle;
}

void AutoGenerateInteractionUiView::setTitle(const AutoGenerateInteractionUiViewText &newTitle)
{
    mTitle = newTitle;
}

AutoGenerateInteractionUiViewBlocks *AutoGenerateInteractionUiView::blocks() const
{
    return mBlocks;
}

void AutoGenerateInteractionUiView::setBlocks(AutoGenerateInteractionUiViewBlocks *newBlocks)
{
    mBlocks = newBlocks;
}

void AutoGenerateInteractionUiView::generateWidget(QWidget *widget, RocketChatAccount *account)
{
    // TODO customize title
    widget->setWindowTitle(mTitle.generateText());
    auto mainLayout = new QVBoxLayout(widget);
    if (!mBlocks) {
        mBlocks = new AutoGenerateInteractionUiViewBlocks(this);
    }
    mBlocks->generateWidget(account, widget);
    if (mCloseButton || mSubmitButton) {
        auto buttonDialog = new QDialogButtonBox(widget);
        if (mCloseButton) {
            auto closeButton = buttonDialog->addButton(QDialogButtonBox::Close);
            closeButton->setText(mCloseButton->text().generateText());
            connect(closeButton, &QPushButton::clicked, this, &AutoGenerateInteractionUiView::closed);
        }
        if (mSubmitButton) {
            auto okButton = buttonDialog->addButton(QDialogButtonBox::Ok);
            okButton->setText(mSubmitButton->text().generateText());
            connect(okButton, &QPushButton::clicked, this, &AutoGenerateInteractionUiView::submited);
        }
        mainLayout->addStretch(1);
        mainLayout->addWidget(buttonDialog);
    }
}

QJsonObject AutoGenerateInteractionUiView::serialize() const
{
    QJsonObject o;
    o["blocks"_L1] = mBlocks->serialize();
    o["title"_L1] = mTitle.serialize();
    if (mSubmitButton) {
        o["submit"_L1] = mSubmitButton->serialize();
    }
    if (mCloseButton) {
        o["close"_L1] = mCloseButton->serialize();
    }
    o["id"_L1] = QString::fromLatin1(mId);
    o["type"_L1] = QString::fromLatin1(mType);
    o["appId"_L1] = QString::fromLatin1(mAppId);
    return o;
}

QByteArray AutoGenerateInteractionUiView::type() const
{
    return mType;
}

void AutoGenerateInteractionUiView::setType(const QByteArray &newType)
{
    mType = newType;
}

QByteArray AutoGenerateInteractionUiView::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUiView::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t)
{
    d.space() << "id:" << t.id();
    d.space() << "type:" << t.type();
    d.space() << "appId:" << t.appId();
    d.space() << "showIcon:" << t.showIcon();
    d.space() << "closeButton:" << *t.closeButton();
    d.space() << "submitButton:" << *t.submitButton();
    d.space() << "title:" << t.title();
    d.space() << "blocks:" << t.blocks();
    return d;
}

#include "moc_autogenerateinteractionuiview.cpp"
