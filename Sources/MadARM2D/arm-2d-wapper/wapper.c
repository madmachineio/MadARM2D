#include <stdio.h>

#include "arm_2d.h"
#include "arm_2d_helper.h"


#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif

static ARM_NOINIT arm_2d_helper_pfb_t mad_arm_2d_pfb;
static int (*draw_bitmap)(int,int,int,int,char*) = NULL;
static int (*draw_bg)(void*, void*) = NULL;

static 
IMPL_PFB_ON_DRAW(__pfb_draw_background_handler)
{
    ARM_2D_UNUSED(bIsNewFrame);

    draw_bg(ptTile, pTarget);

    return arm_fsm_rt_cpl;
}

static 
IMPL_PFB_ON_LOW_LV_RENDERING(__pfb_render_handler)
{
    const arm_2d_tile_t *ptTile = &(ptPFB->tTile);

    ARM_2D_UNUSED(pTarget);
    ARM_2D_UNUSED(bIsNewFrame);

    draw_bitmap(ptTile->tRegion.tLocation.iX,
                    ptTile->tRegion.tLocation.iY,
                    ptTile->tRegion.tSize.iWidth,
                    ptTile->tRegion.tSize.iHeight,
                    ptTile->pchBuffer);
                    
    arm_2d_helper_pfb_report_rendering_complete(&mad_arm_2d_pfb, 
                                                (arm_2d_pfb_t *)ptPFB);
}

void mad_arm_2d_init(int width, 
                        int height, 
                        int color_type, 
                        int pfb_width, 
                        int pfb_height, 
                        int (*draw)(int,int,int,int,char*),
                        int (*bg)(void*,void*))
{
    arm_2d_init();

    __attribute__((section(".bss.noinit.arm_2d_pfb_pool")))                 
         static struct {                                                        
            arm_2d_pfb_t tFPB;                                                  
            __ALIGNED(4)                                                        
            uint16_t tBuffer[(32) * (32)];                       
        } s_tPFBs[2];                                                   
                                                                                
        arm_2d_helper_pfb_cfg_t tCFG = {                                        
            .tDisplayArea.tSize = {                                             
                .iWidth = (width),                                     
                .iHeight = (height),                                   
            },                                                                 
                                                                                
            .FrameBuffer.ptPFBs = (arm_2d_pfb_t *)s_tPFBs,                      
            .FrameBuffer.tFrameSize = {                                         
                .iWidth = (32),                                            
                .iHeight = (32),                                          
            },                                                                  
            .FrameBuffer.wBufferSize = sizeof(s_tPFBs[0].tBuffer),              
            .FrameBuffer.hwPFBNum = dimof(s_tPFBs),                                                                                  
        };                                                                     
                                                                                
                                     

    if (arm_2d_helper_pfb_init(&mad_arm_2d_pfb, &tCFG) < 0) {
        //! error detected
        assert(false);
    }

    while(arm_fsm_rt_cpl != arm_2d_helper_pfb_task(&mad_arm_2d_pfb,NULL));
}
