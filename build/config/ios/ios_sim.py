#!/usr/bin/python
# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import argparse
import os
import errno
import subprocess
import sys
import re

simctl_path = [
  '/usr/bin/env',
  'xcrun',
  'simctl',
]

plist_buddy_path = [
  '/usr/bin/env',
  'xcrun',
  'PlistBuddy',
]

def application_identifier(path):
  identifier = subprocess.check_output( plist_buddy_path + [
    '-c',
    'Print CFBundleIdentifier',
    '%s/Info.plist' % path,
  ])

  return identifier.strip()

def install(args):
  return subprocess.check_call( simctl_path + [
    'install',
    'booted',
    args.path,
  ])

def install_launch_and_wait(args, wait):
  res = install(args)

  if res != 0:
    return res

  identifier = application_identifier(args.path)

  launch_args = [ 'launch' ]

  if wait:
    launch_args += [ '-w' ]

  launch_args += [
    'booted',
    identifier,
  ]

  return subprocess.check_output( simctl_path + launch_args ).strip()

def launch(args):
  install_launch_and_wait(args, False)

def debug(args):
  launch_res = install_launch_and_wait(args, True)
  launch_pid = re.search('.*: (\d+)', launch_res).group(1)
  return os.system(' '.join([
    '/usr/bin/env',
    'xcrun',
    'lldb',
    '-s',
    os.path.join(os.path.dirname(__file__), 'lldb_start_commands.txt'),
    '-p',
    launch_pid,
  ]))

def main():
  parser = argparse.ArgumentParser(description='A script that launches an'
                                   ' application in the simulator and attaches'
                                   ' the debugger to the same')

  parser.add_argument('-p', dest='path', required=True,
                      help='Path the the simulator application')

  subparsers = parser.add_subparsers()

  launch_parser = subparsers.add_parser('launch', help='Launch')
  launch_parser.set_defaults(func=launch)

  install_parser = subparsers.add_parser('install', help='Install')
  install_parser.set_defaults(func=install)

  debug_parser = subparsers.add_parser('debug', help='Debug')
  debug_parser.set_defaults(func=debug)

  args = parser.parse_args()

  return args.func(args)

if __name__ == '__main__':
  sys.exit(main())
