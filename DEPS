# This file is automatically processed to create .DEPS.git which is the file
# that gclient uses under git.
#
# See http://code.google.com/p/chromium/wiki/UsingGit
#
# To test manually, run:
#   python tools/deps2git/deps2git.py -o .DEPS.git -w <gclientdir>
# where <gcliendir> is the absolute path to the directory containing the
# .gclient file (the parent of 'src').
#
# Then commit .DEPS.git locally (gclient doesn't like dirty trees) and run
#   gclient sync
# Verify the thing happened you wanted. Then revert your .DEPS.git change
# DO NOT CHECK IN CHANGES TO .DEPS.git upstream. It will be automatically
# updated by a bot when you modify this one.
#
# When adding a new dependency, please update the top-level .gitignore file
# to list the dependency's destination directory.

vars = {
  'chromium_git': 'https://chromium.googlesource.com',
  'dart_svn': 'https://dart.googlecode.com',
  'skia_revision': '409fd66a5afcef5f165f7ccec7c3473add231752',
  'v8_revision': '230d131d173ab2d60291d303177bc04ec3f6e519',
  'angle_revision': 'bdd419f9f5b006e913606e7363125942c8ae06bc',
  'buildtools_revision': '15308f469a704c45d15567fa69cd94ce07ad0e1b',
  'dart_revision': '856248f1356d1e7564c8b27dcbe35c770cde2eb5',
  'dart_observatory_packages_revision': '45565',
  'pdfium_revision': 'b0115665b0f33971f1b7077740d51e155583cec0',
  'boringssl_revision': '642f1498d056dbba3e50ed5a232ab2f482626dec',
  'lss_revision': 'e079768b7e3a94dcbe7d338496c0c3bde7151b6e',
  'nss_revision': 'bb4e75a43d007518ae7d618665ea2f25b0c60b63',
  'nacl_revision': '87d5dd90911a0657c27574f78e86b7dfc4ad8b29',
}

# Only these hosts are allowed for dependencies in this DEPS file.
# If you need to add a new host, contact chrome infrastructure team.
allowed_hosts = [
  'boringssl.googlesource.com',
  'chromium.googlesource.com',
  'dart.googlecode.com',
  'pdfium.googlesource.com',
]

deps = {
  'src/buildtools':
   Var('chromium_git') + '/chromium/buildtools.git' + '@' +  Var('buildtools_revision'),

  'src/testing/gtest':
   Var('chromium_git') + '/external/googletest.git' + '@' + 'be1868139ffe0ccd0e8e3b37292b84c821d9c8ad', # from svn revision 704

  'src/testing/gmock':
   Var('chromium_git') + '/external/googlemock.git' + '@' + '29763965ab52f24565299976b936d1265cb6a271', # from svn revision 501

  'src/third_party/angle':
   Var('chromium_git') + '/angle/angle.git' + '@' +  Var('angle_revision'),

  'src/third_party/icu':
   Var('chromium_git') + '/chromium/deps/icu.git' + '@' + '7c81740601355556e630da515b74d889ba2f8d08',

  'src/tools/grit':
    Var('chromium_git') + '/external/grit-i18n.git' + '@' + 'c1b1591a05209c1ad467e845ba8543c22f9072af', # from svn revision 189

  'src/v8':
    Var('chromium_git') + '/v8/v8.git' + '@' +  Var('v8_revision'),

  'src/dart':
    Var('chromium_git') + '/external/github.com/dart-lang/sdk.git' + '@' + Var('dart_revision'),

  'src/dart/third_party/observatory_pub_packages':
    Var('dart_svn') + '/svn/third_party/observatory_pub_packages' + '@' +
    Var('dart_observatory_packages_revision'),

  'src/third_party/skia':
   Var('chromium_git') + '/skia.git' + '@' +  Var('skia_revision'),

  'src/third_party/yasm/source/patched-yasm':
   Var('chromium_git') + '/chromium/deps/yasm/patched-yasm.git' + '@' + '4671120cd8558ce62ee8672ebf3eb6f5216f909b',

  'src/third_party/libjpeg_turbo':
   Var('chromium_git') + '/chromium/deps/libjpeg_turbo.git' + '@' + '034e9a9747e0983bc19808ea70e469bc8342081f',

  'src/third_party/smhasher/src':
    Var('chromium_git') + '/external/smhasher.git' + '@' + 'e87738e57558e0ec472b2fc3a643b838e5b6e88f',

  'src/third_party/pywebsocket/src':
    Var('chromium_git') + '/external/pywebsocket/src.git' + '@' + 'cb349e87ddb30ff8d1fa1a89be39cec901f4a29c',

  'src/third_party/mesa/src':
   Var('chromium_git') + '/chromium/deps/mesa.git' + '@' + '071d25db04c23821a12a8b260ab9d96a097402f0',

  'src/third_party/pdfium':
   'https://pdfium.googlesource.com/pdfium.git' + '@' +  Var('pdfium_revision'),

  'src/third_party/boringssl/src':
   'https://boringssl.googlesource.com/boringssl.git' + '@' +  Var('boringssl_revision'),

  'src/third_party/requests/src':
    Var('chromium_git') + '/external/github.com/kennethreitz/requests.git' + '@' + 'f172b30356d821d180fa4ecfa3e71c7274a32de4',

  'src/native_client':
    Var('chromium_git') + '/native_client/src/native_client.git' + '@' + Var('nacl_revision'),
}


