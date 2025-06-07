// Vita3K emulator project
// Copyright (C) 2025 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <imgui.h>

#include <cstdio>
#include <memory>

namespace renderer {
struct State;
}

struct ImguiTextureData
{
    virtual ~ImguiTextureData() = default;
    virtual ImTextureID GetImTextureID() const = 0;
};

class ImGui_Texture {
    std::unique_ptr<ImguiTextureData> texture;
    bool loadTexture(renderer::State *state, unsigned char *data, int height, int width);

public:
    int height = 0;
    int width = 0;

    ImGui_Texture() = default;
    ImGui_Texture(renderer::State *state, FILE *f);
    ImGui_Texture(renderer::State *state, unsigned char const *buffer, int len);
    ImGui_Texture(ImGui_Texture &&texture) noexcept;

    bool loadTextureFromFile(renderer::State *state, FILE *f);
    bool loadTextureFromMemory(renderer::State *state, unsigned char const *buffer, int len);

    operator bool() const;
    operator ImTextureID() const;
    bool operator==(const ImGui_Texture &texture) const;

    ImGui_Texture &operator=(ImGui_Texture &&texture) noexcept;
    ImGui_Texture &operator=(const ImGui_Texture &texture) = delete;

    ~ImGui_Texture() = default;
};
