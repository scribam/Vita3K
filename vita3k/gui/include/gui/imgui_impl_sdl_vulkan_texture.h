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

#pragma once

#include <gui/imgui_impl_sdl_state.h>
#include <renderer/vulkan/state.h>
#include <vkutil/vkutil.h>

struct VKTextureData : ImguiTextureData
{
private:
    renderer::vulkan::VKState *state = nullptr;

    VkDescriptorSet DS;         // Descriptor set: this is what you'll pass to Image()

    // Need to keep track of these to properly cleanup
    vk::ImageView     ImageView;
    vk::Image         Image;
    vk::DeviceMemory  ImageMemory;
    vk::Sampler       Sampler;
    vk::Buffer        UploadBuffer;
    vk::DeviceMemory  UploadBufferMemory;

public:
    VKTextureData(renderer::vulkan::VKState *vk_state, unsigned char *data, int width, int height);
    ~VKTextureData() override;

    ImTextureID GetImTextureID() const override;
};