deps_os = {
  'android': {
    'src/third_party/colorama/src':
     Var('chromium_git') + '/external/colorama.git' + '@' + '799604a1041e9b3bc5d2789ecbd7e8db2e18e6b8',

    'src/third_party/jsr-305/src':
        Var('chromium_git') + '/external/jsr-305.git' + '@' + '642c508235471f7220af6d5df2d3210e3bfc0919',

    'src/third_party/junit/src':
      Var('chromium_git') + '/external/junit.git' + '@' + '45a44647e7306262162e1346b750c3209019f2e1',

    'src/third_party/mockito/src':
      Var('chromium_git') + '/external/mockito/mockito.git' + '@' + 'ed99a52e94a84bd7c467f2443b475a22fcc6ba8e',

    'src/third_party/robolectric/lib':
      Var('chromium_git') + '/chromium/third_party/robolectric.git' + '@' + '6b63c99a8b6967acdb42cbed0adb067c80efc810',

    'src/third_party/android_tools':
     Var('chromium_git') + '/android_tools.git' + '@' + 'a1ffd63322c438627d78ea56eb73fb8779e06950',

    'src/third_party/appurify-python/src':
     Var('chromium_git') + '/external/github.com/appurify/appurify-python.git' + '@' + 'ee7abd5c5ae3106f72b2a0b9d2cb55094688e867',

    'src/third_party/freetype':
       Var('chromium_git') + '/chromium/src/third_party/freetype.git' + '@' + 'fd6919ac23f74b876c209aba5eaa2be662086391',

    'src/third_party/requests/src':
      Var('chromium_git') + '/external/github.com/kennethreitz/requests.git' + '@' + 'f172b30356d821d180fa4ecfa3e71c7274a32de4',
  },
}


