/* gsound-context.c
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
/**
 * SECTION: gsound-context
 * @title: GSoundContext
 * @short_description: GSound context object
 * @see_also: #ca_context
 * 
 * A #GSoundContext is used for playing system sounds. The typical use pattern
 * is:
 * 
 * * Initialize the #GSoundContext
 * * [Optional] Set any global attributes using gsound_context_set_attributes()
 * * [Optional] Cache any frequently-used sounds (for example, sound
 *   effects for a game) using gsound_context_cache()
 * * Play sounds using gsound_context_play_simple() or gsound_context_play_full()
 * * Close the connection to the sound server and clean up the context using
 *   g_object_unref()
 *
 * #GSoundContext implements the #GInitable interface, so if created with
 * g_object_new() (as typically happens with language bindings) then you must
 * call the g_initable_init() method before attempting to use it.
 *
 * # Simple Examples
 *
 * In C:
 * 
 * |[<!-- language="C" -->
 * GSoundContext *ctx = NULL;
 * GCancellable *cancellable = g_cancellable_new();
 * GError *error = NULL;
 * 
 * ctx = gsound_context_new(cancellable, &error);
 * if (error) {
 *     // handle error
 * }
 * 
 * gsound_context_play_simple(ctx, cancellable, &error,
 *                            GSOUND_ATTR_EVENT_ID, "phone-incoming-call",
 *                            // other attributes...
 *                            NULL);
 * ]| 
 * 
 * or, using Python via GObject Introspection:
 * 
 * |[<!-- language="Python" -->
 * from gi.repository import GSound
 * 
 * ctx = GSound.Context()
 * 
 * try:
 *     ctx.init();
 *     ctx.play_simple({ GSound.ATTR_EVENT_ID : "phone-incoming-call" })
 * except:
 *     # Handle error
 *     pass
 * ]|
 *
 * or using Vala:
 * 
 * |[<!-- language="Vala" -->
 * try {
 *     var ctx = new GSound.Context();
 *     ctx.play_simple(null, GSound.Attribute.EVENT_ID, "phone-incoming-call");
 * } catch (Error e) {
 *     // handle error
 * }
 * ]|
 *
 * # `play_simple()` versus `play_full()`
 * 
 * The above examples use the gsound_context_play_simple() method for
 * playing sounds. This is a "fire and forget" method which returns
 * immediately and does not block your program, and is suitable for most use
 * cases.
 * 
 * If you need to find out when the sound finished (for example to repeat the
 * sound) then you can use the gsound_context_play_full() version. This
 * is an asynchronous method using the standard GIO async pattern, which will
 * run the supplied #GAsyncReadyCallback when the sound server has finished.
 * It is guaranteed that the callback will be run exactly once.
 * 
 * Note that calling gsound_context_play_full() with a %NULL callback is not
 * equivalent to calling gsound_context_play_simple(). When calling 
 * play_simple(), errors which occur before the sound is passed to the sound
 * server are reported immediately, whereas with `play_full()` these are reported
 * in the callback. If you pass a %NULL callback to gsound_context_play_full()
 * you will not be able to receive these errors, so it is strongly recommended
 * to avoid doing this and use gsound_context_play_simple() in the case when
 * you don't need to be notified when the sound has finished.
 * 
 * # Passing Attributes
 * 
 * GSound supplies information to the sound server by means of attributes.
 * Attributes can be set on the #GSoundContext itself using
 * gsound_context_set_attributes(), or supplied in a `play()` call. Attributes
 * set on the context will automatically applied to any subsequent `play()`
 * calls, unless overridden by that call.
 * 
 * In C and Vala, attributes are passed as %NULL-terminated list of
 * (attribute, value) pairs. When using GObject introspection, attributes are
 * typically passed using a language-specific associated array, for example
 * a dict in Python or an object in JavaScript.
 * 
 * For the list of attributes supported by GSound, see
 * [GSound Attributes][gsound-GSound-Attributes].
 *
 * # Caching # {#caching}
 * 
 * If supported by the sound server, frequently-used sounds may be cached. This
 * may be useful, for example, for sound effects in a game. To cache a sound,
 * either call gsound_context_cache(), or pass the special
 * #GSOUND_ATTR_CANBERRA_CACHE_CONTROL attribute to one of the `play()`
 * functions.
 * 
 * For example, in the startup code for a game you might include something
 * like the following (error checking omitted):
 * 
 * |[ <-- language="C" -->
 * GSoundContext *ctx = gsound_context_new (NULL, NULL);
 * gsound_context_cache(ctx, NULL,
 *                      GSOUND_ATTR_MEDIA_FILENAME,
 *                      "/path/to/player-spaceship-fire-laser.ogg",
 *                      NULL);
 * ]|
 * 
 * There are three caching modes available, "permanent", "volatile" and "never".
 * The default mode when calling gsound_context_cache() is "permanent", and
 * the default mode for gsound_context_play_simple() and `play_full()` is
 * "never".
 * 
 * See the documentation for #GSOUND_ATTR_CANBERRA_CACHE_CONTROL for more
 * details.
 * 
 */

