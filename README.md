# bold-hour

This is a [pebble](http://getpebble.com) watchface which displays a large, watch-face-sized, bold hour and a smaller minute number in the center of the hour.

## My Pebble Faces

Get and Vote for this face on mypebblefaces.com:
- [white numbers](http://mypebblefaces.com/view?fID=700&aName=yanatan16&pageTitle=Bold+Hour+%28White%29&auID=598)
- [black numbers](http://mypebblefaces.com/view?fID=698&aName=yanatan16&pageTitle=Bold+Hour+%28Black%29&auID=598)

## How to build

This watchface was compiled with Android SDK release 1. Make sure you setup the SDK according to [pebble's instructions](http://developer.getpebble.com). Then clone this repo (or your fork) into the watches directory. Then add the symlinks.

```
cd ~/pebble-dev/pebble-sdk-release-001/watches
git clone http://github.com/yanatan16/pebble-bold-hour bold-hour
../tools/create_pebble_project.py --symlink-only ../sdk bold-hour
```

To set it up for white numbers make sure the `minuteLayer` color is `GColorWhite`, and make sure all the images in the resoure map have the `-inv.png` on the end, as in `bold-hour-5-inv.png`.

## Screenshot

![Bold Hour Watchface Screenshot](http://jeisen.github.com/pebble-bold-hour/img/screenshot.jpg)

## License

Copyright (c) 2013 Jon Eisen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
