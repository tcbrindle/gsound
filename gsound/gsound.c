/* GSound -- GObject wrapper for libcanberra
 * 
 * Copyright 2013 Tristan Brindle
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the licence or (at
 * your option) any later version.
 *
 * See the included COPYING file for more information.
 * 
 * Author: Tristan Brindle <t.c.brindle at gmail dot com>
 */

#include "gsound.h"

#include <canberra.h>

#include <stdarg.h>

#define GSOUND_CONTEXT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GSOUND_TYPE_CONTEXT, GSoundContextPrivate))

static void gsound_context_initable_init(GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE(GSoundContext, gsound_context, G_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE,
                                              gsound_context_initable_init));

struct _GSoundContextPrivate
{
    ca_context *ca;
};

GQuark
gsound_error_quark (void)
{
    return g_quark_from_static_string ("gsound-error-quark");
}

static gboolean
test_return(int code, GError **error)
{
    if (code == CA_SUCCESS)
        return TRUE;

    g_set_error_literal (error, GSOUND_ERROR, code, ca_strerror(code));
    return FALSE;
}

static void
hash_table_to_prop_list(GHashTable *ht, ca_proplist *pl)
{
    gpointer key, value;
    GHashTableIter iter;

    g_hash_table_ref(ht);

    g_hash_table_iter_init (&iter, ht);
    while (g_hash_table_iter_next (&iter, &key, &value))
        ca_proplist_sets(pl, key, value);

    g_hash_table_unref(ht);
}

static int
var_args_to_prop_list(va_list args, ca_proplist *pl)
{
    while (TRUE)
    {
        const char *key = va_arg(args, const char*);
        if (!key)
            return CA_SUCCESS;

        const char *val = va_arg(args, const char*);
        if (!val)
            return CA_ERROR_INVALID;

        int res = ca_proplist_sets(pl, key, val);
        if (res != CA_SUCCESS)
            return res;
    }

    return CA_SUCCESS;
}

static void
on_ca_play_full_finished(ca_context *ca,
                         guint32 id,
                         int error_code,
                         gpointer user_data)
{
    GSimpleAsyncResult *result = user_data;

    if (error_code != CA_SUCCESS)
    {
        g_simple_async_result_set_error(result,
                                        GSOUND_ERROR,
                                        error_code,
                                        ca_strerror(error_code));
    }

    g_simple_async_result_complete_in_idle (result);
    g_object_unref(result);
}

static void
on_cancellable_cancelled(GCancellable *cancellable,
                         GSoundContext *self)
{
    ca_context_cancel(self->priv->ca, GPOINTER_TO_INT(cancellable));
}

/**
 * gsound_context_new:
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @error: Return location for error
 * 
 * Returns: (transfer full): A new #GSoundContext
 */
GSoundContext *
gsound_context_new(GCancellable *cancellable, GError **error)
{
    return GSOUND_CONTEXT(g_initable_new(GSOUND_TYPE_CONTEXT,
                                         cancellable,
                                         error,
                                         NULL));
}

/**
 * gsound_context_open:
 * @context: A #GSoundContext
 * @error: Return location for error, or %NULL
 * 
 * Returns: %TRUE if the output device was opened successfully, or %FALSE
 *          (populating @error)
 */
gboolean
gsound_context_open(GSoundContext *self, GError **error)
{
    g_return_val_if_fail(GSOUND_IS_CONTEXT(self), FALSE);

    return test_return (ca_context_open(self->priv->ca), error);
}

/**
 * gsound_context_set_driver:
 * @context: A #GSoundContext
 * @driver: libcanberra driver to use
 * @error: Return location for error, or %NULL
 * 
 * Returns: %TRUE if the libcanberra driver was set successfully
 */
gboolean
gsound_context_set_driver(GSoundContext *self,
                          const char *driver,
                          GError **error)
{
    g_return_val_if_fail (GSOUND_IS_CONTEXT(self), FALSE);

    return test_return (ca_context_set_driver(self->priv->ca, driver), error);
}

/**
 * gsound_context_change_attrs: (skip)
 * @context: A #GSoundContext
 * @error: Return location for error
 * @...: %NULL terminated list of attribute name-value pairs
 * 
 * Returns: %TRUE if attributes were updated successfully
 */
gboolean
gsound_context_change_attrs(GSoundContext *self,
                            GError **error,
                            ...)
{
    ca_proplist *pl;
    va_list args;
    int res;
    
    g_return_val_if_fail(GSOUND_IS_CONTEXT(self), FALSE);

    if ((res = ca_proplist_create(&pl)) != CA_SUCCESS)
        return test_return(res, error);

    va_start(args, error);
    var_args_to_prop_list (args, pl);
    va_end(args);

    res = ca_context_change_props_full(self->priv->ca, pl);

    ca_proplist_destroy(pl);

    return test_return(res, error);
}

/**
 * gsound_context_change_attrsv:
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Hash table of attributes to set
 * @error: Return location for error, or %NULL
 * 
 * Returns: %TRUE if attributes were updated successfully
 */
