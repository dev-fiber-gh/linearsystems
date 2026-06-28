#include <boost/multiprecision/cpp_bin_float.hpp>
#include <iostream>
#include <vector>

using cpp_bin_float_512 = boost::multiprecision::number<boost::multiprecision::cpp_bin_float<512>>;
const cpp_bin_float_512 lim = 1e-256;


void transform(std::vector<std::vector<cpp_bin_float_512>>& mtx,
                    const uint32_t rowIdx, const uint32_t columnIdx) {
    for(uint32_t ri = rowIdx; ri < mtx.size(); ri++) {
        const cpp_bin_float_512 mlt = 1.0 / mtx[ri][columnIdx];
        for(uint32_t ci = columnIdx; ci < mtx[ri].size(); ci++) {
            mtx[ri][ci] *= mlt;
        }
    }
    
    if(mtx[rowIdx][columnIdx] <= 0) {
        for(uint32_t ri = rowIdx + 1; ri < mtx.size(); ri++) 
            for(uint32_t ci = columnIdx; ci < mtx[ri].size(); ci++) {
                mtx[ri][ci] += mtx[rowIdx][ci];
                
                if(mtx[ri][ci] > 0) {
                   if(mtx[ri][ci] < lim) mtx[ri][ci] = 0;
                }
                else if(mtx[ri][ci] < 0) {
                   if(mtx[ri][ci] > lim) mtx[ri][ci] = 0;
                }                   
            }
    }
    else {
        for(uint32_t ri = rowIdx + 1; ri < mtx.size(); ri++) 
            for(uint32_t ci = columnIdx; ci < mtx[ri].size(); ci++) { 
                mtx[ri][ci] -= mtx[rowIdx][ci];
                
                if(mtx[ri][ci] > 0) {
                   if(mtx[ri][ci] < lim) mtx[ri][ci] = 0;
                }
                else if(mtx[ri][ci] < 0) {
                   if(mtx[ri][ci] > lim) mtx[ri][ci] = 0;
                }
            }
    }
    
    for(uint32_t ri = 0; ri < mtx.size(); ri++) {
        for(uint32_t ci = 0; ci < mtx[ri].size(); ci++) std::cout << mtx[ri][ci] << " ";
        std::cout << "\n";
    }
    std::cout << "\n" << std::endl;
}


int main(int argc, char **argv) {
    std::vector<std::vector<cpp_bin_float_512>> mtx;
    
    /*
        Example 1 (Openstax Intermediate Algebra 2e textbook).
        
        x = -9, y = 3, z = -1
        
         /  3x + 8y + 2z = -5
        -|  2x + 5y - 3z =  0
         \   x + 2y - 2z = -1
         
         Note:
         Openstax's row-echelon form for this matrix is different (final solution is easier to calculate).
         transform() function must be more complex to generate elegant console output like this: 
         
         | 1  2 -2 -1 |
         | 0  1  1  2 |
         | 0  0  1 -1 |         
         
         Solution:
         1z = -1  ->  z = -1
         1y + 1z = 2  ->  1y = 3  ->  y = 3
         1x + 2y - 2z = -1  -> 1x + 6 + 2 = -1  ->  1x = -9  ->  x = -9
         
    */
    mtx.resize(3);
    mtx[0] = { 3, 8,  2, -5 };
    mtx[1] = { 2, 5, -3,  0 };
    mtx[2] = { 1, 2, -2, -1 };
    transform(mtx, 0, 0);
    transform(mtx, 1, 1);
    transform(mtx, 2, 2);
    /*
        Console output:
        1,  2.66667,  0.666667,  -1.66667 
        0,        1,        13,       -10 
        0,        0,         1,        -1 
    
        
        Solution:
        1z = -1  ->  z = -1
        1y + 13z = -10  ->  1y = 3  ->  y = 3
        1x + 2.66667y + 0.666667z = -1.66667  ->  1x + 8 + -0.666667 = -1.66667  ->  1x = -9  ->  x = -9 
    */

    
    
    
    /*
        Example 2 (generated with qwen 3.7):
        
        x = 1, y = 2, z = -1, w = 3
        
        /   x +  y +  z +  w =   5
       .|  2x -  y + 3z -  w = - 6
       `|  3x + 4y - 2z +  w =  16
        \   x - 2y +  z + 4w =   8
    */
    mtx.resize(4);
    mtx[0] = { 1,  1,  1,  1,  5 };
    mtx[1] = { 2, -1,  3, -1, -6 };
    mtx[2] = { 3,  4, -2,  1, 16 };
    mtx[3] = { 1, -2,  1,  4,  8 };
    transform(mtx, 0, 0);
    transform(mtx, 1, 1);
    transform(mtx, 2, 2);
    transform(mtx, 3, 3);
    /*
        Console output:
        1,  1,          1,         1,         5 
        0,  1,  -0.333333,         1,   5.33333 
        0,  0,          1,  0.642857,  0.928571 
        0,  0,          0,         1,         3 
        
        
        Solution:        
        1w = 3  ->  w = 3
        1z + 0.642857w = 0.928571      ->  1z + 1.928571 = 0.928571      ->  1z = -1  ->  z = -1
        1y - 0.333333z + 1w = 5.33333  ->  1y + 0.333333 + 3 = 5.333333  ->  1y = 2   ->  y = 2
        1x + 1y + 1z + 1w = 5          ->  1x + 2 - 1 + 3 = 5            ->  1x = 1   ->  x = 1
    */
    
    
    return 0;
}
