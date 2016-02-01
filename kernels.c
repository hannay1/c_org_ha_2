/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "willlllllllll",              /* Team name */

    "mmmhmmmmmm",     /* First member full name */
    "eduduedueduedu",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

char rotate1_descr[] = "ROTATE_1";
void rotate1(int dim, pixel *src, pixel *dst) 
{
    int i,j;

    for (i = 0; i < dim; i = i + 16)
    {
        for (j = 0; j < dim; j++)
        { 
            dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
            dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
            dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
            dst[RIDX(dim-1-j, i+4, dim)] = src[RIDX(i+4, j, dim)];
            dst[RIDX(dim-1-j, i+5, dim)] = src[RIDX(i+5, j, dim)];
            dst[RIDX(dim-1-j, i+6, dim)] = src[RIDX(i+6, j, dim)];
            dst[RIDX(dim-1-j, i+7, dim)] = src[RIDX(i+7, j, dim)];
            dst[RIDX(dim-1-j, i+8, dim)] = src[RIDX(i+8, j, dim)];
            dst[RIDX(dim-1-j, i+9, dim)] = src[RIDX(i+9, j, dim)];
            dst[RIDX(dim-1-j, i+10, dim)] = src[RIDX(i+10, j, dim)];
            dst[RIDX(dim-1-j, i+11, dim)] = src[RIDX(i+11, j, dim)];
            dst[RIDX(dim-1-j, i+12, dim)] = src[RIDX(i+12, j, dim)];
            dst[RIDX(dim-1-j, i+13, dim)] = src[RIDX(i+13, j, dim)];
            dst[RIDX(dim-1-j, i+14, dim)] = src[RIDX(i+14, j, dim)];
            dst[RIDX(dim-1-j, i+15, dim)] = src[RIDX(i+15, j, dim)]; 
                      
        }

    }

    /*
    unrolling the first loop offers ~ 245% increase in speedup by completing 16
    rotations per iteration of i instead of 1.

    */
   

}

char rotate2_descr[] = "ROTATE_2";
void rotate2(int dim, pixel *src, pixel *dst) 
{
    int i,j, ii;

    for (i = 0; i < dim; i = i + 16)
    {

        for (j = 0; j < dim; j++)
        { 

            for(ii = 0; ii < 16; ii++)
             {
                
                   dst[RIDX(dim-1-j, i+ii, dim)] = src[RIDX(i+ii, j, dim)]; 
                

             }   
             
                      
        }

    }

    /*
    unrolling the first loop offers ~ 245% increase in speedup by completing 16
    rotations per iteration of i instead of 1.

    */
   

}

char rotate3_descr[] = "ROTATE_3";
void rotate3(int dim, pixel *src, pixel *dst) 
{
    int i,j, ii;

    for (i = 0; i < dim; i = i + 16)
    {

        for (j = 0; j < dim; j++)
        { 

            for(ii = 0; ii < 16; ii++)
             {
                
                   dst[RIDX(dim-1-j, i+ii, dim)] = src[RIDX(i+ii, j, dim)]; 
                

             }   
             
                      
        }

    }

    /*

    blocking


    

    */
   

}

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    naive_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&rotate1, rotate1_descr);
    add_rotate_function(&rotate2, rotate2_descr);
    add_rotate_function(&rotate3, rotate3_descr);

    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{

    ///INLINE THIS:
    int ii, jj;
    pixel_sum sum;
    pixel_sum *sum1;
    sum1 = &sum;
    pixel current_pixel;
    pixel *curr;
    curr = &current_pixel;


    //initialize_pixel_sum(&sum);
    //pixel_sum *sum;
    sum1->red = sum1->green = sum1->blue = 0;
    sum1->num = 0;

    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
    {
    	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
        {
    	    /*accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);*/
            pixel p = src[RIDX(ii, jj, dim)];
            /*pixel pp = &p;*/
            sum1->red += (int) p.red;
            sum1->green += (int) p.green;
            sum1->blue += (int) p.blue;
            sum1->num++;
        }
    }
    
    
    curr->red = (unsigned short) (sum.red/sum.num);
    curr->green = (unsigned short) (sum.green/sum.num);
    curr->blue = (unsigned short) (sum.blue/sum.num);
    /*assign_sum_to_pixel(&current_pixel, sum);*/
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    
    naive_smooth(dim,src,dst);

    /*

    */
  
}