gboolean
gsound_context_change_attrsv (GSoundContext *self,
                              GHashTable *attrs,
                              GError **error)
{
    ca_proplist *pl;
    
    g_return_val_if_fail(GSOUND_IS_CONTEXT(self), FALSE);

    int res = ca_proplist_create(&pl);
    if (!test_return (res, error))
        return FALSE;
    
    hash_table_to_prop_list (attrs, pl);

    res = ca_context_change_props_full(self->priv->ca, pl);

    ca_proplist_destroy(pl);

    return test_return(res, error);
}

/**
 * gsound_context_play_simple: (skip)
 * @context: A #GSoundContext
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @error: Return location for error, or %NULL
 * @...: Arguments
 * 
 * Returns: %TRUE on success, or %FALSE, populating @error
 */
gboolean
gsound_context_play_simple (GSoundContext *self,
                            GCancellable *cancellable,
                            GError **error,
                            ...)
{
    ca_proplist *pl;
    va_list args;
    int res;
    
    g_return_val_if_fail(GSOUND_IS_CONTEXT(self), FALSE);

    if ((res = ca_proplist_create(&pl)) != CA_SUCCESS)
        return test_return(res, error);

    va_start(args, error);
    var_args_to_prop_list (args, pl);
    va_end(args);

    res = ca_context_play_full(self->priv->ca,
                               GPOINTER_TO_INT(cancellable),
                               pl, NULL, NULL);

    if (cancellable)
        g_cancellable_connect(cancellable,
                              G_CALLBACK(on_cancellable_cancelled),
                              g_object_ref(self),
                              g_object_unref);

    ca_proplist_destroy(pl);

    return test_return(res, error);
}

/**
 * gsound_context_play_simplev:
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Attributes
 * @cancellable: (allow-none): A #GCancellable
 * @error: Return location for error
 * 
 * Returns: %TRUE on success, %FALSE on error
 * 
 * Rename to: gsound_context_play_simple
 */
gboolean
gsound_context_play_simplev(GSoundContext *self,
                            GHashTable *attrs,
                            GCancellable *cancellable,
                            GError **error)
{
    ca_proplist *proplist;
    int res = ca_proplist_create(&proplist);
    if (!test_return (res, error))
        return FALSE;
    
    hash_table_to_prop_list (attrs, proplist);

    res = ca_context_play_full(self->priv->ca,
                               GPOINTER_TO_INT(cancellable),
                               proplist, NULL, NULL);

    if (cancellable)
        g_cancellable_connect(cancellable,
                              G_CALLBACK(on_cancellable_cancelled),
                              g_object_ref(self),
                              g_object_unref);

    ca_proplist_destroy(proplist);
    
    return test_return (res, error);
}

/**
 * gsound_context_play_full: (skip)
 * @context: A #GSoundContext
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @callback: (scope async): callback
 * @user_data: User data passed to @callback
 * @...: Attributes
 * 
 */
void
gsound_context_play_full(GSoundContext *self,
                         GCancellable *cancellable,
                         GAsyncReadyCallback callback,
                         gpointer user_data,
                         ...)
{
    GError *inner_error = NULL;
    ca_proplist *proplist;
    va_list args;
    int res = ca_proplist_create(&proplist);
    if (!test_return (res, &inner_error))
    {
        g_simple_async_report_take_gerror_in_idle (G_OBJECT(self),
                                                   callback,
                                                   user_data,
                                                   inner_error);
    }

    va_start(args, user_data);
    var_args_to_prop_list (args, proplist);
    va_end(args);

    GSimpleAsyncResult *result = g_simple_async_result_new(G_OBJECT(self),
                                                           callback,
                                                           user_data,
                                                           NULL /*FIXME*/);

    res = ca_context_play_full(self->priv->ca,
                               GPOINTER_TO_INT(cancellable),
                               proplist,
                               on_ca_play_full_finished,
                               result);

    if (cancellable)
        g_cancellable_connect(cancellable,
                              G_CALLBACK(on_cancellable_cancelled),
                              g_object_ref(self),
                              g_object_unref);

    ca_proplist_destroy(proplist);

    if (!test_return (res, &inner_error))
    {
        g_simple_async_report_take_gerror_in_idle (G_OBJECT(self),
                                                   callback,
                                                   user_data,
                                                   inner_error);
        g_object_unref(result);
    }
}

/**
 * gsound_context_play_fullv:
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Attributes
 * @cancellable: (allow-none): A #GCancellable, or %NULL
 * @callback: (scope async): callback
 * @user_data: user_data
 * 
 * Rename to: gsound_context_play_full
 */
