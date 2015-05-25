/*
 * Js.cpp -- JavaScript support in Malikania Engine
 *
 * Copyright (c) 2013, 2014, 2015 Malikania Authors
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Js.h"

namespace malikania {

Js::Js()
	: JsHandle(duk_create_heap_default(), duk_destroy_heap)
{
	duk_push_array(get());
	duk_put_global_string(get(), "\xff""\xff""references");
	duk_push_array(get());
	duk_put_global_string(get(), "\xff""\xff""references-stack");
	duk_push_int(get(), 0);
	duk_put_global_string(get(), "\xff""\xff""references-current");
}

/*
 * This reference any value into the internal global property "references".
 *
 * First, we check in the references-stack field if an old reference was removed and we reuse
 * this reference instead of pushing a new one.
 */
unsigned dukx_ref(duk_context *ctx)
{
	unsigned ref;

	/*
	 * 1. Check for "released" references.
	 */
	dukx_assert_begin(ctx);
	duk_get_global_string(ctx, "\xff""\xff""references-stack");
	duk_get_prop_string(ctx, -1, "length");
	if (duk_to_int(ctx, -1) > 0) {
		duk_pop(ctx);
		duk_get_prop_index(ctx, -1, 0);
		ref = duk_to_int(ctx, -1);
		duk_pop_2(ctx);
	} else {
		duk_pop_2(ctx);
		duk_get_global_string(ctx, "\xff""\xff""references-current");
		ref = duk_to_int(ctx, -1);
		duk_pop(ctx);
		duk_push_int(ctx, ref + 1);
		duk_put_global_string(ctx, "\xff""\xff""references-current");
	}
	/*
	 * 2. Store the object
	 */
	duk_get_global_string(ctx, "\xff""\xff""references");
	duk_dup(ctx, -2);
	duk_put_prop_index(ctx, -2, ref);
	duk_pop_2(ctx);
	dukx_assert_end(ctx, -1);

	return ref;
}

void dukx_refget(duk_context *ctx, unsigned ref)
{
	dukx_assert_begin(ctx);
	duk_get_global_string(ctx, "\xff""\xff""references");
	duk_get_prop_index(ctx, -1, ref);
	duk_remove(ctx, -2);
	dukx_assert_end(ctx, 1);
}

void dukx_unref(duk_context *ctx, unsigned ref)
{
	dukx_assert_begin(ctx);
	duk_get_global_string(ctx, "\xff""\xff""references-stack");
	duk_push_int(ctx, ref);
	duk_put_prop_index(ctx, -2, 0);
	duk_pop(ctx);
	dukx_assert_equals(ctx);
}

} // !malikania
