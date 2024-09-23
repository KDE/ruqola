Name:           ruqola
Version:        2.3.0
Release:        0
Summary:        Rocket.Chat client for the KDE desktop
Source0:        %{name}-%{version}.tar.gz
Source1:        %{name}-%{version}.tar.gz.asc
Url:            https://invent.kde.org/network/ruqola
Group:          Network/InstantMessaging
License:        GPL-2.0-or-later
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Vendor:         KDE
Packager:       Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>

Requires: breeze-icon-theme
BuildRequires: cmake extra-cmake-modules qtkeychain-qt6-devel
%if %{defined suse_version}
BuildRequires:  python3 update-desktop-files libqt6-qtbase-devel libqt6-qtwebsockets-devel libqt6-qtnetworkauth-devel libqt6-qtmultimedia-devel libqt6-qtsvg-devel kcoreaddons-devel ki18n-devel kcrash-devel knotifications-devel kiconthemes-devel syntax-highlighting-devel knotifyconfig-devel kdoctools-devel kdbusaddons-devel kxmlgui-devel kio-devel sonnet-devel ktextwidgets-devel ktextaddons-devel kidletime-devel karchive-devel purpose-devel prison-qt6-devel
%endif

%if %{defined fedora}
BuildRequires:  gcc-c++ shared-mime-info python desktop-file-utils qt6-qtbase-devel qt6-qtwebsockets-devel qt6-qtnetworkauth-devel qt6-qtmultimedia-devel qt6-qtsvg-devel  kf6-kcoreaddons-devel kf6-ki18n-devel kf6-kcrash-devel kf6-knotifications-devel kf6-kiconthemes-devel kf6-syntax-highlighting-devel kf6-knotifyconfig-devel kf6-kdoctools-devel kf6-kdbusaddons-devel kf6-kxmlgui-devel kf6-kio-devel kf6-sonnet-devel kf6-ktextwidgets-devel ktextaddons-devel kf6-kidletime-devel kf6-prison-devel kf6-kconfigwidgets-devel kf6-karchive-devel kf6-purpose-devel generic-logos util-linux
%endif

%description
Ruqola is a Rocket.Chat client for the KDE desktop.

It supports multi-account, search in room, open close rooms, direct message, thread, discussions.
RC settings can be changed directly.

Maintainer:
----------
     Laurent Montel <montel@kde.org>

%define debug_package %{nil}
%global __debug_install_post %{nil}

%prep
%setup -T -c
%{__tar} -zxf %{SOURCE0} --strip-components=1

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=False
%__make %{?_smp_mflags}

%install
%make_install

%clean
%__rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_prefix}/bin/ruqola
%{_libdir}/libruqolacore.*
%{_libdir}/libruqolawidgets.*
%{_libdir}/librocketchatrestapi-qt.*
%dir %{_libdir}/qt6/plugins/ruqolaplugins/
%{_libdir}/qt6/plugins/ruqolaplugins/authentication/
%{_libdir}/qt6/plugins/ruqolaplugins/textplugins/
%{_prefix}/share/applications/org.kde.ruqola.desktop
%{_prefix}/share/icons/hicolor
%{_prefix}/share/doc/HTML/en/ruqola/
%{_prefix}/share/knotifications5/ruqola.notifyrc
%{_prefix}/share/metainfo/org.kde.ruqola.appdata.xml
%{_prefix}/share/qlogging-categories5/ruqola.categories
%{_prefix}/share/qlogging-categories5/ruqola.renamecategories
%{_prefix}/share/locale

%changelog
* Sun 09 Jun 2024 Allen Winter <allen.winter@kdab.com> 2.2.0
- 2.2.0 release
* Thu 14 Mar 2024 Allen Winter <allen.winter@kdab.com> 2.1.1
- 2.1.1 release
* Mon 12 Feb 2024 Allen Winter <allen.winter@kdab.com> 2.1.0
- 2.1.0 release
* Tue 17 Oct 2023 Allen Winter <allen.winter@kdab.com> 2.0.1
- 2.0.1 release
* Wed 27 Sep 2023 Allen Winter <allen.winter@kdab.com> 2.0.0
- 2.0.0 release
* Mon 03 Jul 2023 Allen Winter <allen.winter@kdab.com> 1.9.2
- 1.9.2 release
* Mon 23 Jan 2023 Allen Winter <allen.winter@kdab.com> 1.9.1
- 1.9.1 release
* Wed 21 Dec 2022 Allen Winter <allen.winter@kdab.com> 1.9.0
- 1.9.0 release
* Wed 23 Nov 2022 Allen Winter <allen.winter@kdab.com> 1.8.2
- 1.8.2 release
* Sun 16 Oct 2022 Allen Winter <allen.winter@kdab.com> 1.8.1
- 1.8.1 release
* Tue Aug 31 2022 Allen Winter <allen.winter@kdab.com> 1.8.0
- 1.8.0 release
* Mon Aug 08 2022 Allen Winter <allen.winter@kdab.com> 1.7.4
- 1.7.4 release
* Sun Jul 31 2022 Allen Winter <allen.winter@kdab.com> 1.7.3
- 1.7.3 release
* Sun Jun 12 2022 Allen Winter <allen.winter@kdab.com> 1.7.2
- 1.7.2 release
* Sun May 01 2022 Allen Winter <allen.winter@kdab.com> 1.7.1
- 1.7.1 release
* Fri Apr 01 2022 Allen Winter <allen.winter@kdab.com> 1.7.0
- 1.7.0 release
* Tue Feb 22 2022 Allen Winter <allen.winter@kdab.com> 1.6.3
- 1.6.3 release
* Wed Feb 02 2022 Allen Winter <allen.winter@kdab.com> 1.6.2
- 1.6.2 release
* Wed Dec 22 2021 Allen Winter <allen.winter@kdab.com> 1.6.1
- 1.6.1 release
* Fri Dec 03 2021 Allen Winter <allen.winter@kdab.com> 1.6.0
- 1.6.0 release
* Fri Aug 06 2021 Allen Winter <allen.winter@kdab.com> 1.5.0
- 1.5.0 release
* Wed May 19 2021 Allen Winter <allen.winter@kdab.com> 1.4.1
- 1.4.1 release
* Thu Apr 22 2021 Allen Winter <allen.winter@kdab.com> 1.4.0
- 1.4.0 release
* Fri Feb 19 2021 Allen Winter <allen.winter@kdab.com> 1.3.1
- 1.3.1 release
* Mon Feb 01 2021 Allen Winter <allen.winter@kdab.com> 1.3.0
- 1.3.0 release
* Sat Dec 12 2020 Allen Winter <allen.winter@kdab.com> 1.2.2
- 1.2.2 release
* Sun Dec 06 2020 Allen Winter <allen.winter@kdab.com> 1.2.1
- 1.2.1 release
* Tue Dec 01 2020 Allen Winter <allen.winter@kdab.com> 1.1.98
- 1.2.0 beta2
