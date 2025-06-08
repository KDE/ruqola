# Release method

- change set(RUQOLA_STABLE_VERSION false) in toplevel CMakeLists.txt to true
- change set(RUQOLA_RELEASE_VERSION "24.12.16") in toplevel CMakeLists.txt
  (used for show when version is too old)
- change src/widgets/needupdateversion/needupdateversionutils.cpp
  [https://cdn.kde.org/ci-builds/network/ruqola/version_number](https://cdn.kde.org/ci-builds/network/ruqola/version_number)

## Update generate binaire

- sysadmin/repo-metadata (update version here)
- sysadmin/ci-utilities (update version here)
