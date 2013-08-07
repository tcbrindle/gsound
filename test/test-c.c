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
/*
 * A brief example of using GSound from C.
 * 
 * We also test that we can cancel two playing sounds with the same
 * cancellable, and do that from another thread.
 * 
 * Note that cancellation doesn't seem to work with the Alsa driver, but that
 * seems to be a bug in Canberra itself. Use Pulse.
 */


#include <gsound.h>

#define CANBERRA_DRIVER "pulse"

static void
check_error(GError *error)
{
    if (!error)
        return;
    
    g_error("Received error:"
            "\tdomain: %d\n"
            "\tcode: %d\n"
            "\tmessage: %s",
            error->domain, error->code, error->message);
}

static gpointer
thread_func(gpointer data)
{
    /* Wait a second, then cancel */
    g_usleep (1 * G_USEC_PER_SEC);
    g_cancellable_cancel(data);
    return NULL;
}

static void
on_play_finished(GObject *obj, GAsyncResult *result, gpointer user_data)
{
    GError *error = NULL;
    GMainLoop *main_loop = user_data;

    gsound_context_play_full_finish(GSOUND_CONTEXT(obj), result, &error);

    if (error)
    {
        if (error->domain != G_IO_ERROR ||
            error->code != G_IO_ERROR_CANCELLED)
            check_error(error);
        g_error_free(error);
    }
    else
    {
        g_assert_not_reached();
    }

    g_main_loop_quit (main_loop);
}

int
main(int argc, char *argv[])
{
    GSoundContext *ctx;
    GCancellable *cancellable;
    GMainLoop *main_loop;
    GThread *thread;
    GError *error = NULL;

    cancellable = g_cancellable_new();
    
    ctx = gsound_context_new(cancellable, &error);
    check_error(error);

    gsound_context_set_driver (ctx, CANBERRA_DRIVER, &error);
    check_error(error);
    
    gsound_context_open (ctx, &error);
    check_error(error);

    g_assert(!g_cancellable_is_cancelled (cancellable));

    main_loop = g_main_loop_new (NULL, TRUE);

    gsound_context_play_simple (ctx, cancellable, &error,
                                GSOUND_ATTR_MEDIA_FILENAME,
                                "/usr/share/sounds/alsa/Front_Right.wav",
                                NULL);
    check_error (error);

    gsound_context_play_full(ctx, cancellable,
                             on_play_finished, main_loop,
                             GSOUND_ATTR_MEDIA_FILENAME, "/usr/share/sounds/alsa/Front_Center.wav",
                             NULL);

    thread = g_thread_new("Thread2", thread_func, cancellable);

    g_main_loop_run(main_loop);

    /* FIXME: We seem to leak the cancellable, but I can't for the life of me
     * see why or how. Valgrind is no help. */
    g_object_unref(cancellable);
    g_object_unref(ctx);

    return 0;
}
