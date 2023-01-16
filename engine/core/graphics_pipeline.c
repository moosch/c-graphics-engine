#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "graphics_pipeline.h"
#include "../definitions.h"
#include "../platform/system.h"
#include "types.h"

void create_shader_module(VkDevice device, VkShaderModule *shader_module, file_info *file);
void get_binding_description(VkVertexInputBindingDescription *binding_description);
void get_attribute_descriptions(VkVertexInputAttributeDescription *attribute_descriptions);

void create_graphics_pipeline(GROEI_context *context,
                              const char *vertext_shader_path,
                              const char *fragment_shader_path) {
  file_info vert_file = {0};
  VkShaderModule vert_shader_module = {0};
  file_info frag_file = {0};
  VkShaderModule frag_shader_module = {0};

  read_file(vertext_shader_path, &vert_file);
  read_file(fragment_shader_path, &frag_file);
  create_shader_module(context->device, &vert_shader_module, &vert_file);
  create_shader_module(context->device, &frag_shader_module, &frag_file);

  VkPipelineShaderStageCreateInfo vertShader_stage_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
    .module = vert_shader_module,
    .pName = "main",
    //.pSpecializationInfo = NULL,
  };

  VkPipelineShaderStageCreateInfo frag_shader_stage_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
    .module = frag_shader_module,
    .pName = "main",
  };

  VkPipelineShaderStageCreateInfo shaderStages[] = {
    vertShader_stage_info,
    frag_shader_stage_info,
  };

  VkVertexInputBindingDescription binding_description = {0};
  get_binding_description(&binding_description);
  VkVertexInputAttributeDescription attribute_descriptions[2];
  get_attribute_descriptions(attribute_descriptions);

  VkPipelineVertexInputStateCreateInfo vertex_input_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .vertexBindingDescriptionCount = 0,
    .pVertexBindingDescriptions = NULL, // Optional
    .vertexAttributeDescriptionCount = 0,
    .pVertexAttributeDescriptions = NULL // Optional
  };

  vertex_input_info.vertexBindingDescriptionCount = 1;
  vertex_input_info.vertexAttributeDescriptionCount = 2;
  vertex_input_info.pVertexBindingDescriptions = &binding_description;
  vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions;

  u32 dynamic_states_size = 2;
  VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

  VkPipelineDynamicStateCreateInfo dynamic_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .dynamicStateCount = dynamic_states_size,
    .pDynamicStates = dynamic_states,
  };

  VkPipelineInputAssemblyStateCreateInfo input_assembly = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = VK_FALSE,
  };

  VkViewport viewport = {
    .x = 0.0f,
    .y = 0.0f,
    .width = (float)context->swap_chain_extent.width,
    .height = (float)context->swap_chain_extent.height,
    .minDepth = 0.0f,
    .maxDepth = 1.0f,
  };

  VkRect2D scissor = {
    .offset = {0, 0},
    .extent = context->swap_chain_extent,
  };

  VkPipelineViewportStateCreateInfo viewport_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .viewportCount = 1,
    .pViewports = &viewport,
    .scissorCount = 1,
    .pScissors = &scissor,
  };

  VkPipelineRasterizationStateCreateInfo rasterizer = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .lineWidth = 1.0f,
    .cullMode = VK_CULL_MODE_BACK_BIT,
    .frontFace = VK_FRONT_FACE_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .depthBiasConstantFactor = 0.0f, // Optional
    .depthBiasClamp = 0.0f, // Optional
    .depthBiasSlopeFactor = 0.0f, // Optional
  };

  VkPipelineMultisampleStateCreateInfo multisampling = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .sampleShadingEnable = VK_FALSE,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .minSampleShading = 1.0f, // Optional
    .pSampleMask = NULL, // Optional
    .alphaToCoverageEnable = VK_FALSE, // Optional
    .alphaToOneEnable = VK_FALSE, // Optional
  };

  VkPipelineColorBlendAttachmentState color_blend_attachment = {
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    .blendEnable = VK_FALSE,
    .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
    .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
    .colorBlendOp = VK_BLEND_OP_ADD, // Optional
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
    .alphaBlendOp = VK_BLEND_OP_ADD, // Optional
  };

  VkPipelineColorBlendStateCreateInfo color_blending = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_COPY, // Optional
    .attachmentCount = 1,
    .pAttachments = &color_blend_attachment,
    .blendConstants[0] = 0.0f, // Optional
    .blendConstants[1] = 0.0f, // Optional
    .blendConstants[2] = 0.0f, // Optional
    .blendConstants[3] = 0.0f, // Optional
  };

  VkPipelineLayoutCreateInfo pipeline_layout_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = 0, // Optional
    .pSetLayouts = NULL, // Optional
    .pushConstantRangeCount = 0, // Optional
    .pPushConstantRanges = NULL, // Optional
  };

  if (vkCreatePipelineLayout(context->device, &pipeline_layout_info, NULL, &context->pipeline_layout) != VK_SUCCESS) {
    printf("failed to create pipeline layout!");
    exit(GROEI_ERROR_GRAPHICS_PIPELINE_LAYOUT_CODE);
  }

  VkGraphicsPipelineCreateInfo pipeline_info = {0};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.flags = 0;
  pipeline_info.pStages = shaderStages;
  pipeline_info.pVertexInputState = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisampling;
  pipeline_info.pDepthStencilState = NULL; // Optional
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.pDynamicState = &dynamic_state;
  pipeline_info.layout = context->pipeline_layout;
  pipeline_info.renderPass = context->render_pass;
  pipeline_info.subpass = 0;
  pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
  pipeline_info.basePipelineIndex = -1; // Optional

  if (vkCreateGraphicsPipelines(context->device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &context->graphics_pipeline) != VK_SUCCESS) {
    printf("Failed to create graphics pipeline!\n");
    exit(GROEI_ERROR_GRAPHICS_PIPELINE_CODE);
  }

  free(vert_file.content);
  free(frag_file.content);
  vkDestroyShaderModule(context->device, frag_shader_module, NULL);
  vkDestroyShaderModule(context->device, vert_shader_module, NULL);
}

void create_shader_module(VkDevice device, VkShaderModule *shader_module, file_info *file) {
  VkShaderModuleCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .codeSize = file->size,
    .pCode = (u32*)file->content
  };

  if (vkCreateShaderModule(device, &create_info, NULL, shader_module) != VK_SUCCESS) {
    printf("failed to create shader module!\n");
    exit(GROEI_ERROR_CREATE_SHADER_MODULE_CODE);
  }
}

void get_attribute_descriptions(VkVertexInputAttributeDescription *attribute_descriptions) {
  attribute_descriptions[0] = (VkVertexInputAttributeDescription){0};
  attribute_descriptions[0].binding = 0;
  attribute_descriptions[0].location = 0;
  attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attribute_descriptions[0].offset = offsetof(vertex, pos);

  attribute_descriptions[1] = (VkVertexInputAttributeDescription){0};
  attribute_descriptions[1].binding = 0;
  attribute_descriptions[1].location = 1;
  attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attribute_descriptions[1].offset = offsetof(vertex, color);
}

void get_binding_description(VkVertexInputBindingDescription *binding_description) {
  binding_description->binding = 0;
  binding_description->stride = sizeof(vertex);
  binding_description->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}
