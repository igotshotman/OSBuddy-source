#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
A small utility module to prevent code duplication.

For licensing information, please see the LICENSE file.
"""

import subprocess
import sys

def warn(*string):
    """
    Prints a warning to stderr.

    Args:
        *string: the (optional) message to print.
    """
    print(*string, file=sys.stderr)

def die(*string):
    """
    Prints a warning to stderr and exits with an exit code of 1.

    Args:
        *string: the (optional) message to print.
    """
    warn(*string)
    sys.exit(1)

def call_asynchronous(*args):
    """
    Spawns a new process (asynchronous) using subprocess.Popen, see:
    https://docs.python.org/3/library/subprocess.html#subprocess.Popen.

    Args:
        *args: a list containing the executable to spawn, optionally with arguments to feed it.
               See the linked documentation for examples.
    Returns:
        True iff the process was spawned succesfully. Otherwise, False is returned with the
        exception message printed to stderr.
    """
    try:
        subprocess.Popen(*args)
        return True
    except OSError as err:
        warn("{}".format(err))
        return False

def call_synchronous(*args):
    """
    Spawns a new process (synchronous), using Python's subprocess module.

    Args:
        *args: a list containing the executable to spawn, optionally with arguments to feed it.
               See the subprocess documentation for examples.
    """
    try:
        subprocess.call(*args)
    except OSError as err:
        warn("{}".format(err))

def check_output(*args, shell=False):
    """
    A wrapper around subprocess.check_output, to prevent code duplication. See
    https://docs.python.org/3/library/subprocess.html#subprocess.check_output

    Args:
        *args: a list containing the executable whose output to check, optionally with arguments to
               feed it. See the subprocess documentation for examples.
        shell: if True, the process will be spawned through the shell. See the subprocess
               documentation for a more thorough explanation on the use-cases. Note that shell=True
               is generally unsafe!
    Returns:
        The output of the spawned process on success. Otherwise, None is returned with the
        exception message printed to stderr.
    """
    try:
        return subprocess.check_output(*args, shell=shell)
    except (OSError, subprocess.CalledProcessError) as err:
        warn("{}".format(err))
    return None

