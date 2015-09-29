#include <stdexcept>

#include "Window.h"

using namespace std;

namespace malikania {

void Window::setOnKeyUp(KeyUp function)
{
	m_keyUpList.push_back(std::move(function));
}

void Window::setOnKeyDown(KeyDown function)
{
	m_keyDownList.push_back(std::move(function));
}

void Window::setOnMouseMove(MouseMove function)
{
	m_mouseMoveList.push_back(std::move(function));
}

void Window::setOnRefresh(Refresh function)
{
	m_refreshList.push_back(std::move(function));
}

/* ------------------------------------------------------------------
 * JavaScript Window glue
 * ------------------------------------------------------------------ */

namespace js {

namespace {

duk_ret_t constructor(ContextPtr pctx)
{
	Context ctx{pctx};

	ctx.construct(std::make_shared<Window>());

	return 0;
}

duk_ret_t clear(ContextPtr pctx)
{
	Context ctx{pctx};

	ctx.self<shared_ptr<Window>>()->clear();

	return 0;
}

duk_ret_t update(ContextPtr pctx)
{
	Context ctx{pctx};

	ctx.self<shared_ptr<Window>>()->clear();

	return 0;
}

const duk_function_list_entry methods[] = {
	{ "clear",	clear,		0	},
	{ "update",	update,		0	},
	{ nullptr,	nullptr,	0	}
};

} // !js

void TypeInfo<std::shared_ptr<Window>>::prototype(Context &ctx)
{
	// TODO: 1. Create the prototype somewhere else
	// TODO: 2. Add more wrapper functions in Js.h
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
}

void registerWindow(Context &ctx)
{
	// TODO: make modules

	// Screen global constructor
	ctx.setGlobal("Screen", Function{constructor, 0});
}

} // !js

} // !malikania