#include "gsound-context.h"

#include <canberra.h>

#include <stdarg.h>

static void gsound_context_initable_init (GInitableIface *iface);

struct _GSoundContext
{
  GObject     parent;

  ca_context *ca;
};

struct _GSoundContextClass
{
  GObjectClass parent_class;
};

G_DEFINE_TYPE_WITH_CODE (GSoundContext, gsound_context, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                                gsound_context_initable_init))

G_DEFINE_QUARK (gsound - error - quark, gsound_error);

static gboolean
test_return (int code, GError **error)
{
  if (code == CA_SUCCESS)
    return TRUE;

  g_set_error_literal (error, GSOUND_ERROR, code, ca_strerror (code));
  return FALSE;
}

static void
hash_table_to_prop_list (GHashTable *ht, ca_proplist *pl)
{
  gpointer key, value;
  GHashTableIter iter;

  g_hash_table_ref (ht);

  g_hash_table_iter_init (&iter, ht);
  while (g_hash_table_iter_next (&iter, &key, &value))
    ca_proplist_sets (pl, key, value);

  g_hash_table_unref (ht);
}

static int
var_args_to_prop_list (va_list args, ca_proplist *pl)
{
  while (TRUE)
    {
      const char *key;
      const char *val;
      int res;

      key = va_arg (args, const char*);
      if (!key)
        return CA_SUCCESS;

      val = va_arg (args, const char*);
      if (!val)
        return CA_ERROR_INVALID;

      res = ca_proplist_sets (pl, key, val);
      if (res != CA_SUCCESS)
        return res;
    }

  return CA_SUCCESS;
}

static void
on_ca_play_full_finished (ca_context *ca,
                          guint32     id,
                          int         error_code,
                          gpointer    user_data)
{
  GTask *task = user_data;

  if (error_code != CA_SUCCESS)
    {
      g_task_return_new_error (task,
                               GSOUND_ERROR,
                               error_code,
                               "%s",
                               ca_strerror (error_code));
    }
  else
    g_task_return_boolean (task, TRUE);

  g_object_unref (task);
}

static void
on_cancellable_cancelled (GCancellable  *cancellable,
                          GSoundContext *self)
{
  ca_context_cancel (self->ca, g_direct_hash (cancellable));
}

/**
 * gsound_context_new:
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @error: Return location for error
 *
 * Creates and initializes a new #GSoundContext. If the an error occured
 * during initialization, #NULL is returned and @error will be set
 * appropriately.
 *
 * Returns: (transfer full): A new #GSoundContext
 */
GSoundContext *
gsound_context_new (GCancellable *cancellable, GError **error)
{
  return GSOUND_CONTEXT (g_initable_new (GSOUND_TYPE_CONTEXT,
                                         cancellable,
                                         error,
                                         NULL));
}

/**
 * gsound_context_open:
 * @context: A #GSoundContext
 * @error: Return location for error
 *
 * Attempts to open a connection to the backend sound driver. It is recommended
 * that you set context attributes with gsound_context_set_attributes() before
 * calling this function.
 *
 * > A connection is automatically opened before playing or caching sounds,
 * > so you rarely need to call this yourself.
 *
 * Returns: %TRUE if the output device was opened successfully, or %FALSE
 *          (populating @error)
 */
gboolean
gsound_context_open (GSoundContext *self, GError **error)
{
  g_return_val_if_fail (GSOUND_IS_CONTEXT (self), FALSE);

  return test_return (ca_context_open (self->ca), error);
}

/**
 * gsound_context_set_driver:
 * @context: A #GSoundContext
 * @driver: libcanberra driver to use
 * @error: Return location for error, or %NULL
 *
 * Sets the libcanberra driver to @driver, for example "pulse", "alsa" or "null".
 * You normally do not need to set this yourself.
 *
 * Note that this function may return %TRUE even if the specified driver is
 * not available: see the libcanberra documentation for details.
 *
 * Returns: %TRUE if the libcanberra driver was set successfully
 */
