/* gsound-attr.h
 * 
 * Copyright (C) 2014 Tristan Brindle <t.c.brindle@gmail.com>
 * 
 * Adapted from canberra.h,
 * 
 * Copyright 2008 Lennart Poettering 
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

#ifndef GSOUND_ATTR_H
#define GSOUND_ATTR_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * SECTION:gsound-attr
 * @title: GSound Attributes
 * @short_description: Attributes recognized by GSound
 * @see_also: #GSoundContext
 * 
 * Attributes which can be applied to a #GSoundContext or passed to one of
 * the `play()` or `cache()` methods.
 */

/**
 * GSOUND_ATTR_MEDIA_NAME:
 *
 * A name describing the media being played. Localized if possible and applicable.
 */
#define GSOUND_ATTR_MEDIA_NAME                         "media.name"

/**
 * GSOUND_ATTR_MEDIA_TITLE:
 *
 * A (song) title describing the media being played. Localized if possible and applicable.
 */
#define GSOUND_ATTR_MEDIA_TITLE                        "media.title"

/**
 * GSOUND_ATTR_MEDIA_ARTIST:
 *
 * The artist of this media. Localized if possible and applicable.
 */
#define GSOUND_ATTR_MEDIA_ARTIST                       "media.artist"

/**
 * GSOUND_ATTR_MEDIA_LANGUAGE:
 *
 * The language this media is in, in some standard POSIX locale string, such as "de_DE".
 */
#define GSOUND_ATTR_MEDIA_LANGUAGE                     "media.language"

/**
 * GSOUND_ATTR_MEDIA_FILENAME:
 *
 * The file name this media was or can be loaded from.
 */
#define GSOUND_ATTR_MEDIA_FILENAME                     "media.filename"

/**
 * GSOUND_ATTR_MEDIA_ICON:
 *
 * An icon for this media in binary PNG format.
 */
#define GSOUND_ATTR_MEDIA_ICON                         "media.icon"

/**
 * GSOUND_ATTR_MEDIA_ICON_NAME:
 *
 * An icon name as defined in the XDG icon naming specifcation.
 */
#define GSOUND_ATTR_MEDIA_ICON_NAME                    "media.icon_name"

/**
 * GSOUND_ATTR_MEDIA_ROLE:
 *
 * The "role" this media is played in. For event sounds the string
 * "event". For other cases strings like "music", "video", "game", ...
 */
#define GSOUND_ATTR_MEDIA_ROLE                         "media.role"

/**
 * GSOUND_ATTR_EVENT_ID:
 *
 * A textual id for an event sound, as mandated by the XDG sound naming specification.
 */
#define GSOUND_ATTR_EVENT_ID                           "event.id"

/**
 * GSOUND_ATTR_EVENT_DESCRIPTION:
 *
 * A descriptive string for the sound event. Localized if possible and applicable.
 */
#define GSOUND_ATTR_EVENT_DESCRIPTION                  "event.description"

/**
 * GSOUND_ATTR_EVENT_MOUSE_X:
 *
 * If this sound event was triggered by a mouse input event, the X
 * position of the mouse cursor on the screen, formatted as string.
 */
#define GSOUND_ATTR_EVENT_MOUSE_X                      "event.mouse.x"

/**
 * GSOUND_ATTR_EVENT_MOUSE_Y:
 *
 * If this sound event was triggered by a mouse input event, the Y
 * position of the mouse cursor on the screen, formatted as string.
 */
#define GSOUND_ATTR_EVENT_MOUSE_Y                      "event.mouse.y"

/**
 * GSOUND_ATTR_EVENT_MOUSE_HPOS:
 *
 * If this sound event was triggered by a mouse input event, the X
 * position of the mouse cursor as fractional value between 0 and 1,
 * formatted as string, 0 reflecting the left side of the screen, 1
 * the right side.
 */
#define GSOUND_ATTR_EVENT_MOUSE_HPOS                   "event.mouse.hpos"

