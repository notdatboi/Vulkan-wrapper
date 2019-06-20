#ifndef SPARK_SUBPASS_HPP
#define SPARK_SUBPASS_HPP

#include"SparkIncludeBase.hpp"
#include"Executives.hpp"
#include"System.hpp"
#include<vector>

namespace spk
{
    namespace utils
    {
        class Subpass
        {
        public:
            Subpass();
            Subpass(uint32_t id, 
                const std::vector<vk::AttachmentReference>& inputAttachments, 
                const std::vector<vk::AttachmentReference>& colorAttachments,
                const vk::AttachmentReference& depthStencilAttachment,
                const std::vector<uint32_t>& preserveAttachments,
                const vk::PipelineStageFlags stageFlags,
                const vk::AccessFlags accessFlags);
            void create(uint32_t id, 
                const std::vector<vk::AttachmentReference>& inputAttachments, 
                const std::vector<vk::AttachmentReference>& colorAttachments,
                const vk::AttachmentReference& depthStencilAttachment,
                const std::vector<uint32_t>& preserveAttachments,
                const vk::PipelineStageFlags stageFlags,
                const vk::AccessFlags accessFlags);
            const vk::SubpassDependency next(const Subpass& nextSubpass);
            Subpass& beginRecording(const vk::RenderPass& renderPass, const vk::Framebuffer& framebuffer, const vk::Fence& waitFence = vk::Fence());
            Subpass& bindDescriptorSets(const vk::PipelineLayout& layout, const std::vector<vk::DescriptorSet>& descriptorSets, const uint32_t firstSet = 0);
            Subpass& bindIndexBuffer(const vk::Buffer& indexBuffer, const vk::IndexType indexType, const uint32_t offset = 0);
            Subpass& bindVertexBuffers(const std::vector<vk::Buffer>& vertexBuffers, const std::vector<vk::DeviceSize>& offsets = std::vector<vk::DeviceSize>(), const uint32_t firstBinding = 0, const uint32_t bindingCount = ~0);
            Subpass& bindPipeline(const vk::Pipeline& pipeline);
            Subpass& draw(const uint32_t vertexCount, const uint32_t instanceCount, const uint32_t firstVertex = 0, const uint32_t firstInstance = 0);
            Subpass& drawIndexed(const uint32_t indexCount, const uint32_t instanceCount, const uint32_t firstIndex = 0, const uint32_t firstInstance = 0, const int32_t offset = 0);
            Subpass& endRecording();
            Subpass& reset(const bool releaseResources);

            // Getters
            const vk::SubpassDescription getDescription() const;
            const vk::CommandBuffer& getSecondaryCommandBuffer() const;
        private:
            vk::PipelineStageFlags stageMask;
            vk::AccessFlags accessMask;
            vk::CommandBuffer secondaryCommandBuffer;
            vk::SubpassDescription description;
            uint32_t index;
        };
    }
}

#endif