gboolean
gsound_context_set_driver (GSoundContext *self,
                           const char    *driver,
                           GError       **error)
{
  g_return_val_if_fail (GSOUND_IS_CONTEXT (self), FALSE);

  return test_return (ca_context_set_driver (self->ca, driver), error);
}

/**
 * gsound_context_set_attributes: (skip)
 * @context: A #GSoundContext
 * @error: Return location for error
 * @...: %NULL terminated list of attribute name-value pairs
 *
 * Set attributes or change attributes on @context. Subsequent calls to this
 * function calling the same attributes will override the earlier values.
 *
 * Note that GSound will set the #GSOUND_ATTR_APPLICATION_NAME and
 * #GSOUND_ATTR_APPLICATION_ID for you if using #GApplication, so you do
 * not normally need to set these yourself.
 *
 * Returns: %TRUE if attributes were updated successfully
 */
gboolean
gsound_context_set_attributes (GSoundContext *self,
                               GError       **error,
                               ...)
{
  ca_proplist *pl;
  va_list args;
  int res;

  g_return_val_if_fail (GSOUND_IS_CONTEXT (self), FALSE);

  if ((res = ca_proplist_create (&pl)) != CA_SUCCESS)
    return test_return (res, error);

  va_start (args, error);
  var_args_to_prop_list (args, pl);
  va_end (args);

  res = ca_context_change_props_full (self->ca, pl);

  g_clear_pointer (&pl, ca_proplist_destroy);

  return test_return (res, error);
}

/**
 * gsound_context_set_attributesv: (rename-to gsound_context_set_attributes)
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Hash table of attributes to set
 * @error: Return location for error, or %NULL
 *
 * Set attributes or change attributes on @context. Subsequent calls to this
 * function calling the same attributes will override the earlier values.
 *
 * Note that GSound will set the #GSOUND_ATTR_APPLICATION_NAME and
 * #GSOUND_ATTR_APPLICATION_ID for you if using #GApplication, so you do
 * not normally need to set these yourself.
 * 
 * This function is intented to be used by language bindings.
 *
 * Returns: %TRUE if attributes were updated successfully
 */
gboolean
gsound_context_set_attributesv (GSoundContext *self,
                                GHashTable    *attrs,
                                GError       **error)
{
  ca_proplist *pl;
  int res;

  g_return_val_if_fail (GSOUND_IS_CONTEXT (self), FALSE);

  res = ca_proplist_create (&pl);
  if (!test_return (res, error))
    return FALSE;

  hash_table_to_prop_list (attrs, pl);

  res = ca_context_change_props_full (self->ca, pl);

  g_clear_pointer (&pl, ca_proplist_destroy);

  return test_return (res, error);
}

/**
 * gsound_context_play_simple: (skip)
 * @context: A #GSoundContext
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @error: Return location for error, or %NULL
 * @...: A %NULL-terminated list of attribute-value pairs
 *
 * The basic "fire-and-forget" play command. This function will not block, and
 * just sends a request to the sound server before immediately returning.
 *
 * If you need to know when a sound finishes playing then you should call
 * gsound_context_play_full() instead.
 *
 * You can cancel playback at any time by calling g_cancellable_cancel() on
 * @cancellable, if supplied. 
 *
 * Returns: %TRUE on success, or %FALSE, populating @error
 */
gboolean
gsound_context_play_simple (GSoundContext *self,
                            GCancellable  *cancellable,
                            GError       **error,
                            ...)
{
  ca_proplist *pl;
  va_list args;
  int res;

  g_return_val_if_fail (GSOUND_IS_CONTEXT (self), FALSE);

  if ((res = ca_proplist_create (&pl)) != CA_SUCCESS)
    return test_return (res, error);

  va_start (args, error);
  var_args_to_prop_list (args, pl);
  va_end (args);

  res = ca_context_play_full (self->ca,
                              g_direct_hash (cancellable),
                              pl, NULL, NULL);

  if (cancellable)
    g_cancellable_connect (cancellable,
                           G_CALLBACK (on_cancellable_cancelled),
                           g_object_ref (self),
                           g_object_unref);

  g_clear_pointer (&pl, ca_proplist_destroy);

  return test_return (res, error);
}