hooks = [
  {
    # This clobbers when necessary (based on get_landmines.py). It must be the
    # first hook so that other things that get/generate into the output
    # directory will not subsequently be clobbered.
    'name': 'landmines',
    'pattern': '.',
    'action': [
        'python',
        'src/build/landmines.py',
    ],
  },
  {
    # Pull clang if needed or requested via GYP_DEFINES.
    'name': 'clang',
    'pattern': '.',
    'action': ['python', 'src/tools/clang/scripts/update.py', '--if-needed'],
  },
  {
    # Pull dart sdk if needed
    'name': 'dart',
    'pattern': '.',
    'action': ['python', 'src/tools/dart/update.py'],
  },
  {
    # This downloads SDK extras and puts them in the
    # third_party/android_tools/sdk/extras directory on the bots. Developers
    # need to manually install these packages and accept the ToS.
    'name': 'sdkextras',
    'pattern': '.',
    # When adding a new sdk extras package to download, add the package
    # directory and zip file to .gitignore in third_party/android_tools.
    'action': ['python', 'src/build/download_sdk_extras.py'],
  },
  {
    # Update LASTCHANGE. This is also run by export_tarball.py in
    # src/tools/export_tarball - please keep them in sync.
    'name': 'lastchange',
    'pattern': '.',
    'action': ['python', 'src/build/util/lastchange.py',
               '-o', 'src/build/util/LASTCHANGE'],
  },
  # Pull GN binaries. This needs to be before running GYP below.
  {
    'name': 'gn_linux32',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/linux32/gn.sha1',
    ],
  },
  {
    'name': 'gn_linux64',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/linux64/gn.sha1',
    ],
  },
  {
    'name': 'gn_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/mac/gn.sha1',
    ],
  },
  {
    'name': 'gn_win',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win*',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/win/gn.exe.sha1',
    ],
  },
  # Pull clang-format binaries using checked-in hashes.
  {
    'name': 'clang_format_linux',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'src/buildtools/linux64/clang-format.sha1',
    ],
  },
  {
    'name': 'clang_format_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'src/buildtools/mac/clang-format.sha1',
    ],
  },
  # Pull sky_server binaries using checked-in hashes.
  {
    'name': 'sky_server_linux',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'mojo',
                '-s', 'src/sky/tools/skygo/linux64/sky_server.sha1',
    ],
  },
  {
    'name': 'sky_server_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'mojo',
                '-s', 'src/sky/tools/skygo/mac/sky_server.sha1',
    ],
  },
  # Pull binutils for linux, enabled debug fission for faster linking /
  # debugging when used with clang on Ubuntu Precise.
  # https://code.google.com/p/chromium/issues/detail?id=352046
  {
    'name': 'binutils',
    'pattern': 'src/third_party/binutils',
    'action': [
        'python',
        'src/third_party/binutils/download.py',
    ],
  },
  # Pull eu-strip binaries using checked-in hashes.
  {
    'name': 'eu-strip',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-eu-strip',
                '-s', 'src/build/linux/bin/eu-strip.sha1',
    ],
  },
  # Pull the prebuilt network service binaries according to
  # mojo/public/tools/NETWORK_SERVICE_VERSION.
  {
    'name': 'download_network_service',
    'pattern': '',
    'action': [ 'python',
                'src/mojo/public/tools/download_network_service.py',
                '--tools-directory', '../../../tools',
    ],
  },
  # Run "pub get" on any directories with checked-in pubspec.yaml files
  # (excluding sky/, whose pubspec.yaml files are not intended for supporting
  # building in-place in the repo).
  {
    'name': 'run_dart_pub_get',
    'pattern': '',
    'action': [ 'python',
                'src/mojo/public/tools/git/dart_pub_get.py',
                '--repository-root', '../../../..',
                '--dart-sdk-directory',
                '../../../../third_party/dart-sdk/dart-sdk',
                '--dirs-to-ignore', 'sky/',
    ],
  },
  {
    # Ensure that we don't accidentally reference any .pyc files whose
    # corresponding .py files have already been deleted.
    'name': 'remove_stale_pyc_files',
    'pattern': 'src/tools/.*\\.py',
    'action': [
        'python',
        'src/tools/remove_stale_pyc_files.py',
        'src/tools',
    ],
  },
  {
    # This downloads binaries for Native Client's newlib toolchain.
    # Done in lieu of building the toolchain from scratch as it can take
    # anywhere from 30 minutes to 4 hours depending on platform to build.
    'name': 'nacltools',
    'pattern': '.',
    'action': [
        'python', 'src/build/download_nacl_toolchains.py',
        '--packages', 'pnacl_newlib',
        'sync', '--extract',
    ],
  },
  {
    # This downloads linux and android Go binaries according to
    # tools/go/VERSION.
    'name': 'gotools',
    'pattern': '.',
    'action': [
        'python', 'src/tools/go/download.py',
    ],
  },
  # Pull DejaVu fonts using checked-in hashes.
  {
    'name': 'dejavu-fonts',
    'pattern': '',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--no_auth',
                '--bucket', 'mojo/dejavu-fonts',
                '-s', 'src/third_party/dejavu-fonts-ttf-2.34/ttf/DejaVuSansMono.ttf.sha1',
    ],
  },
]
