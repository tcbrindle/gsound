/* gsound-context.h
 *
 * Copyright (C) 2013 Tristan Brindle <t.c.brindle@gmail.com>
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

#ifndef GSOUND_CONTEXT_H
#define GSOUND_CONTEXT_H

#include <gio/gio.h>

#include "gsound-attr.h"

G_BEGIN_DECLS
#define GSOUND_TYPE_CONTEXT              (gsound_context_get_type ())
#define GSOUND_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSOUND_TYPE_CONTEXT, GSoundContext))
#define GSOUND_CONTEXT_CLASS(obj)        (G_TYPE_CHECK_CLASS_CAST ((obj), GSOUND_TYPE_CONTEXT, GSoundContextClass))
#define GSOUND_IS_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSOUND_TYPE_CONTEXT))
#define GSOUND_IS_CONTEXT_CLASS(obj)     (G_TYPE_CHECK_CLASS_TYPE ((obj), GSOUND_TYPE_CONTEXT))
#define GSOUND_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GSOUND_TYPE_CONTEXT, GSoundContextClass))
typedef struct _GSoundContext GSoundContext;
typedef struct _GSoundContextClass GSoundContextClass;

/**
 * GSoundContext:
 * ca: the wrapped context
 *
 * Wrapper for ca_context.
 */

#define GSOUND_ERROR (gsound_error_quark())
GQuark gsound_error_quark(void);

typedef enum
{
    GSOUND_ERROR_NOTSUPPORTED = -1,
    GSOUND_ERROR_INVALID = -2,
    GSOUND_ERROR_STATE = -3,
    GSOUND_ERROR_OOM = -4,
    GSOUND_ERROR_NODRIVER = -5,
    GSOUND_ERROR_SYSTEM = -6,
    GSOUND_ERROR_CORRUPT = -7,
    GSOUND_ERROR_TOOBIG = -8,
    GSOUND_ERROR_NOTFOUND = -9,
    GSOUND_ERROR_DESTROYED = -10,
    GSOUND_ERROR_CANCELED = -11,
    GSOUND_ERROR_NOTAVAILABLE = -12,
    GSOUND_ERROR_ACCESS = -13,
    GSOUND_ERROR_IO = -14,
    GSOUND_ERROR_INTERNAL = -15,
    GSOUND_ERROR_DISABLED = -16,
    GSOUND_ERROR_FORKED = -17,
    GSOUND_ERROR_DISCONNECTED = -18
} GSoundError;
GType             gsound_context_get_type          (void);

GSoundContext    *gsound_context_new               (GCancellable  *cancellable,
                                                    GError       **error);

gboolean          gsound_context_open              (GSoundContext  *context,
                                                    GError        **error);

gboolean          gsound_context_set_attributes    (GSoundContext  *context,
                                                    GError        **error,
                                                    ...) G_GNUC_NULL_TERMINATED;

gboolean          gsound_context_set_attributesv   (GSoundContext  *context,
                                                    GHashTable     *attrs,
                                                    GError        **error);

gboolean          gsound_context_set_driver        (GSoundContext  *context,
                                                    const char     *driver,
                                                    GError        **error);

gboolean          gsound_context_play_simple       (GSoundContext  *context,
                                                    GCancellable   *cancellable,
                                                    GError        **error,
                                                    ...) G_GNUC_NULL_TERMINATED;

gboolean          gsound_context_play_simplev      (GSoundContext  *context,
                                                    GHashTable     *attrs,
                                                    GCancellable   *cancellable,
                                                    GError        **error);

void              gsound_context_play_full         (GSoundContext       *context,
                                                    GCancellable        *cancellable,
                                                    GAsyncReadyCallback  callback,
                                                    gpointer             user_data,
                                                    ...) G_GNUC_NULL_TERMINATED;

void              gsound_context_play_fullv        (GSoundContext       *context,
                                                    GHashTable          *attrs,
                                                    GCancellable        *cancellable,
                                                    GAsyncReadyCallback  callback,
                                                    gpointer             user_data);

gboolean          gsound_context_play_full_finish  (GSoundContext  *context,
                                                    GAsyncResult   *result,
                                                    GError        **error);

gboolean          gsound_context_cache             (GSoundContext  *context,
                                                     GError        **error,
                                                     ...) G_GNUC_NULL_TERMINATED;

gboolean          gsound_context_cachev            (GSoundContext  *context,
                                                    GHashTable     *attrs,
                                                    GError        **error);

G_END_DECLS
#endif /* GSOUND_CONTEXT_H */

