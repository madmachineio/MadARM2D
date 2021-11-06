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

static ARM_NOINIT arm_2d_helper_pfb_t * s_tPFB;
static char s_chPerformanceInfo[MAX(((240/6)+1), 54)] = {0};
void display_task(void) 
{  
            
    /*! define dirty regions */
    IMPL_ARM_2D_REGION_LIST(s_tDirtyRegions, static const)
        
        /* a region for the busy wheel */
        ADD_REGION_TO_LIST(s_tDirtyRegions,
            .tLocation = {(240 - 80) / 2,
                          (240 - 80) / 2},
            .tSize = {
                .iWidth = 80,
                .iHeight = 80,  
            },
        ),
        
        /* a region for the status bar on the bottom of the screen */
        ADD_LAST_REGION_TO_LIST(s_tDirtyRegions,
            .tLocation = {0,240 - 8*2},
            .tSize = {
                .iWidth = 240,
                .iHeight = 8*2,  
            },
        ),

    END_IMPL_ARM_2D_REGION_LIST()
            

/*! define the partial-flushing area */

    example_gui_do_events();

    //! call partial framebuffer helper service
    while(arm_fsm_rt_cpl != arm_2d_helper_pfb_task( 
                                s_tPFB, 
                                (arm_2d_region_list_item_t *)s_tDirtyRegions));
                                        
    //! update performance info
    do {

        int32_t nTotalCyclCount = s_tPFB->Statistics.nTotalCycle;
        int32_t nTotalLCDCycCount = s_tPFB->Statistics.nRenderingCycle;

        snprintf(s_chPerformanceInfo, 
                 sizeof(s_chPerformanceInfo),
                "UPS %3d:%2dms (LCD Latency %2dms) ", 
                (int32_t)0 / nTotalCyclCount, 
                nTotalCyclCount / ((int32_t)0 / 1000),
                nTotalLCDCycCount / ((int32_t)0 / 1000));

    } while(0);

}        

__OVERRIDE_WEAK 
void example_gui_on_refresh_evt_handler(const arm_2d_tile_t *ptFrameBuffer)
{
    ARM_2D_UNUSED(ptFrameBuffer);
    
    //! print performance inf
}


static 
IMPL_PFB_ON_DRAW(__pfb_draw_handler)
{
    ARM_2D_UNUSED(pTarget);
    example_gui_refresh(ptTile, bIsNewFrame);

    arm_2d_op_wait_async(NULL);
    return arm_fsm_rt_cpl;
}

void mad_arm_2d_test(arm_2d_helper_pfb_t *p)
{
	s_tPFB = p;
	example_gui_init();

	ARM_2D_HELPER_PFB_UPDATE_ON_DRAW_HANDLER(   s_tPFB, 
                                                &__pfb_draw_handler);
    while (1) {
        display_task();
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
