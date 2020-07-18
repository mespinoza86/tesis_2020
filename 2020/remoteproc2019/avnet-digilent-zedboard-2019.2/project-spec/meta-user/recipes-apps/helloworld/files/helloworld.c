/*
* Copyright (C) 2013 - 2016  Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in this
* Software without prior written authorization from Xilinx.
*
*/

#include <time.h>
#include <stdio.h>

int main(int argc, char **argv)
{
     clock_t start, end;
     double cpu_time_used;
     
     start = clock();
     int mul=1;
     int sum=1;

     int times = atoi(argv[1]);
     printf("Times tiene el valor de %i\n\n",times);

     for (int i=0;i<times;i++){
	for(int x=0; x < times; x++){
		for(int y=0; y < times; y++){
			for (int z=0; z < times; z++){
				mul = multiplicar(mul,sum);
				sum++;
			}
		}
	}
     }
     end = clock();
     cpu_time_used = ((double) (end - start))*1000 / (CLOCKS_PER_SEC); 
     printf("El valor de mul es %i\n",mul);
     printf("XXXXX fun() took %f seconds to execute \n", cpu_time_used); 
    return 0;
}

int multiplicar (int a, int b){
	int result;
	result = a*b;
	return result;
}
