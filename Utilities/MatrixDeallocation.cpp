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

#include "MatrixDeallocation.h"

int MatrixDeallocation_SparseSolversFormat(unsigned int **ip2_RowIndex, unsigned int **ip2_ColumnIndex, double **dp2_JacobianValue) {
  //Deallocate the arrays
  delete[] (*ip2_RowIndex);
  delete ip2_RowIndex;

  delete[] (*ip2_ColumnIndex);
  delete ip2_ColumnIndex;

  delete[] (*dp2_JacobianValue);
  delete dp2_JacobianValue;

  return _TRUE;
}

int MatrixDeallocation_RowCompressedFormat(double ***dp3_HessianValue, unsigned int i_numOfRows) {
  //Deallocate the 2D Matrix
	free_2DMatrix(dp3_HessianValue, i_numOfRows);
	return _TRUE;
}


int MatrixDeallocation_CoordinateFormat(unsigned int **ip2_RowIndex, unsigned int **ip2_ColumnIndex, double **dp2_HessianValue) {
  //Deallocate the arrays
  delete[] (*ip2_RowIndex);
  delete ip2_RowIndex;

  delete[] (*ip2_ColumnIndex);
  delete ip2_ColumnIndex;

  delete[] (*dp2_HessianValue);
  delete dp2_HessianValue;

  return _TRUE;
}

