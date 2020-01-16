add_executable(ruqola-widgets main.cpp)

target_link_libraries(ruqola-widgets
    libruqolacore
    libruqolawidgets
    librocketchatrestapi-qt5
    KF5::Crash
)

install(TARGETS ruqola-widgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
