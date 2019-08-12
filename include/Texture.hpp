#ifndef SPARK_TEXTURE_HPP
#define SPARK_TEXTURE_HPP

#include<Image.hpp>
#include<HardwareImageBuffer.hpp>

namespace spk
{
    class Texture
    {
    public:
        Texture();
        Texture& setFormat(const vk::Format format);                //
        Texture& setMipmapLevelCount(const uint32_t levelCount);         // must be called before loading, 1 means no mipmaps
        Texture& loadFromImage(const Image& src);
        //Texture& copyToImage(Image& dst) const;

        const vk::Image& getImage() const;
        const vk::ImageView& getView() const;
        const vk::Sampler& getSampler() const;

        void clearResources();
        ~Texture();
    private:
        void generateMipmaps();
        void createView();
        void createSampler();
        
        HardwareImageBuffer image;
        vk::ImageView view;
        vk::Extent3D extent;
        vk::ImageUsageFlags usage;
        vk::Sampler sampler;
    };
}

#endif