/**
 * gsound_context_play_simplev: (rename-to gsound_context_play_simple)
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Attributes
 * @cancellable: (allow-none): A #GCancellable
 * @error: Return location for error
 *
 * The basic "fire-and-forget" play command. This function will not block, and
 * just sends a request to the sound server before immediately returning.
 *
 * If you need to know when a sound finishes playing then you should call
 * gsound_context_play_full() instead.
 *
 * You can cancel playback at any time by calling g_cancellable_cancel() on
 * @cancellable, if supplied.
 * 
 * This function is intented to be used by language bindings.
 *
 * Returns: %TRUE on success, %FALSE on error
 *
 */
gboolean
gsound_context_play_simplev (GSoundContext *self,
                             GHashTable    *attrs,
                             GCancellable  *cancellable,
                             GError       **error)
{
  ca_proplist *pl;
  int res = ca_proplist_create (&pl);

  if (!test_return (res, error))
    return FALSE;

  hash_table_to_prop_list (attrs, pl);

  res = ca_context_play_full (self->ca,
                              g_direct_hash (cancellable),
                              pl, NULL, NULL);

  if (cancellable)
    g_cancellable_connect (cancellable,
                           G_CALLBACK (on_cancellable_cancelled),
                           g_object_ref (self),
                           g_object_unref);

  g_clear_pointer (&pl, ca_proplist_destroy);

  return test_return (res, error);
}

/**
 * gsound_context_play_full: (skip)
 * @context: A #GSoundContext
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @callback: (scope async): callback
 * @user_data: User data passed to @callback
 * @...: A %NULL-terminated list of attribute-value pairs
 *
 * Asynchronously request a sound to be played. When playback is finished
 * (or if an error occurs) then @callback will be called, following the
 * normal GIO async pattern.
 *
 * If playback is cancelled via @cancellable, then @callback will be called
 * with #G_IO_ERROR_CANCELLED.
 *
 * If you do not need notification of when playback is complete, you should
 * use gsound_context_play_simple().
 */
void
gsound_context_play_full (GSoundContext      *self,
                          GCancellable       *cancellable,
                          GAsyncReadyCallback callback,
                          gpointer            user_data,
                          ...)
{
  GError *inner_error = NULL;
  ca_proplist *proplist;
  va_list args;
  GTask *task;
  int res;

  task = g_task_new (self, cancellable, callback, user_data);

  res = ca_proplist_create (&proplist);
  if (!test_return (res, &inner_error))
    {
      g_task_return_error (task, inner_error);
      g_object_unref (task);
      return;
    }

  va_start (args, user_data);
  var_args_to_prop_list (args, proplist);
  va_end (args);

  res = ca_context_play_full (self->ca,
                              g_direct_hash (cancellable),
                              proplist,
                              on_ca_play_full_finished,
                              task);

  if (cancellable)
    g_cancellable_connect (cancellable,
                           G_CALLBACK (on_cancellable_cancelled),
                           g_object_ref (self),
                           g_object_unref);

  g_clear_pointer (&proplist, ca_proplist_destroy);

  if (!test_return (res, &inner_error))
    {
      g_task_return_error (task, inner_error);
      g_object_unref (task);
    }
}

/**
 * gsound_context_play_fullv: (rename-to gsound_context_play_full)
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Attributes
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @callback: (scope async): callback
 * @user_data: user_data
 *
 * Asynchronously request a sound to be played. When playback is finished
 * (or if an error occurs) then @callback will be called, following the
 * normal GIO async pattern.
 *
 * If playback is cancelled via @cancellable, then @callback will be called
 * with #G_IO_ERROR_CANCELLED.
 *
 * If you do not need notification of when playback is complete, you should
 * use gsound_context_play_simple().
 * 
 * This function is intented to be used by language bindings.
 */
void
gsound_context_play_fullv (GSoundContext      *self,
                           GHashTable         *attrs,
                           GCancellable       *cancellable,
                           GAsyncReadyCallback callback,
                           gpointer            user_data)
{
  GError *inner_error = NULL;
  ca_proplist *proplist;
  GTask *task;
  int res;

  task = g_task_new (self, cancellable, callback, user_data);

  res = ca_proplist_create (&proplist);
  if (!test_return (res, &inner_error))
    {
      g_task_return_error (task, inner_error);
      g_object_unref (task);
      return;
    }

  hash_table_to_prop_list (attrs, proplist);

  res = ca_context_play_full (self->ca,
                              g_direct_hash (cancellable),
                              proplist,
                              on_ca_play_full_finished,
                              task);

  if (cancellable)
    g_cancellable_connect (cancellable,
                           G_CALLBACK (on_cancellable_cancelled),
                           g_object_ref (self),
                           g_object_unref);

  g_clear_pointer (&proplist, ca_proplist_destroy);

  if (!test_return (res, &inner_error))
    {
      g_task_return_error (task, inner_error);
      g_object_unref (task);
    }
}

