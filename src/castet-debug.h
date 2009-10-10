/*
 * Castet - Load and draw SVG
 *
 * Copyright (C) 2009 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License, version 2.1, as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * Originally taken from clutter-gst, relicensed to LGPL 2.1 as allowed by
 * the original material license(the original code is under LGPL 2+).
 *      Copyright (C) 2006-2008 OpenedHand
 *      Copyright (C) 2009 Intel Corporation
 */

#ifndef __CASTET_DEBUG_H__
#define __CASTET_DEBUG_H__

#include <glib.h>

G_BEGIN_DECLS

#ifdef CASTET_ENABLE_DEBUG

typedef enum
{
    CASTET_DEBUG_MISC            = 1 << 0,
    CASTET_DEBUG_PARSING         = 1 << 1,
    CASTET_DEBUG_TRAVERSER       = 1 << 2,
} CastetDebugFlag;

#ifdef __GNUC__

#define CASTET_NOTE(type,x,a...)                                \
    G_STMT_START {                                              \
        if (_castet_debug_flags & CASTET_DEBUG_##type)          \
        { g_message ("[" #type "] " G_STRLOC ": " x, ##a); }    \
    } G_STMT_END

#define CASTET_TIMESTAMP(type,x,a...)                           \
    G_STMT_START {                                              \
        if (_castet_debug_flags & CASTET_DEBUG_##type)          \
        { g_message ("[" #type "]" " %li:"  G_STRLOC ": "       \
                     x, _castet_get_timestamp(), ##a); }        \
    } G_STMT_END

#else /* !__GNUC__ */

/* Try the C99 version; unfortunately, this does not allow us to pass
 * empty arguments to the macro, which means we have to
 * do an intemediate printf.
 */
#define CASTET_NOTE(type,...)                                   \
    G_STMT_START {                                              \
        if (_castet_debug_flags & CASTET_DEBUG_##type)          \
        {                                                       \
            gchar * _fmt = g_strdup_printf (__VA_ARGS__);       \
            g_message ("[" #type "] " G_STRLOC ": %s",_fmt);    \
            g_free (_fmt);                                      \
        }                                                       \
    } G_STMT_END

#define CASTET_TIMESTAMP(type,...)                              \
    G_STMT_START {                                              \
        if (_castet_debug_flags & CASTET_DEBUG_##type)          \
        {                                                       \
            gchar * _fmt = g_strdup_printf (__VA_ARGS__);       \
            g_message ("[" #type "]" " %li:"  G_STRLOC ": %s",  \
                       _castet_get_timestamp(), _fmt);          \
            g_free (_fmt);                                      \
        }                                                       \
    } G_STMT_END

#endif /* __GNUC__ */

#define CASTET_MARK()      CASTET_NOTE(MISC, "== mark ==")

/* We do not even define those (private) symbols when debug is disabled.
 * This is to ensure the debug code is not shiped with the program when
 * disabled */

extern guint _castet_debug_flags;

gulong    _castet_get_timestamp    (void);
gboolean  _castet_debug_init       (void);

#else /* !CASTET_ENABLE_DEBUG */

#define CASTET_NOTE(type,...)         G_STMT_START { } G_STMT_END
#define CASTET_MARK()                 G_STMT_START { } G_STMT_END
#define CASTET_TIMESTAMP(type,...)    G_STMT_START { } G_STMT_END

#endif /* CASTET_ENABLE_DEBUG */

G_END_DECLS

#endif /* __CASTET_DEBUG_H__ */
