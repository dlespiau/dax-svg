/*
 * Castet - Load and draw SVG
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

#ifndef __CASTET_INTERNALS_H__
#define __CASTET_INTERNALS_H__

/*
 * Some useful macros
 */
#ifdef __GNUC__
#define CASTET_API              __attribute__ ((visibility ("default")))
#define CASTET_CONSTRUCTOR      __attribute__((constructor))
#define CASTET_UNUSED(x)        UNUSED_ ## x __attribute__((unused))
#else
/** mark a symbol as part of the API */
#define CASTET_API
/** mark a function to be executed before main() */
#define CASTET_CONSTRUCTOR
/** tells the compiler this variable/argument is not used */
#define CASTET_UNUSED(x)		x
#endif /* __GNUC__ */

#define likely(x)		G_LIKELY(x)
#define unlikely(x)		G_UNLIKELY(x)
#define unused(x)		CASTET_UNUSED(x)

#endif /* __CASTET_INTERNALS_H__ */
