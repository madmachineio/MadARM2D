void mad_arm_2d_init(int width, 
                        int height, 
                        int color_type, 
                        int pfb_width, 
                        int pfb_height, 
                        int (*draw)(int,int,int,int,char*),
                        int (*bg)(void*,void*));