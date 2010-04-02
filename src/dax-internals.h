/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __DAX_INTERNALS_H__
#define __DAX_INTERNALS_H__

#include <glib.h>

/*
 * Some useful macros
 */
#ifdef __GNUC__
#define DAX_API              __attribute__ ((visibility ("default")))
#define DAX_CONSTRUCTOR      __attribute__((constructor))
#define DAX_UNUSED(x)        UNUSED_ ## x __attribute__((unused))
#else
/** mark a symbol as part of the API */
#define DAX_API
/** mark a function to be executed before main() */
#define DAX_CONSTRUCTOR
/** tells the compiler this variable/argument is not used */
#define DAX_UNUSED(x)		x
#endif /* __GNUC__ */

#define likely(x)		G_LIKELY(x)
#define unlikely(x)		G_UNLIKELY(x)
#define unused(x)		DAX_UNUSED(x)

/* no, not the International Space Station. */
#define ISS(s)       (g_intern_static_string((s)))

/* GParamSpec related macros */
#define DAX_PARAM_READABLE                       \
    G_PARAM_READABLE    | G_PARAM_STATIC_NAME  |    \
    G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB

#define DAX_PARAM_WRITABLE                       \
    G_PARAM_WRITABLE    | G_PARAM_STATIC_NAME  |    \
    G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB

#define DAX_PARAM_READWRITE                      \
    G_PARAM_READABLE    | G_PARAM_WRITABLE    |     \
    G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |     \
    G_PARAM_STATIC_BLURB

#endif /* __DAX_INTERNALS_H__ */
