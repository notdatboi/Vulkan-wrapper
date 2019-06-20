#ifndef SPARK_SHADER_SET_HPP
#define SPARK_SHADER_SET_HPP

#include"SparkIncludeBase.hpp"
#include<vector>
#include<string>
#include<fstream>
#include"System.hpp"

namespace spk
{
    enum class ShaderType
    {
        Vertex, 
        Fragment
    };

    struct ShaderInfo
    {
        ShaderType type;
        std::string filename;
    };

    class ShaderSet
    {
    public:
        ShaderSet();
        ShaderSet(const ShaderSet& set);
        ShaderSet(const std::vector<ShaderInfo>& shaders);
        void create(const std::vector<ShaderInfo>& shaders);
        ShaderSet& operator=(const ShaderSet& set);
        const std::vector<vk::PipelineShaderStageCreateInfo>& getShaderStages() const;
        const uint32_t getIdentifier() const;
        ~ShaderSet();
    private:
        std::vector<ShaderInfo> infos;
        std::vector<char> getCode(const std::string& filename) const;
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        std::vector<std::pair<vk::ShaderModule, vk::ShaderStageFlagBits> > shaderModules;
        void destroy();
    };
}

#endif