/**
 * gsound_context_play_full_finish:
 * @context: A #GSoundContext
 * @result: Result object passed to the callback of
 *   gsound_context_play_full()
 * @error: Return location for error
 *
 * Finish an async operation started by gsound_context_play_full(). You
 * must call this function in the callback to free memory and receive any
 * errors which occurred.
 *
 * Returns: %TRUE if playing finished successfully
 */
gboolean
gsound_context_play_full_finish (GSoundContext *self,
                                 GAsyncResult  *result,
                                 GError       **error)
{
  g_return_val_if_fail (g_task_is_valid (result, self), FALSE);

  return g_task_propagate_boolean (G_TASK (result), error);
}

/**
 * gsound_context_cache: (skip)
 * @context: A #GSoundContext
 * @error: Return location for error
 * @...: A %NULL-terminated list of attribute-value pairs
 *
 * Requests that a sound be cached on the server. See [#caching][gsound-GSound-Context#caching].
 *
 * Returns: %TRUE on success
 */
gboolean
gsound_context_cache (GSoundContext *self,
                      GError       **error,
                      ...)
{
  ca_proplist *pl;
  va_list args;
  int res;

  g_return_val_if_fail (GSOUND_IS_CONTEXT (self), FALSE);

  if ((res = ca_proplist_create (&pl)) != CA_SUCCESS)
    return test_return (res, error);

  va_start (args, error);
  var_args_to_prop_list (args, pl);
  va_end (args);

  res = ca_context_cache_full (self->ca, pl);

  g_clear_pointer (&pl, ca_proplist_destroy);

  return test_return (res, error);
}

/**
 * gsound_context_cachev: (rename-to gsound_context_cache)
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Hash table of attrerties
 * @error: Return location for error, or %NULL
 * 
 * Requests that a sound be cached on the server. See [#caching][gsound-GSound-Context#caching].
 *
 * This function is intented to be used by language bindings.
 */
gboolean
gsound_context_cachev (GSoundContext *self,
                       GHashTable    *attrs,
                       GError       **error)
{
  ca_proplist *proplist;
  int res = ca_proplist_create (&proplist);

  if (!test_return (res, error))
    return FALSE;

  hash_table_to_prop_list (attrs, proplist);

  res = ca_context_cache_full (self->ca, proplist);

  g_clear_pointer (&proplist, ca_proplist_destroy);

  return test_return (res, error);
}

static gboolean
gsound_context_real_init (GInitable    *initable,
                          GCancellable *cancellable,
                          GError      **error)
{
  GSoundContext *self = GSOUND_CONTEXT (initable);
  int success;
  ca_proplist *pl;

  if (self->ca)
    return TRUE;

  success = ca_context_create (&self->ca);

  if (!test_return (success, error))
    return FALSE;

  /* Set a couple of attributes here if we can */
  ca_proplist_create (&pl);

  ca_proplist_sets (pl, CA_PROP_APPLICATION_NAME, g_get_application_name ());
  if (g_application_get_default ())
    {
      GApplication *app = g_application_get_default ();
      ca_proplist_sets (pl, CA_PROP_APPLICATION_ID,
                        g_application_get_application_id (app));
    }

  success = ca_context_change_props_full (self->ca, pl);

  g_clear_pointer (&pl, ca_proplist_destroy);

  if (!test_return (success, error))
      g_clear_pointer (&self->ca, ca_context_destroy);

  return TRUE;
}

static void
gsound_context_finalize (GObject *obj)
{
  GSoundContext *self = GSOUND_CONTEXT (obj);

  g_clear_pointer (&self->ca, ca_context_destroy);

  G_OBJECT_CLASS (gsound_context_parent_class)->finalize (obj);
}

static void
gsound_context_class_init (GSoundContextClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->finalize = gsound_context_finalize;
}

static void
gsound_context_init (GSoundContext *self)
{
}

static void
gsound_context_initable_init (GInitableIface *iface)
{
  iface->init = gsound_context_real_init;
}

