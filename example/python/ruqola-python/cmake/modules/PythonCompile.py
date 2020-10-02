# By Simon Edwards <simon@simonzone.com>
# This file is in the public domain.

"""
Byte-compiles a given Python source file, generating a .pyc file or, if the
Python executable was invoked with -O, a .pyo file from it.
It uses the --destination-dir option to set the path to the source file (which
will appear in tracebacks, for example), so that if the .py file was in a build
root will appear with the right path.
"""

import argparse
import os
import py_compile


if __name__ == '__main__':
    parser = argparse.ArgumentParser('Byte-compiles a Python source file.')
    parser.add_argument('-d', '--destination-dir', required=True,
                        help='Location where the source file will be '
                        'installed, without any build roots.')
    parser.add_argument('source_file',
                        help='Source file to byte-compile.')

    args = parser.parse_args()

    dfile = os.path.join(args.destination_dir,
                         os.path.basename(args.source_file))
    py_compile.compile(args.source_file, dfile=dfile)
