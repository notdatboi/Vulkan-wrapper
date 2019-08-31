#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP
#include<vulkan/vulkan.h>
#include<System.hpp>

class Swapchain
{
public:
    Swapchain();
    void create(const System* system);
    const VkImageView& getView(const uint32_t index) const;
    const VkImage& getImage(const uint32_t index) const;
    void destroy();
private:
    VkSwapchainKHR swapchain;
    Array<VkImage> images;
    Array<VkImageView> views;
    const System* system;
};

#endif