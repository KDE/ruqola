ecm_add_test(yandexengineclienttest.cpp yandexengineclienttest.h ../yandexengineclient.cpp ../yandexengineplugin.cpp
    TEST_NAME yandexengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
