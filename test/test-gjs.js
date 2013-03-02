#!/usr/bin/gjs

const GLib = imports.gi.GLib;
const Gio = imports.gi.Gio;
const GSound = imports.gi.GSound;

GLib.set_application_name("GSound test");

let mainloop = imports.mainloop;
let ctx = new GSound.Context();
ctx.init(null);

ctx.set_driver("pulse");

//ctx.cache({ "media.filename" : "/usr/share/sounds/alsa/Front_Left.wav"  });

let cancellable = new Gio.Cancellable();
ctx.play_simple({ "media.filename" : "/usr/share/sounds/alsa/Front_Left.wav"  },
                cancellable);

ctx.play_full({ "media.filename" : "/usr/share/sounds/alsa/Front_Center.wav"  },
             cancellable,
             function (ctx, res) {
                try {
                    ctx.play_full_finish(res);
                } catch (e) {
                    print(e);
                } finally {
             //       mainloop.quit('');
                }
             });

//cancellable.cancel();
mainloop.run('');

