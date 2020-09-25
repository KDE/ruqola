
__all__ = ['RocketChatRestApiQt5']

# Preload PySide2 libraries to avoid missing libraries while loading KDDockWidgets
try:
    from PySide2 import QtCore
except Exception:
    print("Failed to load PySide")
    raise

