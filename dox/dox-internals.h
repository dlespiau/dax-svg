/*
 * Dox - A toy navigator
 *
 * Copyright © 2010 Intel Corporation.
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DOX_INTERNALS_H__
#define __DOX_INTERNALS_H__

#include <glib.h>

/*
 * Some useful macros
 */
#ifdef __GNUC__
#define DOX_API              __attribute__ ((visibility ("default")))
#define DOX_CONSTRUCTOR      __attribute__((constructor))
#define DOX_UNUSED(x)        UNUSED_ ## x __attribute__((unused))
#else
/** mark a symbol as part of the API */
#define DOX_API
/** mark a function to be executed before main() */
#define DOX_CONSTRUCTOR
/** tells the compiler this variable/argument is not used */
#define DOX_UNUSED(x)		x
#endif /* __GNUC__ */

#define likely(x)		G_LIKELY(x)
#define unlikely(x)		G_UNLIKELY(x)
#define unused(x)		DOX_UNUSED(x)

/* no, not the International Space Station. */
#define I_(s)           (g_intern_static_string((s)))

/* GParamSpec related macros */
#define DOX_GPARAM_READABLE                         \
    G_PARAM_READABLE    | G_PARAM_STATIC_NAME  |    \
    G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB

#define DOX_GPARAM_WRITABLE                         \
    G_PARAM_WRITABLE    | G_PARAM_STATIC_NAME  |    \
    G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB

#define DOX_GPARAM_READWRITE                        \
    G_PARAM_READABLE    | G_PARAM_WRITABLE    |     \
    G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |     \
    G_PARAM_STATIC_BLURB

#endif /* __DOX_INTERNALS_H__ */
