/*
 * Dax - Load and draw SVG
 *
 * Copyright (C) 2009, 2010 Intel Corporation
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>..
 */

/*
 * Originally taken from clutter-gst, relicensed to LGPL 2.1 as allowed by
 * the original material license(the original code is under LGPL 2+).
 *      Copyright (C) 2006-2008 OpenedHand
 *      Copyright (C) 2009 Intel Corporation
 */

#ifndef __DAX_DEBUG_H__
#define __DAX_DEBUG_H__

#include <glib.h>

G_BEGIN_DECLS

#ifdef DAX_ENABLE_DEBUG

typedef enum
{
    DAX_DEBUG_MISC            = 1 << 0,
    DAX_DEBUG_PARSING         = 1 << 1,
    DAX_DEBUG_NAMESPACE       = 1 << 2,
    DAX_DEBUG_TRAVERSER       = 1 << 3,
    DAX_DEBUG_EVENT           = 1 << 4,
    DAX_DEBUG_LOADING         = 1 << 5,
    DAX_DEBUG_SCRIPT          = 1 << 6
} DaxDebugFlag;

#ifdef __GNUC__

#define DAX_NOTE(type,x,a...)                                   \
    G_STMT_START {                                              \
        if (G_UNLIKELY (_dax_debug_flags & DAX_DEBUG_##type))   \
          g_message ("[" #type "] " G_STRLOC ": " x, ##a);      \
    } G_STMT_END

#define DAX_TIMESTAMP(type,x,a...)                              \
    G_STMT_START {                                              \
        if (G_UNLIKELY (_dax_debug_flags & DAX_DEBUG_##type))   \
          g_message ("[" #type "]" " %li:"  G_STRLOC ": "       \
                     x, _dax_get_timestamp(), ##a);             \
    } G_STMT_END

#else /* !__GNUC__ */

/* Try the C99 version; unfortunately, this does not allow us to pass
 * empty arguments to the macro, which means we have to
 * do an intemediate printf.
 */
#define DAX_NOTE(type,...)                                      \
    G_STMT_START {                                              \
        if (G_UNLIKELY (_dax_debug_flags & DAX_DEBUG_##type))   \
        {                                                       \
            gchar * _fmt = g_strdup_printf (__VA_ARGS__);       \
            g_message ("[" #type "] " G_STRLOC ": %s",_fmt);    \
            g_free (_fmt);                                      \
        }                                                       \
    } G_STMT_END

#define DAX_TIMESTAMP(type,...)                                 \
    G_STMT_START {                                              \
        if (G_UNLIKELY (_dax_debug_flags & DAX_DEBUG_##type))   \
        {                                                       \
            gchar * _fmt = g_strdup_printf (__VA_ARGS__);       \
            g_message ("[" #type "]" " %li:"  G_STRLOC ": %s",  \
                       _dax_get_timestamp(), _fmt);             \
            g_free (_fmt);                                      \
        }                                                       \
    } G_STMT_END

#endif /* __GNUC__ */

#define DAX_MARK()      DAX_NOTE(MISC, "== mark ==")

/* We do not even define those (private) symbols when debug is disabled.
 * This is to ensure the debug code is not shiped with the program when
 * disabled */

extern guint _dax_debug_flags;

gulong    _dax_get_timestamp    (void);
gboolean  _dax_debug_init       (void);

#else /* !DAX_ENABLE_DEBUG */

#define DAX_NOTE(type,...)         G_STMT_START { } G_STMT_END
#define DAX_MARK()                 G_STMT_START { } G_STMT_END
#define DAX_TIMESTAMP(type,...)    G_STMT_START { } G_STMT_END

#endif /* DAX_ENABLE_DEBUG */

G_END_DECLS

#endif /* __DAX_DEBUG_H__ */