/**
 * GSOUND_ATTR_EVENT_MOUSE_VPOS:
 *
 * If this sound event was triggered by a mouse input event, the Y
 * position of the mouse cursor as fractional value between 0 and 1,
 * formatted as string, 0 reflecting the top end of the screen, 1
 * the bottom end.
 */
#define GSOUND_ATTR_EVENT_MOUSE_VPOS                   "event.mouse.vpos"

/**
 * GSOUND_ATTR_EVENT_MOUSE_BUTTON:
 *
 * If this sound event was triggered by a mouse input event, the
 * number of the mouse button that triggered it, formatted as string. 1
 * for left mouse button, 3 for right, 2 for middle.
 */
#define GSOUND_ATTR_EVENT_MOUSE_BUTTON                 "event.mouse.button"

/**
 * GSOUND_ATTR_WINDOW_NAME:
 *
 * If this sound event was triggered by a window on the screen, the
 * name of this window as human readable string.
 */
#define GSOUND_ATTR_WINDOW_NAME                        "window.name"

/**
 * GSOUND_ATTR_WINDOW_ID:
 *
 * If this sound event was triggered by a window on the screen, some
 * identification string for this window, so that the sound system can
 * recognize specific windows.
 */
#define GSOUND_ATTR_WINDOW_ID                          "window.id"

/**
 * GSOUND_ATTR_WINDOW_ICON:
 *
 * If this sound event was triggered by a window on the screen, binary
 * icon data in PNG format for this window.
 */
#define GSOUND_ATTR_WINDOW_ICON                        "window.icon"

/**
 * GSOUND_ATTR_WINDOW_ICON_NAME:
 *
 * If this sound event was triggered by a window on the screen, an
 * icon name for this window, as defined in the XDG icon naming
 * specification.
 */
#define GSOUND_ATTR_WINDOW_ICON_NAME                   "window.icon_name"

/**
 * GSOUND_ATTR_WINDOW_X:
 *
 * If this sound event was triggered by a window on the screen, the X
 * position of the window measured from the top left corner of the
 * screen to the top left corner of the window.
 */
#define GSOUND_ATTR_WINDOW_X                           "window.x"

/**
 * GSOUND_ATTR_WINDOW_Y:
 *
 * If this sound event was triggered by a window on the screen, the y
 * position of the window measured from the top left corner of the
 * screen to the top left corner of the window.
 */
#define GSOUND_ATTR_WINDOW_Y                           "window.y"

/**
 * GSOUND_ATTR_WINDOW_WIDTH:
 *
 * If this sound event was triggered by a window on the screen, the
 * pixel width of the window.
 */
#define GSOUND_ATTR_WINDOW_WIDTH                       "window.width"

/**
 * GSOUND_ATTR_WINDOW_HEIGHT:
 *
 * If this sound event was triggered by a window on the screen, the
 * pixel height of the window.
 */
#define GSOUND_ATTR_WINDOW_HEIGHT                      "window.height"

/**
 * GSOUND_ATTR_WINDOW_HPOS:
 *
 * If this sound event was triggered by a window on the screen, the X
 * position of the center of the window as fractional value between 0
 * and 1, formatted as string, 0 reflecting the left side of the
 * screen, 1 the right side.
 */
#define GSOUND_ATTR_WINDOW_HPOS                        "window.hpos"

/**
 * GSOUND_ATTR_WINDOW_VPOS:
 *
 * If this sound event was triggered by a window on the screen, the Y
 * position of the center of the window as fractional value between 0
 * and 1, formatted as string, 0 reflecting the top side of the
 * screen, 1 the bottom side.
 */
#define GSOUND_ATTR_WINDOW_VPOS                        "window.vpos"

/**
 * GSOUND_ATTR_WINDOW_DESKTOP:
 *
 * If this sound event was triggered by a window on the screen and the
 * windowing system supports multiple desktops, a comma seperated list
 * of indexes of the desktops this window is visible on. If this
 * attribute is an empty string, it is visible on all desktops
 * (i.e. 'sticky'). The first desktop is 0. (e.g. "0,2,3")
 */
#define GSOUND_ATTR_WINDOW_DESKTOP                    "window.desktop"

