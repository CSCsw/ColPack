/************************************************************************************
    Copyright (C) 2005-2008 Assefaw H. Gebremedhin, Arijit Tarafdar, Duc Nguyen,
    Alex Pothen

    This file is part of ColPack.

    ColPack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ColPack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ColPack.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************************/

#ifndef JACOBIANRECOVERY2D_H
#define JACOBIANRECOVERY2D_H

using namespace std;

namespace ColPack
{
	/** @ingroup group5
	 *  @brief class JacobianRecovery2D in @link group5@endlink.
	 */
	class JacobianRecovery2D
	{
	public: //DOCUMENTED

		/// A routine for recovering a Jacobian from a Star-Bicoloring based compressed representation.
		/**
		Parameter:
		- Input: 
			- dp2_RowCompressedMatrix: The row compressed matrix that contains all computed values. Row compressed matrix is the matrix where all rows with the same color ID (the values of m_vi_LeftVertexColors[] are equal) are merged together.
			- dp2_ColumnCompressedMatrix: The column compressed matrix that contains all computed values. Column compressed matrix is the matrix where all columns with the same color ID (the values of m_vi_RightVertexColors[] are equal) are merged together.
			- uip2_JacobianSparsityPattern.
		- Output: 
			- dp3_JacobianValue

		Precondition:
		- Star Bicoloring routine has been called.
		- uip2_JacobianSparsityPattern: The Jacobian matrix must be stored in compressed sparse rows format
		- dp3_JacobianValue is just a pointer pointing to a 2D matrix (no memory allocated yet). This matrix will be created (memory will be allocated) by DirectRecover() and the pointer will be assigned to dp3_JacobianValue

		Postcondition:
		- dp3_JacobianValue points to a 2d matrix contains the numerical values of the Jacobian. Compressed sparse rows format is used.
		
		Return value:
		- _TRUE upon successful
		
		About input parameters:
		- This routine doesn't need to take (Star) Bicoloring results (m_vi_LeftVertexColors and m_vi_RightVertexColors) of the Jacobian as another paramenter because that information is known internally already (because of the 1st precondition). 

		Compressed sparse rows format for dp3_JacobianValue:
		- This is a 2D matrix of doubles.
		- The first element of each row will specify the number of non-zeros in the Jacobian => Value of the first element + 1 will be the length of that row.
		- The value of each element after the 1st element is the value of the non-zero in the Jacobian. The value of dp3_JacobianValue[col][row] is the value of element [col][uip2_JacobianSparsityPattern[col][row]] in the real (uncompressed) Jacobian 
		- An example of compressed sparse rows format:
			- Uncompressed matrix:	<br>
		1	.5	0	<br>
		.2	2	3	<br>
		0	6	-.5	<br>
			- Corresponding uip2_JacobianSparsityPattern:	<br>
		2	0	1		<br>
		3	0	1	2	<br>
		2	1	2		<br>
			- Corresponding dp3_JacobianValue:	<br>
		2	1	.5		<br>
		3	.2	2	3	<br>
		2	6	-.5		<br>

		Algorithm: Basically the combination of RecoverForPD2RowWise() (for dp2_RowCompressedMatrix) and RecoverForPD2ColumnWise() (for dp2_ColumnCompressedMatrix) in BipartiteGraphPartialColoringInterface class
		*/
		static int DirectRecover_ADOLCFormat(BipartiteGraphBicoloringInterface* g, double** dp2_RowCompressedMatrix, double** dp2_ColumnCompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, double*** dp3_JacobianValue);

		/// A routine for recovering a Jacobian from a Star-Bicoloring based compressed representation.
		/**
		Return by recovery routine: three vectors in "Coordinate Format" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_5
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		//*/
		static int DirectRecover_CoordinateFormat(BipartiteGraphBicoloringInterface* g, double** dp2_RowCompressedMatrix, double** dp2_ColumnCompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_JacobianValue);
	};
}
#endif

