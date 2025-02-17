/*
 * long.c
 *
 * Example program for 3x3 matrix multiplication with long data.
 * It's purpose is to show the time difference when doing the same calculations with long and float format on an avr micro controller.
 *
 * Author : Emmanouil Petrakos
 * Created: 27/12/2020
 * Developed with AtmelStudio 7.0.129
 */ 

void matrix_3x3_multiplication( long input1[3][3] , long input2[3][3] , long output[3][3] );


// Declare and initialize arrays.
// Globals so they don't get optimized away.
long matrix1[3][3] =
{
	{ 1 , 2 , 3 } ,
	{ 4 , 5 , 6 } ,
	{ 7 , 8 , 9 }
};
long matrix2[3][3] =
{
	{ 1 , 2 , 3 } ,
	{ 4 , 5 , 6 } ,
	{ 7 , 8 , 9 }
};
long matrix3[3][3] =
{
	{ 0 , 0 , 0 } ,
	{ 0 , 0 , 0 } ,
	{ 0 , 0 , 0 }
};


/**
 * \brief 
 * Main function. Calls multiplication function and stops the program in a infinite loop.
 */
int main( void )
{	
	// Call multiplication function.
	// One breakpoint when the function get called,
	matrix_3x3_multiplication( matrix1 , matrix2 , matrix3 );
	// and another one when it returns to calculate delay.
	asm(""); // nop so it's easier to observe the return
	
	// Don't let the program hang.
    while (1) 
    {
    }
}


/**
 * \brief 
 * Matrix multiplication of two 3x3 arrays of longs.
 * 
 * \param inputA pointer to the 3x3 array of longs used as the multiplier.
 * \param inputB pointer to the 3x3 array of longs used as the multiplicand.
 * \param output pointer to the 3x3 array of longs where the product is saved.
 * 
 * \return void
 */
void matrix_3x3_multiplication( long inputA[3][3] , long inputB[3][3] , long output[3][3] )
{
	output[0][0] = inputA[0][0] * inputB[0][0] + inputA[0][1] * inputB[1][0] + inputA[0][2] * inputB[2][0];
	output[0][1] = inputA[0][0] * inputB[0][1] + inputA[0][1] * inputB[1][1] + inputA[0][2] * inputB[2][1];
	output[0][2] = inputA[0][0] * inputB[0][2] + inputA[0][1] * inputB[1][2] + inputA[0][2] * inputB[2][2];
	
	output[1][0] = inputA[1][0] * inputB[0][0] + inputA[1][1] * inputB[1][0] + inputA[1][2] * inputB[2][0];
	output[1][1] = inputA[1][0] * inputB[0][1] + inputA[1][1] * inputB[1][1] + inputA[1][2] * inputB[2][1];
	output[1][2] = inputA[1][0] * inputB[0][2] + inputA[1][1] * inputB[1][2] + inputA[1][2] * inputB[2][2];
	
	output[2][0] = inputA[2][0] * inputB[0][0] + inputA[2][1] * inputB[1][0] + inputA[2][2] * inputB[2][0];
	output[2][1] = inputA[2][0] * inputB[0][1] + inputA[2][1] * inputB[1][1] + inputA[2][2] * inputB[2][1];
	output[2][2] = inputA[2][0] * inputB[0][2] + inputA[2][1] * inputB[1][2] + inputA[2][2] * inputB[2][2];
}