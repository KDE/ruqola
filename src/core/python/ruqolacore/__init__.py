
__all__ = ['RuqolaCore']

# Preload PySide2 libraries to avoid missing libraries while loading RuqolaCore
try:
    from PySide2 import QtCore
except Exception:
    print("Failed to load PySide")
    raise

