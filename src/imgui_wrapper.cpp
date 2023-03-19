#include "sg/imgui/imgui_wrapper.h"

namespace sg::imgui {

IImGuiWrapper::IImGuiWrapper(IImGuiWrapper::on_start_t on_start_cb,
                             IImGuiWrapper::on_end_t on_end_cb,
                             IImGuiWrapper::on_iteration_t on_it_cb):
    m_on_start(on_start_cb), m_on_end(on_end_cb), m_on_iteration(on_it_cb)
{

}

void IImGuiWrapper::initalise()
{
    ImPlot::CreateContext();
    m_on_start();
}

void IImGuiWrapper::iterate(bool& done)
{
    m_on_iteration(done);
}

void IImGuiWrapper::cleanup()
{
    on_end_t();
    ImPlot::DestroyContext();
}


}
