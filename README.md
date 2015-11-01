GSound
======

What is it?
-----------

GSound is a small library for playing system sounds. It's designed to be
used via GObject Introspection, and is a thin wrapper around the [libcanberra](http://0pointer.de/lennart/projects/libcanberra/) C library.

Homepage: https://wiki.gnome.org/Projects/GSound

Documentation: https://developer.gnome.org/gsound/stable

Source: https://git.gnome.org/browse/gsound

Bugs: https://bugzilla.gnome.org/enter_bug.cgi?product=gsound

##Usage via GObject Introspection.

As GSound is based on libcanberra it shares a similar API. To use it,
you first create and initialise a GSound context. You then pass the
context a list of (attribute, value) pairs instructing it what to play,
like so in Python:

```Python
from gi.repository import GSound

try:
    ctx = GSound.Context()
    ctx.init()
    ctx.play_simple({ GSound.ATTR_EVENT_ID : "phone-incoming-call" })
except:
    # Handle errors
    pass
```

or the equivalent in JavaScript (using GJS)

```JavaScript
const GSound = imports.gi.GSound;

let ctx = new GSound.Context();

try {
    ctx.init();
    // For some reason I can't seem to use the attribute defines in GJS
    ctx.play_simple({ "event.id" : "phone-incoming-call" }, null);
} catch (e) {
    // handle error
}
```

The list of supported attributes can be found in 
[gsound-attr.h](https://developer.gnome.org/gsound/stable/gsound-GSound-Attributes.html),
and can be used via the `GSound.ATTR_*` string constants if the target
language supports this.

Playing Sounds
--------------

There are two very similar play commands, `GSound.Context.play_simple()`
and the corresponding `play_full()`.

The "full" version is an asynchronous function following the GIO model.
It takes a callback argument which will be called when the sound has
finished playing. As with other GIO async functions, you should call
`GSound.Context.play_finish()` within the callback in order to receive
any exceptions which might have occurred during playback; note that
cancelling playback will result in a `Gio.IOError.CANCELLED` exception
(or something similar).

On the other hand, `play_simple()` does not give you any feedback when
the sound finishes. However, it will still report any errors which
occurred before the sound was submitted to the server (with
`play_full()`, these are reported in the callback).

It's important to note that both versions are non-blocking and will
return control to your application immediately, without waiting for the
sound to finish playing (or even start, really).

Usage in Vala
-------------

GSound can be used in Vala via the included VAPI file. The API is
slightly different from other introspected languages. Since Vala supports
varargs, these are used to pass attribute-value pairs rather than
`GHashTable`s. One neat feature as that since `play_full()` is a proper
async function, it can be used with `yield` like so:

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

(Note that libcanberra comes with it's own VAPI file, which you could use
instead.)

Usage in C
-----------

As a C library, GSound can of course be used from C and C++. If your project
is already using  GObject libraries then you'll find the style fits right in.
Like Vala, the C API uses varargs to pass attributes to the backend, for
example (error checking omitted for brevity):

```C
GSoundContext *ctx = gsound_context_new(NULL, NULL);

gsound_context_play_simple(ctx, NULL, NULL;
                           GSOUND_ATTR_EVENT_ID, "phone-incoming-call",
                           NULL);
```

License
-------

> This program is free software; you can redistribute it and/or modify it
> under the terms of the GNU Lesser General Public License as published
> by the Free Software Foundation, either version 2.1 of the License, or
> (at your option) any later version.

> This program is distributed in the hope that it will be useful, but
> WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
> General Public License for more details.

Prerequisites
-------------

Building GSound from git requires a fairly modern Gnome development
environment; so far it's only been tested on Fedora 20. At a minimum,
you'll need headers for GObject (at least 2.36.0) and libcanberra, as well
as the needed machinery to generate GObject introspection data and the Vala
VAPI. 

Differences from libcanberra
----------------------------

GSound wraps the libcanberra API very closely, with the following
differences:

1) Attribute/value pairs are passed using `GHashTable`s instead of
   `ca_proplists`. This is because most GI consumers have automatic conversion
   from native associative array types (e.g. Python's `dict`) to `GHashTable`.
   The Vala and C APIs instead use varargs, as libcanberra does in those
   languages.

2) Errors are reported using `GError`s rather than using integer return values.
   Most GI consumers map `GError`s to native exceptions, and they work mostly like
   exceptions in Vala too.

3) Callbacks use the standard GIO async pattern (i.e. call a `finish()` function
   on a `GAsyncResult` that's handed to a callback you supply).

4) `GCancellable`s are used for cancelling sounds in progress, again for
   consistency with GIO.

5) What libcanberra calls "properties" are called "attributes" in GSound to
   avoid confusing them with GObject properties. The actual strings (e.g.
   "media.filename") are the same, however.
