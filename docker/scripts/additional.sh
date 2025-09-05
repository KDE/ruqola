#SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
#SPDX-License-Identifier: MIT

#!/bin/bash

echo "📦 Installing Python dependencies.."
#Create a virtual environment at $HOME/lintenv
python3 -m venv $HOME/.reuselintenv

#Activate the virtual environment
source $HOME/.reuselintenv/bin/activate

#Upgrade pip to the latest version
python -m ensurepip --upgrade

#Install required pip packages
pip install setuptools
pip install reuse
pip install --break-system-packages python-gitlab gcovr cppcheck_codequality doxyqml cheroot wsgidav check-jsonschema chai pygdbmi \
	yamllint==1.33.0

#Deactivate the virtual environment
deactivate

#Add an alias for activating the virtual environment to .bashrc
echo 'alias activate_lint_env="source $HOME/.reuselintenv/bin/activate"' >>$HOME/.bashrc

#Retain changes regarding alias
source $HOME/.bashrc

echo "To test lint environment for 'reuse lint' please use 'activate_lint_env' -> 'reuse --version' and then 'deactivate'"

echo "Additional setup complete! Happy Linting! :)"
