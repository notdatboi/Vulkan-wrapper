#ifndef SPARK_IMAGE_HPP
#define SPARK_IMAGE_HPP

#include"SparkIncludeBase.hpp"
#include"System.hpp"
#include"Executives.hpp"
#include"MemoryManager.hpp"
#include<optional>
#include<vector>

namespace spk
{
    class Image
    {
    public:
        Image();
        Image(const vk::Extent3D cExtent, const vk::Format cFormat, const vk::ImageUsageFlags cUsage, const vk::ImageAspectFlags cAspectFlags, const uint32_t cMipLevelCount = 1);
        Image(const Image& img);
        Image& operator=(const Image& img);
        void create(const vk::Extent3D cExtent, const vk::Format cFormat, const vk::ImageUsageFlags cUsage, const vk::ImageAspectFlags cAspectFlags, const uint32_t cMipLevelCount = 1);
        static const std::optional<vk::Format> getSupportedFormat(const std::vector<vk::Format> formats, const vk::ImageTiling tiling, const vk::FormatFeatureFlags flags);

        void recordLayoutChangeCommands(vk::CommandBuffer& layoutChangeBuffer, const vk::ImageLayout newLayout);    // just records layout change command buffer, user must report the new layot when it is changed
        void reportLayoutChange(const vk::ImageLayout newLayout);                                                   // reports, that layout has been changed using the command buffer, recorded by recordLayoutChangeCommands function

        void generateMipmap(vk::CommandBuffer& mipmapGenerateBuffer,     // generates mipmaps and changes layout to ShaderReadOnlyOptimal
            const vk::Filter filter,
            const vk::Semaphore& waitSemaphore,
            const vk::Semaphore& signalSemaphore,
            const vk::Fence& waitFence,
            const vk::Fence& signalFence,
            bool oneTimeSubmit);
        void changeLayout(vk::CommandBuffer& layoutChangeBuffer,        // records layout change command buffer and executes it immediately
            const vk::ImageLayout newLayout,
            const vk::Semaphore& waitSemaphore,
            const vk::Semaphore& signalSemaphore,
            const vk::Fence& waitFence,
            const vk::Fence& signalFence,
            bool oneTimeSubmit);
        void update(vk::CommandBuffer& updateBuffer, 
            const vk::Buffer& buffer,
            const vk::Semaphore& waitSemaphore,
            const vk::Semaphore& signalSemaphore,
            const vk::Fence& waitFence,
            const vk::Fence& signalFence,
            const vk::PipelineStageFlags dstStageFlags,
            bool oneTimeSubmit);                                          // the buffer data must be tightly packed inside the buffer; offset must be 0
        void bindMemory();
        void destroy();
        const vk::Image& getImage() const;
        const vk::Format getFormat() const;
        const vk::ImageSubresourceRange getSubresource() const;
        const vk::ImageLayout getLayout() const;
        ~Image();
    private:
        vk::Extent3D extent;
        vk::ImageLayout layout;
        vk::ImageSubresourceRange subresourceRange;
        vk::Format format;
        vk::ImageUsageFlags usage;
        vk::Image image;
        system::AllocatedMemoryData memoryData;
    };
}

#endif