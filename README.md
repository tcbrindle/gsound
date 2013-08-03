#GSound

##What is it?

GSound is a small library for playing system sounds. It's designed to be used via GObject Introspection, and is a thin wrapper around the [libcanberra](http://0pointer.de/lennart/projects/libcanberra/) C library.

##Usage via GObject Introspection.

GSound is based on libcanberra, and shares a similar API. To use it, you first create a GSound context, and then pass a list of (attribute, value) pairs instructing it what to play, like so in GJS:

```JavaScript
const GSound = imports.gi.GSound;

let ctx = new GSound.Context();

try {
    ctx.init();
    ctx.play_simple(null,
                            { "media.filename" : "/an/example/file.ogg" });
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
    ctx.play_simple(None, { "media.filename" : "/an/example/file.ogg" }
except:
    # Handle errors
    pass
```

There are two play commands: GSound.Context.play_simple() is a fire-and-forget method which returns control to your application immediately and plays the sound in the background. GSound.Context.play_full() does the same thing, but additionally takes a callback which will be fired when the sound has finished playing, in the standard GIO async pattern.

## Usage in Vala

GSound comes with a VAPI file which offers a slightly nicer Vala API than using the GIR directly. For example:

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

