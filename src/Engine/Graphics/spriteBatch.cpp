#include "spriteBatch.hpp"
#include <iostream>
SpriteBatch::SpriteBatch(GraphicsContext *context)
{
    if(context == nullptr)
    {
            return;
            std::cerr << "Invalid GPU Context provided" << std::endl;
    }

    SDL_GPUShader * vert = context->loadShader("spritebatch.vert",0,1,1,0);
    SDL_GPUShader * frag = context->loadShader("spritebatch.frag",1,0,0,0);


    auto createInfo = (SDL_GPUGraphicsPipelineCreateInfo){
			.target_info = (SDL_GPUGraphicsPipelineTargetInfo){
				.num_color_targets = 1,
				.color_target_descriptions = (SDL_GPUColorTargetDescription[]){{
					.format = SDL_GetGPUSwapchainTextureFormat(context->getDevice(), context->getWindow()),
					.blend_state = {
						.enable_blend = true,
						.color_blend_op = SDL_GPU_BLENDOP_ADD,
						.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
						.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
						.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
						.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
						.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
					}
				}}
			},
			.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
			.vertex_shader = vert,
			.fragment_shader = frag
		};

	RenderPipeline = SDL_CreateGPUGraphicsPipeline(context->getDevice(), &createInfo);

    SDL_ReleaseGPUShader(context->getDevice(),vert);
    SDL_ReleaseGPUShader(context->getDevice(),frag);


    auto samplerCreateInfo = (SDL_GPUSamplerCreateInfo){
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .max_anisotropy = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    };

    sampler = SDL_CreateGPUSampler(context->getDevice(), &samplerCreateInfo);

    auto transferBufferCreateInfo = (SDL_GPUTransferBufferCreateInfo){
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = 32768 * sizeof(SpriteData)
    };

    transferBuffer = SDL_CreateGPUTransferBuffer(context->getDevice(), &transferBufferCreateInfo);

    auto bufferCreateInfo = (SDL_GPUBufferCreateInfo) {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size = 32768 * sizeof(SpriteData)
    };

    dataBuffer = SDL_CreateGPUBuffer(context->getDevice(),&bufferCreateInfo);

    auto texture = context->loadTexture("ravioli_atlas.bmp");
    auto otherTexture = context->loadTexture("Sprite-0001.png");
}

