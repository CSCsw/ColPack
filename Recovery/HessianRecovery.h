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

#ifndef HESSIANRECOVERY_H
#define HESSIANRECOVERY_H

using namespace std;

namespace ColPack
{
	/** @ingroup group5
	 *  @brief class HessianRecovery in @link group5@endlink.
	 */
	class HessianRecovery
	{
	public: //DOCUMENTED

		//NEED TO BE UPDATED. because now we do need to color info from GCI
		/// A routine for recovering a Hessian from a star-coloring based compressed representation.
		/**
		Parameter:
		- Input: 
			- dp2_CompressedMatrix: The compressed matrix that contains all computed values
			- uip2_HessianSparsityPattern.
		- Output: 
			- dp3_HessianValue

		Precondition:
		- Star coloring routine has been called.
		- uip2_HessianSparsityPattern: The Hessian matrix must be stored in compressed sparse rows format
		- dp3_HessianValue is just a pointer pointing to a 2D matrix (no memory allocated yet). This matrix will be created (memory will be allocated) by DirectRecover2() and the pointer will be assigned to dp3_HessianValue

		Postcondition:
		- dp3_HessianValue points to a 2d matrix contains the numerical values of the Hessian. Compressed sparse rows format is used
		
		Return value:
		- _TRUE upon successful
		
		About input parameters:
		- This routine doesn't need to take (star) coloring result m_vi_VertexColors of the Hessian as another paramenter because that information is known already (because of the 1st precondition). 

		Compressed sparse rows format for dp3_HessianValue:
		- This is a 2D matrix of doubles.
		- The first element of each row will specify the number of non-zeros in the Hessian => Value of the first element + 1 will be the length of that row.
		- The value of each element after the 1st element is the value of the non-zero in the Hessian. The value of dp3_HessianValue[col][row] is the value of element [col][uip2_HessianSparsityPattern[col][row]] in the real (uncompressed) Hessian 
		- An example of compressed sparse rows format:
			- Uncompressed matrix:	<br>
		1	.5	0	<br>
		.5	2	3	<br>
		0	3	-.5	<br>
			- Corresponding uip2_HessianSparsityPattern:	<br>
		2	0	1		<br>
		3	0	1	2	<br>
		2	1	2		<br>
			- Corresponding dp3_HessianValue:	<br>
		2	1	.5		<br>
		3	.5	2	3	<br>
		2	3	-.5		<br>

		Algorithm: optimized version of DirectRecover1 algorithm (Figure 2, pg 8, "Efficient Computation of Sparse Hessians using Coloring and Automatic Differentiation" paper).
		The complexity of this routine (DirectRecover2) is O(|E|) versus O(|E|*average distance-1 neighbour) for DirectRecover1
		- Do (column-)color statistic for each row, i.e., see how many elements in that row has color 0, color 1 ...
		Results are stored in map<int,int>* colorStatistic. colorStatistic[0] is (column-)color statistic for row 0
		If row 0 has 5 columns with color 3 => colorStatistic[0][3] = 5;
		- Allocate memory for *dp3_HessianValue
		- (Main part) Recover the values of non-zero entries in the Hessian:
		For each row, for each entry, see how many entries in that row have the same color by checking colorStatistic[row][column-color of the entry].
		If colorStatistic[#][#] == 1 => This entry has unique color (in this row). H[j,i] = B[j,color[hi]]
		else H[j,i] = B[i,color[hj]]
		Each non-zero value of the Hessian will be recovered from left to right, top to bottom
		Note: column-color of entry [row 5][column 3] is m_vi_VertexColors[column 3]
		*/
		static int DirectRecover_ADOLCFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, double*** dp3_HessianValue);

		//NEED TO BE UPDATED. because now we do need to color info from GCI
		/// A routine for recovering a Hessian from a star-coloring based compressed representation.
		/**
		Return by recovery routine: three vectors in "Coordinate Format" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_5
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		//*/
		static int DirectRecover_CoordinateFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_HessianValue);

		//NEED TO BE UPDATED. because now we do need to color info from GCI
		/// A routine for recovering a Hessian from a acyclic-coloring based compressed representation.
		/**
		Parameter:
		- Input: 
			- dp2_CompressedMatrix: The compressed matrix that contains all computed values
			- uip2_HessianSparsityPattern.
		- Output: 
			- dp3_HessianValue

		Precondition:
		- Acyclic coloring routine has been called.
		- uip2_HessianSparsityPattern: The Hessian matrix must be stored in compressed sparse rows format
		- dp3_HessianValue is just a pointer pointing to a 2D matrix (no memory allocated yet). This matrix will be created (memory will be allocated) by IndirectRecover2() and the pointer will be assigned to dp3_HessianValue

		Postcondition:
		- dp3_HessianValue points to a 2d matrix contains the numerical values of the Hessian. Compressed sparse rows format is used
		
		Return value:
		- _TRUE upon successful
		
		About input parameters:
		- This routine doesn't need to take (acyclic) coloring result m_vi_VertexColors of the Hessian as another paramenter because that information is known already (because of the 1st precondition). 

		Compressed sparse rows format for dp3_HessianValue: see DirectRecover2()

		Algorithm: created by Assefaw, 1st implemented by Arijit Tarafdar. This function is just a modification of Arijit's implementation
		*/
		static int IndirectRecover_ADOLCFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, double*** dp3_HessianValue);

		//NEED TO BE UPDATED. because now we do need to color info from GCI
		/// A routine for recovering a Hessian from a star-coloring based compressed representation.
		/**
		Return by recovery routine: three vectors in "Coordinate Format" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_5
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		//*/
		static int IndirectRecover_CoordinateFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_HessianValue);
	};
}
#endif

