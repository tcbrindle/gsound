/* gsound-play.vala
 *
 * Copyright (C) 2014 Tristan Brindle <t.c.brindle@gmail.com>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

string event_id;
string filename;
string desc;
string cache;
int loops;
double volume;
string driver;

MainLoop main_loop;
GSound.Context gs_ctx;
HashTable<string, string> attrs;

const OptionEntry[] opts = {
    { "id", 'i', 0, OptionArg.STRING, ref event_id,
    "Event sound identifier", "STRING" },
    { "file", 'f', 0, OptionArg.FILENAME, ref filename,
    "Play file", "PATH" },
    { "description", 'd', 0, OptionArg.STRING, ref desc,
    "Event sound description", "STRING" },
    { "cache-control", 'c', 0, OptionArg.STRING, ref cache,
    "Cache control (permanent, volatile, never", "STRING" },
    { "loop", 'l', 0, OptionArg.INT, ref loops,
    "Loop many times (default: 1)", "INTEGER" },
    { "volume", 'V', 0, OptionArg.DOUBLE, ref volume,
    "A floating point dB value for the sample volume (ex: 0.0)", "STRING" },
    { "backend", 'b', 0, OptionArg.STRING, ref driver,
    "libcanberra backend to use", "STRING" },
    { null }
};

async void play() throws Error
{
    while (loops-- > 0) {
        yield gs_ctx.play_fullv(attrs, null);
    }
}

int main(string[] args)
{
    Environment.set_application_name("gsound-play");
    
    var opt_ctx = new OptionContext();
    opt_ctx.add_main_entries(opts, null);
    
    try {
        opt_ctx.parse(ref args);
        
        if (event_id == null && filename == null) {
            print("No event id or file specified.\n");
            return 1;
        }
        
        gs_ctx = new GSound.Context();
        gs_ctx.set_attributes(GSound.Attribute.APPLICATION_ID, "org.gnome.gsound-test");
        
        if (driver != null) {
            gs_ctx.set_driver(driver);
        }
        
        attrs = new HashTable<string, string>(str_hash, str_equal);
        
        if (event_id != null) {
            attrs.insert(GSound.Attribute.EVENT_ID, event_id);
        }
        if (filename != null) {
            attrs.insert(GSound.Attribute.MEDIA_FILENAME, filename);
        }
        if (cache != null) {
            attrs.insert(GSound.Attribute.CANBERRA_CACHE_CONTROL, cache);
        }
        if (volume != 0.0) {
            attrs.insert(GSound.Attribute.CANBERRA_VOLUME, volume.to_string());
        }
        
        if (loops == 0) {
            loops = 1;
        }
        
        play.begin((obj, res) => {
            try {
                play.end(res);
            } catch (Error e) {
                print("Error: %s\n", e.message);
            } finally {
                main_loop.quit();
            }
        });
        
        main_loop = new MainLoop();
        main_loop.run();

    } catch (Error e) {
        print("Error: %s\n", e.message);
        return 1;
    }
    
    return 0;
}