/**
 * GSOUND_ATTR_WINDOW_X11_DISPLAY:
 *
 * If this sound event was triggered by a window on the screen and the
 * windowing system is X11, the X display name of the window (e.g. ":0").
 */
#define GSOUND_ATTR_WINDOW_X11_DISPLAY                 "window.x11.display"

/**
 * GSOUND_ATTR_WINDOW_X11_SCREEN:
 *
 * If this sound event was triggered by a window on the screen and the
 * windowing system is X11, the X screen id of the window formatted as
 * string (e.g. "0").
 */
#define GSOUND_ATTR_WINDOW_X11_SCREEN                  "window.x11.screen"

/**
 * GSOUND_ATTR_WINDOW_X11_MONITOR:
 *
 * If this sound event was triggered by a window on the screen and the
 * windowing system is X11, the X monitor id of the window formatted as
 * string (e.g. "0").
 */
#define GSOUND_ATTR_WINDOW_X11_MONITOR                 "window.x11.monitor"

/**
 * GSOUND_ATTR_WINDOW_X11_XID:
 *
 * If this sound event was triggered by a window on the screen and the
 * windowing system is X11, the XID of the window formatted as string.
 */
#define GSOUND_ATTR_WINDOW_X11_XID                     "window.x11.xid"

/**
 * GSOUND_ATTR_APPLICATION_NAME:
 *
 * The name of the application this sound event was triggered by as
 * human readable string. (e.g. "GNU Emacs") Localized if possible and
 * applicable.
 * 
 * > This attribute will automatically be added to the #GSoundContext if
 * > it has previously been set with g_set_application_name(), so you normally
 * > do not need to supply this yourself.
 */
#define GSOUND_ATTR_APPLICATION_NAME                   "application.name"

/**
 * GSOUND_ATTR_APPLICATION_ID:
 *
 * An identifier for the program this sound event was triggered
 * by. (e.g. "org.gnu.emacs").
 * 
 * > This attribute will automatically be added to the #GSoundContext with
 * > the #GApplication:application-id if you are using #GApplication, so you
 * > normally do not need to supply this yourself.
 */
#define GSOUND_ATTR_APPLICATION_ID                     "application.id"

/**
 * GSOUND_ATTR_APPLICATION_VERSION:
 *
 * A version number for the program this sound event was triggered
 * by. (e.g. "22.2")
 */
#define GSOUND_ATTR_APPLICATION_VERSION                "application.version"

/**
 * GSOUND_ATTR_APPLICATION_ICON:
 *
 * Binary icon data in PNG format for the application this sound event
 * is triggered by.
 */
#define GSOUND_ATTR_APPLICATION_ICON                   "application.icon"

/**
 * GSOUND_ATTR_APPLICATION_ICON_NAME:
 *
 * An icon name for the application this sound event is triggered by,
 * as defined in the XDG icon naming specification.
 */
#define GSOUND_ATTR_APPLICATION_ICON_NAME              "application.icon_name"

/**
 * GSOUND_ATTR_APPLICATION_LANGUAGE:
 *
 * The locale string the application that is triggering this sound
 * event is running in. A POSIX locale string such as de_DE@euro.
 */
#define GSOUND_ATTR_APPLICATION_LANGUAGE               "application.language"

/**
 * GSOUND_ATTR_APPLICATION_PROCESS_ID:
 *
 * The unix PID of the process that is triggering this sound event, formatted as string.
 */
#define GSOUND_ATTR_APPLICATION_PROCESS_ID             "application.process.id"

/**
 * GSOUND_ATTR_APPLICATION_PROCESS_BINARY:
 *
 * The path to the process binary of the process that is triggering this sound event.
 */
#define GSOUND_ATTR_APPLICATION_PROCESS_BINARY         "application.process.binary"

/**
 * GSOUND_ATTR_APPLICATION_PROCESS_USER:
 *
 * The user that owns the process that is triggering this sound event.
 */
#define GSOUND_ATTR_APPLICATION_PROCESS_USER           "application.process.user"

