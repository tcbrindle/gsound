#!/usr/bin/env python

from gi.repository import GLib
from gi.repository import Gio
from gi.repository import GSound

loop = GLib.MainLoop()
cancellable = Gio.Cancellable()

def on_play_full_finished(ctx, result, unused):
    # This should report "cancelled"
    try:
        ctx.play_full_finish(result)
    finally:
        loop.quit()

def cancel_playing():
    cancellable.cancel()

try:
    ctx = GSound.Context()
    ctx.init(None)
    #ctx.cache({ GSound.ATTR_MEDIA_FILENAME : "/usr/share/sounds/alsa/Front_Left.wav" });
    ctx.play_simple({ GSound.ATTR_MEDIA_FILENAME : "/usr/share/sounds/alsa/Front_Left.wav" },
                    cancellable)
    
    ctx.play_full({ GSound.ATTR_MEDIA_FILENAME : "/usr/share/sounds/alsa/Front_Center.wav" },
                  cancellable, on_play_full_finished, None)
    
    GLib.timeout_add_seconds(1, cancel_playing)
    loop.run()
except GSound.Error:
    print("Bugger")


