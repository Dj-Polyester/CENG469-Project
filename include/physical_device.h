#include "window.h"
#pragma once

struct PhysicalDevice
{
    VkPhysicalDeviceProperties properties{};
    VkPhysicalDeviceFeatures features{};
    VkPhysicalDevice device = VK_NULL_HANDLE;

    // queue families;
    std::vector<VkQueueFamilyProperties> queueFamilies;
    uint32_t queueFamilyCount = 0;
    std::optional<uint32_t> firstGraphicsQueueFamily;
    std::optional<uint32_t> firstPresentQueueFamily;

    const Window &win;
    PhysicalDevice(const VkPhysicalDevice _device, const Window &_win) : win(_win)
    {
        device = _device;
        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);
        // get queue families
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    }
    PhysicalDevice(const PhysicalDevice &&pdev) : win(pdev.win),
                                                  device(pdev.device),
                                                  properties(pdev.properties),
                                                  features(pdev.features)
    {
    }
    void getFirstSuitableQueueFamily()
    {
        for (size_t i = 0; i < queueFamilyCount; i++)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, win.surface, &presentSupport);

            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport)
            {
                firstGraphicsQueueFamily = i;
                firstPresentQueueFamily = i;
                break;
            }
        }
    }
    bool supportsGraphics()
    {
        getFirstSuitableQueueFamily();
        return firstGraphicsQueueFamily.has_value() && firstPresentQueueFamily.has_value();
    }
    uint32_t score() const
    {
        uint32_t score = 0;

        // Discrete GPUs have a significant performance advantage
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += properties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!features.geometryShader)
        {
            return 0;
        }

        return score;
    }
    void query()
    {
        DEBUG("Device features:");
        DEBUG2("robustBufferAccess", features.robustBufferAccess);
        DEBUG2("fullDrawIndexUint32", features.fullDrawIndexUint32);
        DEBUG2("imageCubeArray", features.imageCubeArray);
        DEBUG2("independentBlend", features.independentBlend);
        DEBUG2("geometryShader", features.geometryShader);
        DEBUG2("tessellationShader", features.tessellationShader);
        DEBUG2("sampleRateShading", features.sampleRateShading);
        DEBUG2("dualSrcBlend", features.dualSrcBlend);
        DEBUG2("logicOp", features.logicOp);
        DEBUG2("multiDrawIndirect", features.multiDrawIndirect);
        DEBUG2("drawIndirectFirstInstance", features.drawIndirectFirstInstance);
        DEBUG2("depthClamp", features.depthClamp);
        DEBUG2("depthBiasClamp", features.depthBiasClamp);
        DEBUG2("fillModeNonSolid", features.fillModeNonSolid);
        DEBUG2("depthBounds", features.depthBounds);
        DEBUG2("wideLines", features.wideLines);
        DEBUG2("largePoints", features.largePoints);
        DEBUG2("alphaToOne", features.alphaToOne);
        DEBUG2("multiViewport", features.multiViewport);
        DEBUG2("samplerAnisotropy", features.samplerAnisotropy);
        DEBUG2("textureCompressionETC2", features.textureCompressionETC2);
        DEBUG2("textureCompressionASTC_LDR", features.textureCompressionASTC_LDR);
        DEBUG2("textureCompressionBC", features.textureCompressionBC);
        DEBUG2("occlusionQueryPrecise", features.occlusionQueryPrecise);
        DEBUG2("pipelineStatisticsQuery", features.pipelineStatisticsQuery);
        DEBUG2("vertexPipelineStoresAndAtomics", features.vertexPipelineStoresAndAtomics);
        DEBUG2("fragmentStoresAndAtomics", features.fragmentStoresAndAtomics);
        DEBUG2("shaderTessellationAndGeometryPointSize", features.shaderTessellationAndGeometryPointSize);
        DEBUG2("shaderImageGatherExtended", features.shaderImageGatherExtended);
        DEBUG2("shaderStorageImageExtendedFormats", features.shaderStorageImageExtendedFormats);
        DEBUG2("shaderStorageImageMultisample", features.shaderStorageImageMultisample);
        DEBUG2("shaderStorageImageReadWithoutFormat", features.shaderStorageImageReadWithoutFormat);
        DEBUG2("shaderStorageImageWriteWithoutFormat", features.shaderStorageImageWriteWithoutFormat);
        DEBUG2("shaderUniformBufferArrayDynamicIndexing", features.shaderUniformBufferArrayDynamicIndexing);
        DEBUG2("shaderSampledImageArrayDynamicIndexing", features.shaderSampledImageArrayDynamicIndexing);
        DEBUG2("shaderStorageBufferArrayDynamicIndexing", features.shaderStorageBufferArrayDynamicIndexing);
        DEBUG2("shaderStorageImageArrayDynamicIndexing", features.shaderStorageImageArrayDynamicIndexing);
        DEBUG2("shaderClipDistance", features.shaderClipDistance);
        DEBUG2("shaderCullDistance", features.shaderCullDistance);
        DEBUG2("shaderFloat64", features.shaderFloat64);
        DEBUG2("shaderInt64", features.shaderInt64);
        DEBUG2("shaderInt16", features.shaderInt16);
        DEBUG2("shaderResourceResidency", features.shaderResourceResidency);
        DEBUG2("shaderResourceMinLod", features.shaderResourceMinLod);
        DEBUG2("sparseBinding", features.sparseBinding);
        DEBUG2("sparseResidencyBuffer", features.sparseResidencyBuffer);
        DEBUG2("sparseResidencyImage2D", features.sparseResidencyImage2D);
        DEBUG2("sparseResidencyImage3D", features.sparseResidencyImage3D);
        DEBUG2("sparseResidency2Samples", features.sparseResidency2Samples);
        DEBUG2("sparseResidency4Samples", features.sparseResidency4Samples);
        DEBUG2("sparseResidency8Samples", features.sparseResidency8Samples);
        DEBUG2("sparseResidency16Samples", features.sparseResidency16Samples);
        DEBUG2("sparseResidencyAliased", features.sparseResidencyAliased);
        DEBUG2("variableMultisampleRate", features.variableMultisampleRate);
        DEBUG2("inheritedQueries", features.inheritedQueries);

        DEBUG("Device properties:");
        DEBUG2("apiVersion", properties.apiVersion);
        DEBUG2("driverVersion", properties.driverVersion);
        DEBUG2("vendorID", properties.vendorID);
        DEBUG2("deviceID", properties.deviceID);
        DEBUG2("deviceType", getVkPhysicalDeviceType(properties.deviceType));
        DEBUG2("deviceName", properties.deviceName);
        DEBUG2("pipelineCacheUUID", properties.pipelineCacheUUID);
        DEBUG("Device limits:");
        DEBUG2("maxImageDimension1D", properties.limits.maxImageDimension1D);
        DEBUG2("maxImageDimension2D", properties.limits.maxImageDimension2D);
        DEBUG2("maxImageDimension3D", properties.limits.maxImageDimension3D);
        DEBUG2("maxImageDimensionCube", properties.limits.maxImageDimensionCube);
        DEBUG2("maxImageArrayLayers", properties.limits.maxImageArrayLayers);
        DEBUG2("maxTexelBufferElements", properties.limits.maxTexelBufferElements);
        DEBUG2("maxUniformBufferRange", properties.limits.maxUniformBufferRange);
        DEBUG2("maxStorageBufferRange", properties.limits.maxStorageBufferRange);
        DEBUG2("maxPushConstantsSize", properties.limits.maxPushConstantsSize);
        DEBUG2("maxMemoryAllocationCount", properties.limits.maxMemoryAllocationCount);
        DEBUG2("maxSamplerAllocationCount", properties.limits.maxSamplerAllocationCount);
        DEBUG2("bufferImageGranularity", properties.limits.bufferImageGranularity);
        DEBUG2("sparseAddressSpaceSize", properties.limits.sparseAddressSpaceSize);
        DEBUG2("maxBoundDescriptorSets", properties.limits.maxBoundDescriptorSets);
        DEBUG2("maxPerStageDescriptorSamplers", properties.limits.maxPerStageDescriptorSamplers);
        DEBUG2("maxPerStageDescriptorUniformBuffers", properties.limits.maxPerStageDescriptorUniformBuffers);
        DEBUG2("maxPerStageDescriptorStorageBuffers", properties.limits.maxPerStageDescriptorStorageBuffers);
        DEBUG2("maxPerStageDescriptorSampledImages", properties.limits.maxPerStageDescriptorSampledImages);
        DEBUG2("maxPerStageDescriptorStorageImages", properties.limits.maxPerStageDescriptorStorageImages);
        DEBUG2("maxPerStageDescriptorInputAttachments", properties.limits.maxPerStageDescriptorInputAttachments);
        DEBUG2("maxPerStageResources", properties.limits.maxPerStageResources);
        DEBUG2("maxDescriptorSetSamplers", properties.limits.maxDescriptorSetSamplers);
        DEBUG2("maxDescriptorSetUniformBuffers", properties.limits.maxDescriptorSetUniformBuffers);
        DEBUG2("maxDescriptorSetUniformBuffersDynamic", properties.limits.maxDescriptorSetUniformBuffersDynamic);
        DEBUG2("maxDescriptorSetStorageBuffers", properties.limits.maxDescriptorSetStorageBuffers);
        DEBUG2("maxDescriptorSetStorageBuffersDynamic", properties.limits.maxDescriptorSetStorageBuffersDynamic);
        DEBUG2("maxDescriptorSetSampledImages", properties.limits.maxDescriptorSetSampledImages);
        DEBUG2("maxDescriptorSetStorageImages", properties.limits.maxDescriptorSetStorageImages);
        DEBUG2("maxDescriptorSetInputAttachments", properties.limits.maxDescriptorSetInputAttachments);
        DEBUG2("maxVertexInputAttributes", properties.limits.maxVertexInputAttributes);
        DEBUG2("maxVertexInputBindings", properties.limits.maxVertexInputBindings);
        DEBUG2("maxVertexInputAttributeOffset", properties.limits.maxVertexInputAttributeOffset);
        DEBUG2("maxVertexInputBindingStride", properties.limits.maxVertexInputBindingStride);
        DEBUG2("maxVertexOutputComponents", properties.limits.maxVertexOutputComponents);
        DEBUG2("maxTessellationGenerationLevel", properties.limits.maxTessellationGenerationLevel);
        DEBUG2("maxTessellationPatchSize", properties.limits.maxTessellationPatchSize);
        DEBUG2("maxTessellationControlPerVertexInputComponents", properties.limits.maxTessellationControlPerVertexInputComponents);
        DEBUG2("maxTessellationControlPerVertexOutputComponents", properties.limits.maxTessellationControlPerVertexOutputComponents);
        DEBUG2("maxTessellationControlPerPatchOutputComponents", properties.limits.maxTessellationControlPerPatchOutputComponents);
        DEBUG2("maxTessellationControlTotalOutputComponents", properties.limits.maxTessellationControlTotalOutputComponents);
        DEBUG2("maxTessellationEvaluationInputComponents", properties.limits.maxTessellationEvaluationInputComponents);
        DEBUG2("maxTessellationEvaluationOutputComponents", properties.limits.maxTessellationEvaluationOutputComponents);
        DEBUG2("maxGeometryShaderInvocations", properties.limits.maxGeometryShaderInvocations);
        DEBUG2("maxGeometryInputComponents", properties.limits.maxGeometryInputComponents);
        DEBUG2("maxGeometryOutputComponents", properties.limits.maxGeometryOutputComponents);
        DEBUG2("maxGeometryOutputVertices", properties.limits.maxGeometryOutputVertices);
        DEBUG2("maxGeometryTotalOutputComponents", properties.limits.maxGeometryTotalOutputComponents);
        DEBUG2("maxFragmentInputComponents", properties.limits.maxFragmentInputComponents);
        DEBUG2("maxFragmentOutputAttachments", properties.limits.maxFragmentOutputAttachments);
        DEBUG2("maxFragmentDualSrcAttachments", properties.limits.maxFragmentDualSrcAttachments);
        DEBUG2("maxFragmentCombinedOutputResources", properties.limits.maxFragmentCombinedOutputResources);
        DEBUG2("maxComputeSharedMemorySize", properties.limits.maxComputeSharedMemorySize);
        DEBUG2("maxComputeWorkGroupCount", properties.limits.maxComputeWorkGroupCount[0]);
        DEBUG2("maxComputeWorkGroupCount", properties.limits.maxComputeWorkGroupCount[1]);
        DEBUG2("maxComputeWorkGroupCount", properties.limits.maxComputeWorkGroupCount[2]);
        DEBUG2("maxComputeWorkGroupInvocations", properties.limits.maxComputeWorkGroupInvocations);
        DEBUG2("maxComputeWorkGroupSize", properties.limits.maxComputeWorkGroupSize[0]);
        DEBUG2("maxComputeWorkGroupSize", properties.limits.maxComputeWorkGroupSize[1]);
        DEBUG2("maxComputeWorkGroupSize", properties.limits.maxComputeWorkGroupSize[2]);
        DEBUG2("subPixelPrecisionBits", properties.limits.subPixelPrecisionBits);
        DEBUG2("subTexelPrecisionBits", properties.limits.subTexelPrecisionBits);
        DEBUG2("mipmapPrecisionBits", properties.limits.mipmapPrecisionBits);
        DEBUG2("maxDrawIndexedIndexValue", properties.limits.maxDrawIndexedIndexValue);
        DEBUG2("maxDrawIndirectCount", properties.limits.maxDrawIndirectCount);
        DEBUG2("maxSamplerLodBias", properties.limits.maxSamplerLodBias);
        DEBUG2("maxSamplerAnisotropy", properties.limits.maxSamplerAnisotropy);
        DEBUG2("maxViewports", properties.limits.maxViewports);
        DEBUG2("maxViewportDimensions", properties.limits.maxViewportDimensions[0]);
        DEBUG2("maxViewportDimensions", properties.limits.maxViewportDimensions[1]);
        DEBUG2("viewportBoundsRange", properties.limits.viewportBoundsRange[0]);
        DEBUG2("viewportBoundsRange", properties.limits.viewportBoundsRange[1]);
        DEBUG2("viewportSubPixelBits", properties.limits.viewportSubPixelBits);
        DEBUG2("minMemoryMapAlignment", properties.limits.minMemoryMapAlignment);
        DEBUG2("minTexelBufferOffsetAlignment", properties.limits.minTexelBufferOffsetAlignment);
        DEBUG2("minUniformBufferOffsetAlignment", properties.limits.minUniformBufferOffsetAlignment);
        DEBUG2("minStorageBufferOffsetAlignment", properties.limits.minStorageBufferOffsetAlignment);
        DEBUG2("minTexelOffset", properties.limits.minTexelOffset);
        DEBUG2("maxTexelOffset", properties.limits.maxTexelOffset);
        DEBUG2("minTexelGatherOffset", properties.limits.minTexelGatherOffset);
        DEBUG2("maxTexelGatherOffset", properties.limits.maxTexelGatherOffset);
        DEBUG2("minInterpolationOffset", properties.limits.minInterpolationOffset);
        DEBUG2("maxInterpolationOffset", properties.limits.maxInterpolationOffset);
        DEBUG2("subPixelInterpolationOffsetBits", properties.limits.subPixelInterpolationOffsetBits);
        DEBUG2("maxFramebufferWidth", properties.limits.maxFramebufferWidth);
        DEBUG2("maxFramebufferHeight", properties.limits.maxFramebufferHeight);
        DEBUG2("maxFramebufferLayers", properties.limits.maxFramebufferLayers);
        DEBUG2("framebufferColorSampleCounts", properties.limits.framebufferColorSampleCounts);
        DEBUG2("framebufferDepthSampleCounts", properties.limits.framebufferDepthSampleCounts);
        DEBUG2("framebufferStencilSampleCounts", properties.limits.framebufferStencilSampleCounts);
        DEBUG2("framebufferNoAttachmentsSampleCounts", properties.limits.framebufferNoAttachmentsSampleCounts);
        DEBUG2("maxColorAttachments", properties.limits.maxColorAttachments);
        DEBUG2("sampledImageColorSampleCounts", properties.limits.sampledImageColorSampleCounts);
        DEBUG2("sampledImageIntegerSampleCounts", properties.limits.sampledImageIntegerSampleCounts);
        DEBUG2("sampledImageDepthSampleCounts", properties.limits.sampledImageDepthSampleCounts);
        DEBUG2("sampledImageStencilSampleCounts", properties.limits.sampledImageStencilSampleCounts);
        DEBUG2("storageImageSampleCounts", properties.limits.storageImageSampleCounts);
        DEBUG2("maxSampleMaskWords", properties.limits.maxSampleMaskWords);
        DEBUG2("timestampComputeAndGraphics", properties.limits.timestampComputeAndGraphics);
        DEBUG2("timestampPeriod", properties.limits.timestampPeriod);
        DEBUG2("maxClipDistances", properties.limits.maxClipDistances);
        DEBUG2("maxCullDistances", properties.limits.maxCullDistances);
        DEBUG2("maxCombinedClipAndCullDistances", properties.limits.maxCombinedClipAndCullDistances);
        DEBUG2("discreteQueuePriorities", properties.limits.discreteQueuePriorities);
        DEBUG2("pointSizeRange", properties.limits.pointSizeRange[0]);
        DEBUG2("pointSizeRange", properties.limits.pointSizeRange[1]);
        DEBUG2("lineWidthRange", properties.limits.lineWidthRange[0]);
        DEBUG2("lineWidthRange", properties.limits.lineWidthRange[1]);
        DEBUG2("pointSizeGranularity", properties.limits.pointSizeGranularity);
        DEBUG2("lineWidthGranularity", properties.limits.lineWidthGranularity);
        DEBUG2("strictLines", properties.limits.strictLines);
        DEBUG2("standardSampleLocations", properties.limits.standardSampleLocations);
        DEBUG2("optimalBufferCopyOffsetAlignment", properties.limits.optimalBufferCopyOffsetAlignment);
        DEBUG2("optimalBufferCopyRowPitchAlignment", properties.limits.optimalBufferCopyRowPitchAlignment);
        DEBUG2("nonCoherentAtomSize", properties.limits.nonCoherentAtomSize);
        DEBUG("Sparse properties:");
        DEBUG2("residencyStandard2DBlockShape", properties.sparseProperties.residencyStandard2DBlockShape);
        DEBUG2("residencyStandard2DMultisampleBlockShape", properties.sparseProperties.residencyStandard2DMultisampleBlockShape);
        DEBUG2("residencyStandard3DBlockShape", properties.sparseProperties.residencyStandard3DBlockShape);
        DEBUG2("residencyAlignedMipSize", properties.sparseProperties.residencyAlignedMipSize);
        DEBUG2("residencyNonResidentStrict", properties.sparseProperties.residencyNonResidentStrict);
    }
};