/**
 * GSOUND_ATTR_APPLICATION_PROCESS_HOST:
 *
 * The host name of the host the process that is triggering this sound event runs on.
 */
#define GSOUND_ATTR_APPLICATION_PROCESS_HOST           "application.process.host"

/**
 * GSOUND_ATTR_CANBERRA_CACHE_CONTROL:
 *
 * A special attribute that can be used to control the automatic sound
 * caching of sounds in the sound server. One of "permanent",
 * "volatile", "never". "permanent" will cause this sample to be
 * cached in the server permanently. This is useful for very
 * frequently used sound events such as those used for input
 * feedback. "volatile" may be used for cacheing sounds in the sound
 * server temporarily. They will expire after some time or on cache
 * pressure. Finally, "never" may be used for sounds that should never
 * be cached, because they are only generated very seldomly or even
 * only once at most (such as desktop login sounds).
 *
 * If this attribute is not explicitly passed to gsound_context_play_simple() 
 * or gsound_context_play_full() it will default to "never". If it is not
 * explicitly passed to gsound_context_cache() it will default to "permanent".
 *
 * If the list of attributes is handed on to the sound server this
 * attribute is stripped from it.
 */
#define GSOUND_ATTR_CANBERRA_CACHE_CONTROL             "canberra.cache-control"

/**
 * GSOUND_ATTR_CANBERRA_VOLUME:
 *
 * A special attribute that can be used to control the volume this
 * sound event is played in if the backend supports it. A floating
 * point value for the decibel multiplier for the sound. 0 dB relates
 * to zero gain, and is the default volume these sounds are played in.
 *
 * If the list of attributes is handed on to the sound server this
 * attribute is stripped from it.
 */
#define GSOUND_ATTR_CANBERRA_VOLUME                    "canberra.volume"

/**
 * GSOUND_ATTR_CANBERRA_XDG_THEME_NAME:
 *
 * A special attribute that can be used to control the XDG sound theme that
 * is used for this sample.
 *
 * If the list of attributes is handed on to the sound server this
 * attribute is stripped from it.
 */
#define GSOUND_ATTR_CANBERRA_XDG_THEME_NAME            "canberra.xdg-theme.name"

/**
 * GSOUND_ATTR_CANBERRA_XDG_THEME_OUTPUT_PROFILE:
 *
 * A special attribute that can be used to control the XDG sound theme
 * output profile that is used for this sample.
 *
 * If the list of attributes is handed on to the sound server this
 * attribute is stripped from it.
 */
#define GSOUND_ATTR_CANBERRA_XDG_THEME_OUTPUT_PROFILE  "canberra.xdg-theme.output-profile"

/**
 * GSOUND_ATTR_CANBERRA_ENABLE:
 *
 * A special attribute that can be used to control whether any sounds
 * are played at all. If this attribute is "1" or unset sounds are
 * played as normal. However, if it is "0" all calls to
 * gsound_context_play_simple() or `play_full()` will fail with
 * GSOUND_ERROR_DISABLED.
 *
 * If the list of attributes is handed on to the sound server this
 * attribute is stripped from it.
 */
#define GSOUND_ATTR_CANBERRA_ENABLE                    "canberra.enable"

/**
 * GSOUND_ATTR_CANBERRA_FORCE_CHANNEL:
 *
 * A special attribute that can be used to control on which channel a
 * sound is played. The value should be one of mono, front-left,
 * front-right, front-center, rear-left, rear-right, rear-center, lfe,
 * front-left-of-center, front-right-of-center, side-left, side-right,
 * top-center, top-front-left, top-front-right, top-front-center,
 * top-rear-left, top-rear-right, top-rear-center. This attribute is
 * only honoured by some backends, other backends may choose to ignore
 * it completely.
 *
 * If the list of attributes is handed on to the sound server this
 * attribute is stripped from it.
 */
#define GSOUND_ATTR_CANBERRA_FORCE_CHANNEL            "canberra.force_channel"



G_END_DECLS

#endif /* GSOUND_ATTR_H */
