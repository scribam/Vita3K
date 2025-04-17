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

#include <gui/imgui_impl_sdl_vulkan_texture.h>
#include <renderer/vulkan/state.h>

#include <imgui_impl_vulkan.h>

// Helper function to find Vulkan memory type bits. See ImGui_ImplVulkan_MemoryType() in imgui_impl_vulkan.cpp
uint32_t findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t type_filter, vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties mem_properties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    return 0xFFFFFFFF; // Unable to find memoryType
}

VKTextureData::VKTextureData(renderer::vulkan::VKState *vk_state, unsigned char *data, int width, int height) {
    this->state = vk_state;
    // Calculate allocation size (in number of bytes)
    size_t image_size = width * height * 4;

    // Create the Vulkan image.
    {
        vk::ImageCreateInfo info = {};
        info.setImageType(vk::ImageType::e2D)
            .setFormat(vk::Format::eR8G8B8A8Unorm)
            .setExtent(vk::Extent3D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 })
            .setMipLevels(1)
            .setArrayLayers(1)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setTiling(vk::ImageTiling::eOptimal)
            .setUsage(vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst)
            .setSharingMode(vk::SharingMode::eExclusive)
            .setInitialLayout(vk::ImageLayout::eUndefined);
        Image = vk_state->device.createImage(info);

        vk::MemoryRequirements req = vk_state->device.getImageMemoryRequirements(Image);

        vk::MemoryAllocateInfo alloc_info{};
        alloc_info.setAllocationSize(req.size)
            .setMemoryTypeIndex(findMemoryType(vk_state->physical_device, req.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal));
        ImageMemory = vk_state->device.allocateMemory(alloc_info);

        vk_state->device.bindImageMemory(Image, ImageMemory, 0);
    }

    // Create the Image View
    {
        vk::ImageViewCreateInfo info{};
        info.setImage(Image)
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(vk::Format::eR8G8B8A8Unorm)
            .setSubresourceRange(vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor, // aspectMask
                0, // baseMipLevel
                1, // levelCount
                0, // baseArrayLayer
                1 // layerCount
            });
        ImageView = vk_state->device.createImageView(info);
    }

    // Create Sampler
    {
        // Create the sampler create info structure
        vk::SamplerCreateInfo sampler_info{};
        sampler_info.setMagFilter(vk::Filter::eLinear)
            .setMinFilter(vk::Filter::eLinear)
            .setMipmapMode(vk::SamplerMipmapMode::eLinear)
            .setAddressModeU(vk::SamplerAddressMode::eRepeat)
            .setAddressModeV(vk::SamplerAddressMode::eRepeat)
            .setAddressModeW(vk::SamplerAddressMode::eRepeat)
            .setMinLod(-1000.0f)
            .setMaxLod(1000.0f)
            .setMaxAnisotropy(1.0f);

        // Create the sampler
        Sampler = vk_state->device.createSampler(sampler_info);
    }

    // Create Descriptor Set using ImGUI's implementation
    DS = ImGui_ImplVulkan_AddTexture(Sampler, ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Create Upload Buffer
    {
        // Create the buffer create info structure
        vk::BufferCreateInfo buffer_info{};
        buffer_info.setSize(image_size)
            .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
            .setSharingMode(vk::SharingMode::eExclusive);

        // Create the buffer
        UploadBuffer = vk_state->device.createBuffer(buffer_info);

        // Get memory requirements
        vk::MemoryRequirements req = vk_state->device.getBufferMemoryRequirements(UploadBuffer);

        // Allocate memory
        vk::MemoryAllocateInfo alloc_info{};
        alloc_info.setAllocationSize(req.size)
            .setMemoryTypeIndex(findMemoryType(vk_state->physical_device, req.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible));

        UploadBufferMemory = vk_state->device.allocateMemory(alloc_info);

        // Bind buffer memory
        vk_state->device.bindBufferMemory(UploadBuffer, UploadBufferMemory, 0);
    }

    // Upload to Buffer:
    {
        void *map = nullptr;
        vk::Result result = vk_state->device.mapMemory(UploadBufferMemory, 0, image_size, {}, &map);

        // Copy data to the mapped memory
        memcpy(map, data, image_size);

        // Create a mapped memory range
        vk::MappedMemoryRange range{};
        range.setMemory(UploadBufferMemory)
            .setSize(image_size);

        // Flush the mapped memory range
        result = vk_state->device.flushMappedMemoryRanges(1, &range);

        // Unmap the memory
        vk_state->device.unmapMemory(UploadBufferMemory);
    }

    // Create a command buffer that will perform following steps when hit in the command queue.
    // TODO: this works in the example, but may need input if this is an acceptable way to access the pool/create the command buffer.
    vk::CommandPool command_pool = vk_state->frames[vk_state->current_frame_idx].render_pool;
    vk::CommandBuffer command_buffer;
    {
        vk::CommandBufferAllocateInfo alloc_info{};
        alloc_info.setCommandPool(command_pool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount(1);

        vk_state->device.allocateCommandBuffers(&alloc_info, &command_buffer);

        vk::CommandBufferBeginInfo begin_info{};
        begin_info.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        command_buffer.begin(begin_info);
    }

    // Copy to Image
    {
        // Create the image memory barrier for the copy operation
        vk::ImageMemoryBarrier copy_barrier{};
        copy_barrier.setSrcAccessMask({})
            .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
            .setOldLayout(vk::ImageLayout::eUndefined)
            .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setImage(Image)
            .setSubresourceRange(vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor, // aspectMask
                0, // baseMipLevel
                1, // levelCount
                0, // baseArrayLayer
                1 // layerCount
            });

        // Pipeline barrier for the copy operation
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eHost,
            vk::PipelineStageFlagBits::eTransfer,
            {}, // dependency flags
            0, nullptr, // memory barriers
            0, nullptr, // buffer memory barriers
            1, &copy_barrier // image memory barriers
        );

        // Define the buffer to image copy region
        vk::BufferImageCopy region{};
        region.setBufferOffset(0)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource(vk::ImageSubresourceLayers{
                vk::ImageAspectFlagBits::eColor, // aspectMask
                0, // mipLevel
                0, // baseArrayLayer
                1 // layerCount
            })
            .setImageOffset({ 0, 0, 0 })
            .setImageExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 });

        // Copy buffer to image
        command_buffer.copyBufferToImage(
            UploadBuffer,
            Image,
            vk::ImageLayout::eTransferDstOptimal,
            1, &region);

        // Create the image memory barrier for the use operation
        vk::ImageMemoryBarrier use_barrier{};
        use_barrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
            .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
            .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setImage(Image)
            .setSubresourceRange(vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor, // aspectMask
                0, // baseMipLevel
                1, // levelCount
                0, // baseArrayLayer
                1 // layerCount
            });

        // Pipeline barrier for the use operation
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eFragmentShader,
            {}, // dependency flags
            0, nullptr, // memory barriers
            0, nullptr, // buffer memory barriers
            1, &use_barrier // image memory barriers
        );
    }

    // End command buffer
    {
        // Create the submit info structure
        vk::SubmitInfo end_info{};
        end_info.setCommandBufferCount(1)
            .setPCommandBuffers(&command_buffer);

        // End the command buffer
        command_buffer.end();

        // Submit the command buffer
        vk_state->general_queue.submit(1, &end_info, nullptr);
        vk_state->device.waitIdle();
    }
}

VKTextureData::~VKTextureData() {
    this->state->device.waitIdle();
    this->state->device.freeMemory(UploadBufferMemory);
    this->state->device.destroyBuffer(UploadBuffer);
    this->state->device.destroySampler(Sampler);
    this->state->device.destroyImageView(ImageView);
    this->state->device.destroyImage(Image);
    this->state->device.freeMemory(ImageMemory);

    ImGui_ImplVulkan_RemoveTexture(DS);
}

ImTextureID VKTextureData::GetImTextureID() const {
    return (ImTextureID)this->DS;
}