struct PhysicalDeviceManager
{
    std::optional<uint32_t> bestDeviceOnScoreIndex;
    uint32_t deviceCount = 0;
    std::vector<PhysicalDevice> devices;

    const Window &win;
    const Instance &instance;
    PhysicalDeviceManager(
        const Window &_win,
        const Instance &_instance)
        : win(_win),
          instance(_instance)
    {
        vkEnumeratePhysicalDevices(instance.instance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            ERROR4("Failed to find devices with Vulkan support");
        }
        std::vector<VkPhysicalDevice> devices_tmp(deviceCount);
        vkEnumeratePhysicalDevices(instance.instance, &deviceCount, devices_tmp.data());
        devices.reserve(deviceCount);
        for (size_t i = 0; i < deviceCount; i++)
        {
            devices.push_back(PhysicalDevice(devices_tmp[i], win));
        }
        debugPhysicalDevices((*this));
    }
    void queryDevices()
    {
        DEBUG2("Available devices (deviceCount)", deviceCount);
        for (size_t i = 0; i < deviceCount; ++i)
        {
            DEBUG(i);
            debugPhysicalDevice(devices[i]);
        }
    }

    PhysicalDevice &bestDeviceOnScore()
    {
        uint32_t val;
        if (bestDeviceOnScoreIndex.has_value())
        {
            return devices[bestDeviceOnScoreIndex.value()];
        }

        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<uint32_t, uint32_t> candidates;
        uint32_t i = 0;
        for (const auto &device : devices)
        {
            int score = device.score();
            candidates.insert(std::make_pair(score, i));
            ++i;
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0 && devices[candidates.rbegin()->second].supportsGraphics())
        {
            bestDeviceOnScoreIndex = candidates.rbegin()->second;
        }
        else
        {
            ERROR4("Failed to find a suitable GPU!");
        }
        return devices[bestDeviceOnScoreIndex.value()];
    }
};
