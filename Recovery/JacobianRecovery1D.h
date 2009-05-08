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

#ifndef JACOBIANRECOVERY1D_H
#define JACOBIANRECOVERY1D_H

using namespace std;

namespace ColPack
{
	/** @ingroup group5
	 *  @brief class JacobianRecovery1D in @link group5@endlink.
	 */
	class JacobianRecovery1D
	{
	public: //DOCUMENTED

		/// A routine for recovering a Jacobian from a "Row-wise Distance 2 coloring"-based compressed representation.
		/**
		Precondition:
		- Row-wise Distance 2 coloring routine has been called.
		- uip2_JacobianSparsityPattern (input) The Jacobian matrix must be stored in compressed sparse rows format
		- dp3_JacobianValue (output) is just a pointer pointing to a 2D matrix (no memory allocated yet). This matrix will be created (memory will be allocated) by DirectRecover() and the pointer will be assigned to dp3_JacobianValue

		Postcondition:
		- dp3_JacobianValue points to a 2d matrix contains the numerical values of the Jacobian. Compressed sparse rows format is used 
		
		Return value:
		- _TRUE upon successful
		
		About input parameters:
		- This routine doesn't take (Row-wise Distance 2) coloring result m_vi_LeftVertexColors of the Jacobian as another paramenter because that information is known already (because of the 1st precondition). 

		Compressed sparse rows format for dp3_JacobianValue:
		- This is a 2D matrix of doubles.
		- The first element of each row will specify the number of non-zeros in the Jacobian => Value of the first element + 1 will be the length of that row.
		- The value of each element after the 1st element is the value of the non-zero in the Jacobian. The value of dp3_JacobianValue[col][row] is the value of element [col][uip2_JacobianSparsityPattern[col][row]] in the real (uncompressed) Jacobian 
		
		Example:
		- Uncompressed matrix:
		0	0	.5
		1.2	0	3
		0	2.3	-.5

		- Corresponding uip2_JacobianSparsityPattern:
		1	2
		2	0	2
		2	1	2

		- Corresponding dp3_JacobianValue:
		1	.5
		2	1.2	3
		2	2.3	-.5
		*/
		static int RecoverForPD2RowWise_ADOLCFormat(BipartiteGraphPartialColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, double*** dp3_JacobianValue);
		
		/// A routine for recovering a Jacobian from a "Row-wise Distance 2 coloring"-based compressed representation.
		/**
		Return by recovery routine: three vectors in "Storage Formats for the Direct Sparse Solvers" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_1
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		//*/
		static int RecoverForPD2RowWise_SparseSolversFormat(BipartiteGraphPartialColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_JacobianValue);
		
		/// A routine for recovering a Jacobian from a "Row-wise Distance 2 coloring"-based compressed representation.
		/**
		Return by recovery routine: three vectors in "Coordinate Format" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_5
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		*/
		static int RecoverForPD2RowWise_CoordinateFormat(BipartiteGraphPartialColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_JacobianValue);
		
		/// A routine for recovering a Jacobian from a "Column-wise Distance 2 coloring"-based compressed representation.
		/**
		Precondition:
		- Column-wise Distance 2 coloring routine has been called.
		- uip2_JacobianSparsityPattern (input) The Jacobian matrix must be stored in compressed sparse rows format
		- dp3_JacobianValue (output) is just a pointer pointing to a 2D matrix (no memory allocated yet). This matrix will be created (memory will be allocated) by DirectRecover() and the pointer will be assigned to dp3_JacobianValue

		Postcondition:
		- dp3_JacobianValue points to a 2d matrix contains the numerical values of the Jacobian. Compressed sparse rows format is used
		
		Return value:
		- _TRUE upon successful
		
		About input parameters:
		- This routine doesn't take (Column-wise Distance 2) coloring result m_vi_RightVertexColors of the Jacobian as another paramenter because that information is known already (because of the 1st precondition). 

		Compressed sparse rows format for dp3_JacobianValue:
		- This is a 2D matrix of doubles.
		- The first element of each row will specify the number of non-zeros in the Jacobian => Value of the first element + 1 will be the length of that row.
		- The value of each element after the 1st element is the value of the non-zero in the Jacobian. The value of dp3_JacobianValue[col][row] is the value of element [col][uip2_JacobianSparsityPattern[col][row]] in the real (uncompressed) Jacobian 
		
		Example:
		- Uncompressed matrix:
		0	0	.5
		1.2	0	3
		0	2.3	-.5

		- Corresponding uip2_JacobianSparsityPattern:
		1	2
		2	0	2
		2	1	2

		- Corresponding dp3_JacobianValue:
		1	.5
		2	1.2	3
		2	2.3	-.5
		*/
		static int RecoverForPD2ColumnWise_ADOLCFormat(BipartiteGraphPartialColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, double*** dp3_JacobianValue);

		/// A routine for recovering a Jacobian from a "Column-wise Distance 2 coloring"-based compressed representation.
		/**
		Return by recovery routine: three vectors in "Storage Formats for the Direct Sparse Solvers" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_1
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		//*/
		static int RecoverForPD2ColumnWise_SparseSolversFormat(BipartiteGraphPartialColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_JacobianValue);

		/// A routine for recovering a Jacobian from a "Column-wise Distance 2 coloring"-based compressed representation.
		/**
		Return by recovery routine: three vectors in "Coordinate Format" (zero-based indexing)
		http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_5
		unsigned int** ip2_RowIndex
		unsigned int** ip2_ColumnIndex
		double** dp2_JacobianValue // corresponding non-zero values
		//*/
		static int RecoverForPD2ColumnWise_CoordinateFormat(BipartiteGraphPartialColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_JacobianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_JacobianValue);
	};
}
#endif