char smooth2_descr[] = "smooth2";
void smooth2(int dim, pixel *src, pixel *dst)
{ 
    
    int i,j;

    for (i = 0; i < dim; i++)
    {

        for (j = 0; j < dim; j++)
        { 

                    int ii, jj;
                
                    pixel_sum sum;
                    pixel_sum *sum1;
                    sum1 = &sum;
                    pixel current_pixel;
                    pixel *curr;
                    curr = &current_pixel;


                    //initialize_pixel_sum(&sum);
                    //pixel_sum *sum;
                    sum1->red = sum1->green = sum1->blue = 0;
                    sum1->num = 0;

                    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
                    {
                        for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
                        {
                            /*accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);*/
                            pixel p = src[RIDX(ii, jj, dim)];
                            /*pixel pp = &p;*/
                            sum1->red += (int) p.red;
                            sum1->green += (int) p.green;
                            sum1->blue += (int) p.blue;
                            sum1->num++;
                        }
                    }
                    
                    
                    curr->red = (unsigned short) (sum.red/sum.num);
                    curr->green = (unsigned short) (sum.green/sum.num);
                    curr->blue = (unsigned short) (sum.blue/sum.num);
             
             dst[RIDX(i, j, dim)] = current_pixel;
                      
        }

    }


}



char smooth4_descr[] = "smooth4";
void smooth4(int dim, pixel *src, pixel *dst)
{ 
    
    int i,j,x;

    for (i = 0; i < dim; i+=16)
    {

        for (j = 0; j < dim; j++)
        { 
            
            for(x = 0; x<16;x++)
            {
                    int ii, jj;
                    pixel_sum sum;
                    pixel_sum *sum1;
                    sum1 = &sum;
                    pixel current_pixel;
                    pixel *curr;
                    curr = &current_pixel;

                    //initialize_pixel_sum(&sum);
                    //pixel_sum *sum;
                    sum1->red = sum1->green = sum1->blue = 0;
                    sum1->num = 0;

                    for(ii = max(i+x-1, 0); ii <= min(i+x+1, dim-1); ii++)
                    {
                        for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
                        {
                            /*accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);*/
                            pixel p = src[RIDX(ii, jj, dim)];
                            /*pixel pp = &p;*/
                            sum1->red += (int) p.red;
                            sum1->green += (int) p.green;
                            sum1->blue += (int) p.blue;
                            sum1->num++;
                        }
                    }
                    
                    
                    curr->red = (unsigned short) (sum.red/sum.num);
                    curr->green = (unsigned short) (sum.green/sum.num);
                    curr->blue = (unsigned short) (sum.blue/sum.num);
             
                    dst[RIDX(i, j, dim)] = current_pixel;
                }      
        }

    }


}


char smooth1_descr[] = "smooth1";
void smooth1(int dim, pixel *src, pixel *dst)
{
    int j = 0;
    int i = 0;

    for (j = 0; j < dim; j+=16)
    {

        for (i = 0;  i< dim; i++)
        {

        dst[RIDX(j, i, dim)] = avg(dim, j, i, src);
        dst[RIDX(j+1, i, dim)] = avg(dim, j+1, i, src);
        dst[RIDX(j+2, i, dim)] = avg(dim, j+2, i, src);
        dst[RIDX(j+3, i, dim)] = avg(dim, j+3, i, src);
        dst[RIDX(j+4, i, dim)] = avg(dim, j+4, i, src);
        dst[RIDX(j+5, i, dim)] = avg(dim, j+5, i, src);
        dst[RIDX(j+6, i, dim)] = avg(dim, j+6, i, src);
        dst[RIDX(j+7, i, dim)] = avg(dim, j+7, i, src);
        dst[RIDX(j+8, i, dim)] = avg(dim, j+8, i, src);
        dst[RIDX(j+9, i, dim)] = avg(dim, j+9, i, src);
        dst[RIDX(j+10, i, dim)] = avg(dim, j+10, i, src);
        dst[RIDX(j+11, i, dim)] = avg(dim, j+11, i, src);
        dst[RIDX(j+12, i, dim)] = avg(dim, j+12, i, src);
        dst[RIDX(j+13, i, dim)] = avg(dim, j+13, i, src);
        dst[RIDX(j+14, i, dim)] = avg(dim, j+14, i, src);
        dst[RIDX(j+15, i, dim)] = avg(dim, j+15, i, src);
       

        }


    }

    /*

    This does not have nearly as big a performance gain (.2-.4)
    so there is probably a better way to do this.
    */
}

char smooth5_descr[] = "smooth5";
void smooth5(int dim, pixel *src, pixel *dst) 
{
    int i, j, ii;

    for (i = 0; i < dim; i+=8)
    {
        for (j = 0; j < dim; j++)
        {
            for(ii = 0; ii<8; ii++)
                    dst[RIDX(i+ii, j, dim)] = avg(dim, i+ii, j, src);
        }
    }
}





/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth1,smooth1_descr);
    add_smooth_function(&smooth2, smooth2_descr);
    /*add_smooth_function(&smooth4, smooth4_descr);*/
    add_smooth_function(&smooth5, smooth5_descr);
    /* ... Register additional test functions here */
}

