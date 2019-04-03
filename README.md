# TestMaker Lite 
An open-source test system that is compatible with TestMaker Plus.
Copyright (C) 2019 Tim K <timprogrammer@rambler.ru>. Licensed under GNU GPLv3.

## Features
- Full compatibility with tests created with TestMaker Plus 1.1 and 1.5 (it even supports network tests)
- Non-cluttered code
- Lower memory consumption than TestMaker Plus
- Source code portability
- Depends only on Qt 5 and C++ STL libraries
- It's open-source unlike its predecessor

## Building
Building is as simple as running:
```
$ qmake
$ make
$ make install
```

## Usage
TestMaker Lite is just a test system and it does not include a test editing program. So to create new tests, you'll have to use TestMaker Plus Editor 1.1 or 1.5.

Opening existing tests can be done by just running ``testmakerlite`` and selecting the test manually or by running ``testmakerlite`` from the command-line with the ``-f`` argument:
```
$ testmakerlite -f /path/to/test.csv
```

For LAN/network tests, run ``testmakerlite`` with the ``-network`` CLI option:
```
$ testmakerlite -network http://server:port/path/to/test.csv
```

If you want to take a test full-screen, run ``testmakerlite`` with the ``-fullscreen`` option:
```
$ testmakerlite -fullscreen
```

That's it!
