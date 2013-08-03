#GSound

##What is it?

GSound is a small library for playing system sounds. It's designed to be used via GObject Introspection, and is a thin wrapper around the [libcanberra](http://0pointer.de/lennart/projects/libcanberra/) C library.

##Usage via GObject Introspection.

As GSound is based on libcanberra it shares a similar API. To use it, you first create and initialise a GSound context. You then pass the context a list of (attribute, value) pairs instructing it what to play, like so in GJS:

```JavaScript
const GSound = imports.gi.GSound;

let ctx = new GSound.Context();

try {
    ctx.init();
    ctx.play_simple(null, { "media.filename" : "/an/example/file.ogg" });
} catch (e) {
    // handle errors
}
```

or, in Python

```Python
from gi.repository import GSound

try:
    ctx = GSound.Context()
    ctx.init()
    ctx.play_simple(None, { "media.filename" : "/an/example/file.ogg" })
except:
    # Handle errors
    pass
```

The list of supported attributes can be found in [gsound.h](https://github.com/tcbrindle/gsound/blob/master/gsound/gsound.h), and can be used via GSound.ATTR_* string constants if the target language supports this.

There are two very similar play commands, `GSound.Context.play_simple()` and the corresponding `play_full()`.

The "full" version is an asynchronous function following the GIO model. It takes a callback argument which will be called when the sound has finished playing. As with other GIO async functions, you should call `GSound.Context.play_finish()` within the callback in order to receive any exceptions which might have occurred during playback; note that cancelling playback will result in a `Gio.IOError.CANCELLED` exception (or something similar).

It's important to note that both versions are non-blocking and will return control to your application immediately, without waiting for the sound to finish playing (or even start, really). The main difference is that with the "simple" version you have no way of catching errors that may occur during playback -- it is intended to be "fire and forget".

##Usage in Vala

GSound can be used in Vala via the included VAPI file. It uses C-style varargs for passing attribute/value pairs, but is otherwise the same as the above. One neat feature as that since `play_full()` is a proper async function, it can be used with `yield` like so:


```Vala
public async void play(string filename,
                       GLib.Cancellable? cancellable) throws GLib.Error
{
        var ctx = new GSound.Context();
        ctx.init();
        yield ctx.play_full(cancellable,
                            GSound.Attributes.MEDIA_FILENAME, filename);
}
```

(Note that libcanberra comes with it's own VAPI file, which you could of course use instead of GSound if you prefer.)

##Usage in C

Even though it's a C library, GSound really doesn't offer anything to C users that isn't already easier by using libcanberra directly, except quite a bit of extra typing. Of course, it's perfectly possible to do so if you want to.

##License:

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
    General Public License for more details.

#Prerequisites:

Building GSound from git requires a fairly modern Gnome development environment; so far it's only been tested on Fedora 19. At a minimum, you'll need headers 
for GObject (at least 2.36.0) and libcanberra, as well as the needed machinery to generate GObject introspection data and the Vala VAPI. 

#Differences from libcanberra

GSound wraps the libcanberra API very closely, with the following differences:

1) Attribute/value pairs are passed using `GHashTable`s instead of `ca_proplists`. This is because most GI consumers have automatic conversion from native associative array types (e.g. Python's `dict`) to `GHashTable`. The Vala and C APIs instead use varargs, as libcanberra does in those languages.

2) Errors are reported using GErrors rather than using integer return values. Most GI consumers map GErrors to native exceptions, and they work mostly like exceptions in Vala too.

3) Callbacks use the standard GIO async pattern (i.e. call a finish() function on a GAsyncResult that's handed to a callback you supply).

4) GCancellables are used for cancelling sounds in progress, again for consistency with GIO.

5) What libcanberra calls "properties" are called "attributes" in GSound to avoid confusing them with GObject properties. The actual strings (e.g. "media.filename") are the same, however.


