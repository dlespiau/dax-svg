/* vim: set ts=4 et sw=4 sws=4:
 *
 * wsvg-internals.c - A few handy macros for internal use only
 *
 * This file is part of libwsvg
 * Copyright (C) 2009 Damien Lespiau
 *
 * libwsvg is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as published
 * by the Free Software Foundation.
 *
 * libwsvg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libwsvg.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:
 *   Damien Lespiau <damien.lespiau@gmail.com>
 */

#ifndef __WSVG_INTERNALS_H__
#define __WSVG_INTERNALS_H__

/*
 * Some useful macros
 */
#ifdef __GNUC__
#define WSVG_LIKELY(x)		__builtin_expect((x),1)
#define WSVG_UNLIKELY(x)	__builtin_expect((x),0)
#define WSVG_API		__attribute__ ((visibility ("default")))
#define WSVG_CONSTRUCTOR	__attribute__((constructor))
#define WSVG_UNUSED(x)		UNUSED_ ## x __attribute__((unused))
#else
/** mark a branch as very likeky to be taken */
#define WSVG_LIKELY(x)		(x)
/** mark a branch as very unlikeky to be taken */
#define WSVG_UNLIKELY(x)	(x)
/** mark a symbol as part of the API */
#define WSVG_API
/** mark a function to be executed before main() */
#define WSVG_CONSTRUCTOR
/** tells the compiler this variable/argument is not used */
#define WSVG_UNUSED(x)		x
#endif /* __GNUC__ */

#define likely(x)		WSVG_LIKELY(x)
#define unlikely(x)		WSVG_UNLIKELY(x)
#define unused(x)		WSVG_UNUSED(x)

#endif /* __WSVG_INTERNALS_H__ */

