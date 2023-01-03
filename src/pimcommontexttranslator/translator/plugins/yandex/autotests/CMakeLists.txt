# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(yandexengineclienttest.cpp yandexengineclienttest.h ../yandexengineclient.cpp ../yandexengineplugin.cpp
    TEST_NAME yandexengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
