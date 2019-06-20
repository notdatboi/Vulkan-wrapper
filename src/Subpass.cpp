#include"../include/Subpass.hpp"

namespace spk
{
    namespace utils
    {
        Subpass::Subpass(uint32_t id, 
            const std::vector<vk::AttachmentReference>& inputAttachments, 
            const std::vector<vk::AttachmentReference>& colorAttachments,
            const vk::AttachmentReference& depthStencilAttachment,
            const std::vector<uint32_t>& preserveAttachments,
            const vk::PipelineStageFlags stageFlags,
            const vk::AccessFlags accessFlags)
        {
            create(id, inputAttachments, colorAttachments, depthStencilAttachment, preserveAttachments, stageFlags, accessFlags);
        }

        void Subpass::create(uint32_t id, 
            const std::vector<vk::AttachmentReference>& inputAttachments, 
            const std::vector<vk::AttachmentReference>& colorAttachments,
            const vk::AttachmentReference& depthStencilAttachment,
            const std::vector<uint32_t>& preserveAttachments,
            const vk::PipelineStageFlags stageFlags,
            const vk::AccessFlags accessFlags)
        {
            index = id;
            stageMask = stageFlags,
            accessMask = accessFlags;
            description.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
            description.setInputAttachmentCount(inputAttachments.size());
            description.setPInputAttachments(inputAttachments.data());
            description.setColorAttachmentCount(colorAttachments.size());
            description.setPColorAttachments(colorAttachments.data());
            description.setPResolveAttachments(nullptr);
            description.setPDepthStencilAttachment(&depthStencilAttachment);
            description.setPreserveAttachmentCount(preserveAttachments.size());
            description.setPPreserveAttachments(preserveAttachments.data());

            const vk::Device& logicalDevice = system::System::getInstance()->getLogicalDevice();
            const vk::CommandPool& pool = system::Executives::getInstance()->getPool();

            vk::CommandBufferAllocateInfo cbInfo;
            cbInfo.setCommandBufferCount(1);
            cbInfo.setCommandPool(pool);
            cbInfo.setLevel(vk::CommandBufferLevel::eSecondary);
            if(logicalDevice.allocateCommandBuffers(&cbInfo, &secondaryCommandBuffer) != vk::Result::eSuccess) throw std::runtime_error("Failed to allocate subpass command buffer!\n");
        }

        const vk::SubpassDependency Subpass::next(const Subpass& nextSubpass)
        {
            vk::SubpassDependency dependency;
            dependency.setDependencyFlags(vk::DependencyFlagBits::eByRegion)
                .setSrcSubpass(index)
                .setDstSubpass(nextSubpass.index)
                .setSrcStageMask(stageMask)
                .setDstStageMask(nextSubpass.stageMask)
                .setSrcAccessMask(accessMask)
                .setDstAccessMask(nextSubpass.accessMask);
            return dependency;
        }

        Subpass& Subpass::beginRecording(const vk::RenderPass& renderPass, const vk::Framebuffer& framebuffer, const vk::Fence& waitFence)
        {
            if(waitFence)
            {
                const vk::Device& logicalDevice = system::System::getInstance()->getLogicalDevice();
                if(logicalDevice.waitForFences(1, &waitFence, true, ~0U) != vk::Result::eSuccess) throw std::runtime_error("Failed to wait for fences!\n");
            }

            vk::CommandBufferInheritanceInfo inheritanceInfo;
            inheritanceInfo.setRenderPass(renderPass)
                .setSubpass(index)
                .setFramebuffer(framebuffer)
                .setOcclusionQueryEnable(false);

            vk::CommandBufferBeginInfo beginInfo;
            beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eRenderPassContinue)
                .setPInheritanceInfo(&inheritanceInfo);
            if(secondaryCommandBuffer.begin(&beginInfo) != vk::Result::eSuccess) throw std::runtime_error("Failed to begin command buffer!\n");

            return *this;
        }

        Subpass& Subpass::bindDescriptorSets(const vk::PipelineLayout& layout, const std::vector<vk::DescriptorSet>& descriptorSets, const uint32_t firstSet)
        {
            secondaryCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                layout,
                firstSet,
                descriptorSets.size(),
                descriptorSets.data(),
                0,
                nullptr);

            return *this;
        }

        Subpass& Subpass::bindIndexBuffer(const vk::Buffer& indexBuffer, const vk::IndexType indexType, const uint32_t offset)
        {
            secondaryCommandBuffer.bindIndexBuffer(indexBuffer, offset, indexType);

            return *this;
        }

        Subpass& Subpass::bindVertexBuffers(const std::vector<vk::Buffer>& vertexBuffers, const std::vector<vk::DeviceSize>& offsets, const uint32_t firstBinding, const uint32_t bindingCount)
        {
            if(bindingCount == (~0))
            {
                if(offsets.size() == 0)
                {
                    std::vector<vk::DeviceSize> newOffsets(vertexBuffers.size(), 0);
                    secondaryCommandBuffer.bindVertexBuffers(firstBinding, vertexBuffers.size(), vertexBuffers.data(), newOffsets.data());
                }
                else
                {
                    secondaryCommandBuffer.bindVertexBuffers(firstBinding, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
                }
            }
            else
            {
                if(offsets.size() == 0)
                {
                    std::vector<vk::DeviceSize> newOffsets(vertexBuffers.size(), 0);
                    secondaryCommandBuffer.bindVertexBuffers(firstBinding, bindingCount, vertexBuffers.data(), newOffsets.data());
                }
                else
                {
                    secondaryCommandBuffer.bindVertexBuffers(firstBinding, bindingCount, vertexBuffers.data(), offsets.data());
                }
            }

            return *this;
        }

        Subpass& Subpass::bindPipeline(const vk::Pipeline& pipeline)
        {
            secondaryCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

            return *this;
        }

        Subpass& Subpass::draw(const uint32_t vertexCount, const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance)
        {
            secondaryCommandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);

            return *this;
        }

        Subpass& Subpass::drawIndexed(const uint32_t indexCount, const uint32_t instanceCount, const uint32_t firstIndex, const uint32_t firstInstance, const int32_t offset)
        {
            secondaryCommandBuffer.drawIndexed(indexCount, instanceCount, firstIndex, offset, firstInstance);

            return *this;
        }

        Subpass& Subpass::endRecording()
        {
            secondaryCommandBuffer.end();

            return *this;
        }

        Subpass& Subpass::reset(const bool releaseResources)
        {
            if(releaseResources) secondaryCommandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
            else secondaryCommandBuffer.reset(vk::CommandBufferResetFlags());

            return *this;
        }

        const vk::SubpassDescription Subpass::getDescription() const
        {
            return description;
        }

        const vk::CommandBuffer& Subpass::getSecondaryCommandBuffer() const
        {
            return secondaryCommandBuffer;
        }
    }
}