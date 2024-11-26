// Vita3K emulator project
// Copyright (C) 2024 Vita3K team
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

#include <gui/imgui_impl_sdl_state.h>
#include <gui/imgui_impl_sdl_gl3_texture.h>
#include <gui/imgui_impl_sdl_vulkan_texture.h>
#include <renderer/state.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ImGui_Texture::ImGui_Texture(renderer::State *state, FILE *f) {
    this->loadTextureFromFile(state, f);
}

ImGui_Texture::ImGui_Texture(renderer::State *state, unsigned char const *buffer, int len) {
    this->loadTextureFromMemory(state, buffer, len);
}

ImGui_Texture::operator bool() const {
    if (!this->texture) {
        return false;
    }
    return texture->GetImTextureID() != 0;
}

ImGui_Texture::operator ImTextureID() const {
    if (!this->texture) {
        return 0;
    }
    return texture->GetImTextureID();
}

bool ImGui_Texture::operator==(const ImGui_Texture &texture) const {
    return this->texture->GetImTextureID() == texture.texture->GetImTextureID();
}

ImGui_Texture &ImGui_Texture::operator=(ImGui_Texture &&texture) noexcept {
    this->texture = std::move(texture.texture);
    return *this;
}

ImGui_Texture::ImGui_Texture(ImGui_Texture &&texture) noexcept
    : texture(std::move(texture.texture)) {
}

bool ImGui_Texture::loadTextureFromFile(renderer::State *state, FILE *f) {
    int height = 0;
    int width = 0;

    stbi_uc *data = stbi_load_from_file(f, &width, &height, nullptr, STBI_rgb_alpha);
    if (!data) {
        return false;
    }

    bool result = this->loadTexture(state, data, height, width);
    stbi_image_free(data);
    return result;
}

bool ImGui_Texture::loadTextureFromMemory(renderer::State *state, unsigned char const *buffer, int len) {
    int height = 0;
    int width = 0;

    stbi_uc *data = stbi_load_from_memory(buffer, len, &width, &height, nullptr, STBI_rgb_alpha);
    if (!data) {
        return false;
    }

    bool result = this->loadTexture(state, data, height, width);
    stbi_image_free(data);
    return result;
}

bool ImGui_Texture::loadTexture(renderer::State *state, unsigned char *data, int height, int width) {
    switch (state->current_backend) {
    case renderer::Backend::OpenGL:
        this->height = height;
        this->width = width;
        this->texture = std::make_unique<GLTextureData>(data, width, height);
        break;
    case renderer::Backend::Vulkan:
        this->height = height;
        this->width = width;
        this->texture = std::make_unique<VKTextureData>(dynamic_cast<renderer::vulkan::VKState*>(state), data, width, height);
        break;
    }

    return this->texture != nullptr;
}