void
gsound_context_play_fullv(GSoundContext *self,
                          GHashTable *attrs,
                          GCancellable *cancellable,
                          GAsyncReadyCallback callback,
                          gpointer user_data)
{
    GError *inner_error = NULL;
    ca_proplist *proplist;
    int res = ca_proplist_create(&proplist);
    if (!test_return (res, &inner_error))
    {
        g_simple_async_report_take_gerror_in_idle (G_OBJECT(self),
                                                   callback,
                                                   user_data,
                                                   inner_error);
    }
    
    hash_table_to_prop_list (attrs, proplist);

    GSimpleAsyncResult *result = g_simple_async_result_new(G_OBJECT(self),
                                                           callback,
                                                           user_data,
                                                           NULL /*FIXME*/);

    res = ca_context_play_full(self->priv->ca,
                               GPOINTER_TO_INT(cancellable),
                               proplist,
                               on_ca_play_full_finished,
                               result);

    if (cancellable)
        g_cancellable_connect(cancellable,
                              G_CALLBACK(on_cancellable_cancelled),
                              g_object_ref(self),
                              g_object_unref);

    ca_proplist_destroy(proplist);

    if (!test_return (res, &inner_error))
    {
        g_simple_async_report_take_gerror_in_idle (G_OBJECT(self),
                                                   callback,
                                                   user_data,
                                                   inner_error);
        g_object_unref(result);
    }
}

/**
 * gsound_context_play_full_finish:
 * @context: A #GSoundContext
 * @result: Result object returned to the callback of 
 *   gsound_context_play_full()
 * @error: Return location for error
 * 
 * Returns: %TRUE if playing finished successfully
 */
gboolean
gsound_context_play_full_finish (GSoundContext *self,
                                 GAsyncResult *result,
                                 GError **error)
{
    g_return_val_if_fail(GSOUND_IS_CONTEXT(self), FALSE);
    GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(result);

    return !g_simple_async_result_propagate_error (simple, error);
}

/**
 * gsound_context_cache: (skip)
 * @context: A #GSoundContext
 * @error: Return location for error, or %NULL
 * @...: attributes
 * 
 * Returns: %TRUE on success, %FALSE otherwise
 */
gboolean
gsound_context_cache(GSoundContext *self,
                     GError **error,
                     ...)
{
    ca_proplist *pl;
    va_list args;
    int res;
    
    g_return_val_if_fail(GSOUND_IS_CONTEXT(self), FALSE);

    if ((res = ca_proplist_create(&pl)) != CA_SUCCESS)
        return test_return(res, error);

    va_start(args, error);
    var_args_to_prop_list (args, pl);
    va_end(args);

    res = ca_context_cache_full(self->priv->ca, pl);

    ca_proplist_destroy(pl);

    return test_return(res, error);
}

/**
 * gsound_context_cachev:
 * @context: A #GSoundContext
 * @attrs: (element-type utf8 utf8): Hash table of attrerties
 * @error: Return location for error, or %NULL
 * 
 * Returns: %TRUE on success
 * 
 * Rename to: gsound_context_cache
 */
gboolean
gsound_context_cachev(GSoundContext *self,
                      GHashTable *attrs,
                      GError **error)
{
    ca_proplist *proplist;
    int res = ca_proplist_create(&proplist);
    if (!test_return (res, error))
        return FALSE;
    
    hash_table_to_prop_list (attrs, proplist);

    res = ca_context_cache_full(self->priv->ca, proplist);

    ca_proplist_destroy(proplist);

    return test_return(res, error);
}

static gboolean
gsound_context_real_init(GInitable *initable, GCancellable *cancellable, GError **error)
{
    GSoundContext *self = GSOUND_CONTEXT(initable);

    if (self->priv->ca)
        return TRUE;

    int success = ca_context_create(&self->priv->ca);
    
    if (!test_return(success, error))
        return FALSE;

    /* Set a couple of attributes here if we can */
    ca_proplist *pl;
    ca_proplist_create(&pl);

    ca_proplist_sets(pl, CA_PROP_APPLICATION_NAME, g_get_application_name());
    if (g_application_get_default())
    {
        GApplication *app = g_application_get_default ();
        ca_proplist_sets(pl, CA_PROP_APPLICATION_ID,
                         g_application_get_application_id(app));
    }

    success = ca_context_change_props_full(self->priv->ca, pl);

    ca_proplist_destroy(pl);

    if (!test_return(success, error))
    {
        ca_context_destroy(self->priv->ca);
        self->priv->ca = NULL;
    }

    return TRUE;
}

static void
gsound_context_finalize(GObject *obj)
{
    GSoundContext *self = GSOUND_CONTEXT(obj);

    ca_context_destroy(self->priv->ca);

    G_OBJECT_CLASS(gsound_context_parent_class)->finalize(obj);
}

static void
gsound_context_class_init(GSoundContextClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->finalize = gsound_context_finalize;

    g_type_class_add_private(klass, sizeof(GSoundContextPrivate));
}

static void
gsound_context_init(GSoundContext *self)
{
    self->priv = GSOUND_CONTEXT_GET_PRIVATE(self);
}

static void
gsound_context_initable_init(GInitableIface *iface)
{
    iface->init = gsound_context_real_init;
}
