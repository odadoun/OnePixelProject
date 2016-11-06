#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include <cheerp/webgl.h>

using namespace cheerp;
using namespace client;

void loadCallback() [[client]]
{
    auto canvas = static_cast<HTMLCanvasElement*>(document.getElementById("glcanvas"));

    auto gl = reinterpret_cast<WebGLRenderingContext*>(canvas->getContext("experimental-webgl"));
        
    gl->clearColor(0,0,0,1);
    gl->clear(gl->get_COLOR_BUFFER_BIT());
}

void webMain() [[client]]
{
    document.addEventListener("DOMContentLoaded", Callback(loadCallback));
}
