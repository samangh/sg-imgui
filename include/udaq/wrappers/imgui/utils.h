#pragma once

#include <imgui.h>
#include "karla-font.h"

#include <string>
#include <functional>

namespace udaq::wrappers::imgui {

/* Create a version of ImGui::InputText for std::string */
bool InputText(const char* label, std::string& str, ImGuiInputTextFlags flags=0);

/* Create a numeric input for UInt32 integers */
bool InputUInt32(const char* label, uint32_t& v, ImGuiInputTextFlags flags =0);

/* Disables the ImGUI items defined in func if visibile is false */
void disable_item(bool visible, std::function<void(void)> func);

/* Sets the ImGUI font to Karla */
void set_karla_font();

}
