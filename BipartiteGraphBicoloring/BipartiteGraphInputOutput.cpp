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

#include "ColPackHeaders.h"

using namespace std;

namespace ColPack
{
	//Private Function 2201;3201
	void BipartiteGraphInputOutput::CalculateVertexDegrees()
	{
		int i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());

		int i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());
		
		int i_TotalLeftVertexDegree = _FALSE;
		
		int i_TotalRightVertexDegree = _FALSE;

		i_TotalLeftVertexDegree = i_TotalRightVertexDegree = m_vi_Edges.size()/2;

		for(int i = 0; i < i_LeftVertexCount; i++)
		{
			int i_VertexDegree = m_vi_LeftVertices[i + 1] - m_vi_LeftVertices[i];

			if(m_i_MaximumLeftVertexDegree < i_VertexDegree)
			{
				m_i_MaximumLeftVertexDegree = i_VertexDegree;
			}

			if(m_i_MinimumLeftVertexDegree == _UNKNOWN)
			{
				m_i_MinimumLeftVertexDegree = i_VertexDegree;
			}
			else if(m_i_MinimumLeftVertexDegree > i_VertexDegree)
			{
				m_i_MinimumLeftVertexDegree = i_VertexDegree;
			}
		}

		for(int i = 0; i < i_RightVertexCount; i++)
		{
			int i_VertexDegree = m_vi_RightVertices[i + 1] - m_vi_RightVertices[i];

			if(m_i_MaximumRightVertexDegree < i_VertexDegree)
			{
				m_i_MaximumRightVertexDegree = i_VertexDegree;
			}

			if(m_i_MinimumRightVertexDegree == _UNKNOWN)
			{
				m_i_MinimumRightVertexDegree = i_VertexDegree;
			}
			else if(m_i_MinimumRightVertexDegree > i_VertexDegree)
			{
				m_i_MinimumRightVertexDegree = i_VertexDegree;
			}
		}

		m_i_MaximumVertexDegree = m_i_MaximumLeftVertexDegree>m_i_MaximumRightVertexDegree?m_i_MaximumLeftVertexDegree:m_i_MaximumRightVertexDegree;
		m_i_MinimumVertexDegree = m_i_MinimumLeftVertexDegree<m_i_MinimumRightVertexDegree?m_i_MinimumLeftVertexDegree:m_i_MinimumRightVertexDegree;

		m_d_AverageLeftVertexDegree = (double)i_TotalLeftVertexDegree/i_LeftVertexCount;
		m_d_AverageRightVertexDegree = (double)i_TotalRightVertexDegree/i_RightVertexCount;
		m_d_AverageVertexDegree = (double)(i_TotalLeftVertexDegree + i_TotalRightVertexDegree)/(i_LeftVertexCount + i_RightVertexCount);

		return;

	}

	//Public Constructor 2251;3251
	BipartiteGraphInputOutput::BipartiteGraphInputOutput()
	{
		Clear();
	}

	//Public Destructor 2252;3252
	BipartiteGraphInputOutput::~BipartiteGraphInputOutput()
	{
		Clear();
	}

	//Virtual Function 2253;3254
	void BipartiteGraphInputOutput::Initialize()
	{
		
	}

	//Virtual Function 2254;3254
	void BipartiteGraphInputOutput::Clear()
	{
		BipartiteGraphCore::Clear();

		return;
	}

	//Public Function 2255;3255
	int BipartiteGraphInputOutput::ReadMatrixMarketBipartiteGraph(string s_InputFile)
	{
		int i, j;

		int i_LineCount;
	    
		int i_RowCount, i_ColumnCount;

		int i_LeftVertex, i_RightVertex;

		int i_LeftVertexCount, i_RightVertexCount;

		int i_VertexDegree;

		int i_EdgeCount;
	    
		string _GAP(" ");
	    
		string s_InputLine;
	    
		ifstream InputStream;
	    
		vector<string> vs_InputTokens;
	    
		vector< vector<int> > v2i_LeftVertexAdjacency, v2i_RightVertexAdjacency;

		Clear();

		i_EdgeCount = _FALSE;
		i_LineCount = _TRUE;
	    
		i_LeftVertexCount = i_RightVertexCount = _FALSE;

		m_s_InputFile = s_InputFile;
			
		InputStream.open(m_s_InputFile.c_str());

		if(!InputStream)
		{
			cout<<"File "<<m_s_InputFile<<" Not Found"<<endl;
			return _FALSE;
		}
		else
		{
			cout<<"Found File "<<m_s_InputFile<<endl;
		}

		do
		{
			getline(InputStream, s_InputLine);
			
			if(s_InputLine[0]=='%') 
			{
				continue;
			}

			if(!InputStream)
			{
				break;
			}
	
			if(i_LineCount == _TRUE)
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
			    
				vs_InputTokens.clear();
			    
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}
			    
				i_RowCount = atoi(vs_InputTokens[0].c_str());
				i_ColumnCount = atoi(vs_InputTokens[1].c_str());

				i_LeftVertexCount = i_RowCount;
				i_RightVertexCount = i_ColumnCount;

				i_EdgeCount = atoi(vs_InputTokens[2].c_str());

				v2i_LeftVertexAdjacency.clear();
				v2i_LeftVertexAdjacency.resize((unsigned) i_LeftVertexCount);

				v2i_RightVertexAdjacency.clear();
				v2i_RightVertexAdjacency.resize((unsigned) i_RightVertexCount);
			    
			}
			
			if((i_LineCount > _TRUE) && (i_LineCount <= STEP_UP(i_EdgeCount))) 
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
			    
				vs_InputTokens.clear();
			    
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}

				i_LeftVertex = atoi(vs_InputTokens[0].c_str());
				i_RightVertex = atoi(vs_InputTokens[1].c_str());

				v2i_LeftVertexAdjacency[STEP_DOWN(i_LeftVertex)].push_back(STEP_DOWN(i_RightVertex));
				v2i_RightVertexAdjacency[STEP_DOWN(i_RightVertex)].push_back(STEP_DOWN(i_LeftVertex));
			}
		
			i_LineCount++;

		}
		while(InputStream);
	    
		InputStream.close();	

		for(i=0; i<i_LeftVertexCount; i++)
		{
			m_vi_LeftVertices.push_back((signed) m_vi_Edges.size());

			i_VertexDegree = (signed) v2i_LeftVertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_LeftVertexAdjacency[i][j]);
			}
		}

		m_vi_LeftVertices.push_back((signed) m_vi_Edges.size());

		for(i=0; i<i_RightVertexCount; i++)
		{
			m_vi_RightVertices.push_back((signed) m_vi_Edges.size());

			i_VertexDegree = (signed) v2i_RightVertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_RightVertexAdjacency[i][j]);
			}
		}
	    
		m_vi_RightVertices.push_back((signed) m_vi_Edges.size());

		CalculateVertexDegrees();


#if DEBUG == 2255 || DEBUG == 3255

		int k;
	    
		cout<<endl;
		cout<<"DEBUG 2255;3255 | Graph Coloring | Left Vertex Adjacency | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_LeftVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = mvi_LeftVertices[STEP_UP(i)] - mvi_LeftVertices[i];

			k = _FALSE;

			for(j=mvi_LeftVertices[i]; j<mvi_LeftVertices[STEP_UP(i)]; j++)
			{
				if(k == STEP_DOWN(i_VertexDegree))
				{
					cout<<STEP_UP(m_vi_Edges[j])<<" ("<<i_VertexDegree<<") ";
				}
				else
				{
					cout<<STEP_UP(m_vi_Edges[j])<<", ";
				}

				k++;
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"DEBUG 2255;3255 | Graph Coloring | Right Vertex Adjacency | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_RightVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = mvi_RightVertices[STEP_UP(i)] - mvi_RightVertices[i];

			k = _FALSE;

			for(j=mvi_RightVertices[i]; j<mvi_RightVertices[STEP_UP(i)]; j++)
			{
				if(k == STEP_DOWN(i_VertexDegree))
				{
					cout<<STEP_UP(m_vi_Edges[j])<<" ("<<i_VertexDegree<<") ";
				}
				else
				{
					cout<<STEP_UP(m_vi_Edges[j])<<", ";
				}

				k++;
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Left Vertices = "<<i_LeftVertexCount<<"; Right Vertices = "<<i_RightVertexCount<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}
	
	//Public Function 2256;3256
	int BipartiteGraphInputOutput::ReadMeTiSBipartiteGraph(string s_InputFile)
	{
		Clear();

		m_s_InputFile=s_InputFile;
		ifstream InputStream (m_s_InputFile.c_str());

		if(!InputStream)
		{
			cout<<"File "<<m_s_InputFile<<" Not Found"<<endl;
			return _FALSE;
		}
		else
		{
			cout<<"Found File "<<m_s_InputFile<<endl;
		}
		
		//initialize local data
		int rowCounter=0, row=0, edges=0, num=0, numCount=0;
		istringstream in2;
		string line="";
		map<int,vector<int> > colList;

		getline(InputStream,line);
		rowCounter++;
		in2.str(line);
		in2>>row>>edges;
		m_vi_LeftVertices.push_back(m_vi_Edges.size());

		while(!InputStream.eof())
		{
			getline(InputStream,line);
			if(line!="")
			{
				//cout<<"["<<lineCount<<"] \""<<line<<"\""<<endl;
				in2.clear();
				in2.str(line);
				while(in2>>num)
				{
					num--;
					m_vi_Edges.push_back(num);
					colList[num].push_back(rowCounter-1);
					numCount++;
					//cout<<"\tpush_back "<<num<<endl;
					//cout<<"\tnumCount="<<numCount<<endl;
				}
			}
			rowCounter++;
			m_vi_LeftVertices.push_back(m_vi_Edges.size());
		}
		rowCounter--;
		m_vi_LeftVertices.pop_back();
		if(rowCounter!=row+1 || edges*2!=numCount)
		{
			cout<<"Read fail: rowCounter!=row+1 || edges*2!=numCount"<<endl;
			cout<<"Read fail: "<<rowCounter<<"!="<<row+1<<" || "<<edges*2<<"!="<<numCount<<endl;
			return _FALSE;
		}

		//put together the right vertices
		m_vi_RightVertices.push_back(m_vi_Edges.size());
		for(int i=0;i<row; i++) {
			m_vi_Edges.insert(m_vi_Edges.end(),colList[i].begin(),colList[i].end());
			m_vi_RightVertices.push_back(m_vi_Edges.size());
		}

		/*
		cout<<"--------------------------------------------------------"<<endl;
		cout<<"numCount="<<numCount<<endl;
		cout<<"lineCount="<<lineCount<<endl;
		cout<<"Left vector:";
		for(int i=0;i<m_vi_LeftVertices.size();i++) cout<<"["<<i<<"] "<<m_vi_LeftVertices[i]<<"; ";
		cout<<endl<<"Right vector:";
		for(int i=0;i<m_vi_RightVertices.size();i++) cout<<"["<<i<<"] "<<m_vi_RightVertices[i]<<"; ";
		cout<<endl<<"Edges vector:";
		for(int i=0;i<m_vi_Edges.size();i++) cout<<"["<<i<<"] "<<m_vi_Edges[i]<<"; ";
		cout<<endl<<"--------------------------------------------------------"<<endl;
		//*/
	    
		CalculateVertexDegrees();

		return(_TRUE);
	}
	
	//Public Function 2257;3257
	int BipartiteGraphInputOutput::ReadHarwellBoeingBipartiteGraph(string s_InputFile)
	{
		char sz_Line [256];
		int i_Dummy, i_RHSCRD, i_Input;
		int i_Rows, i_Columns, i_NZ;
		int i, j, n;
		int i_RowCount;
		ifstream InputStream;
		vector<int> vi_Vertex;
		vector<int> vi_Degree, vi_RowPointer;
	    
		Clear();

		m_s_InputFile = s_InputFile;

		InputStream.open ( m_s_InputFile.c_str () );
	    

		if(!InputStream)
		{
			cout<<"File "<<m_s_InputFile<<" Not Found"<<endl;
			return _FALSE;
		}
		else
		{
			cout<<"Found File "<<m_s_InputFile<<endl;
		}

	    
		// get headings
		// line1: who cares about title...
		InputStream.getline ( sz_Line, 255 );

		// line2: ignore everything except for RHSCRD which is the amount of additional lines we might need to compute
		InputStream >> i_Dummy >> i_Dummy >> i_Dummy >> i_Dummy >> i_RHSCRD;
		InputStream.getline ( sz_Line, 255 );
		//sscanf ( sz_Line, "%d %d %d %d %d", &i_Dummy, &i_Dummy, &i_Dummy, &i_Dummy, &i_RHSCRD );

		// line3: matrix type (3 characters), # of rows, # of columns, # of nonzeros, ignored
		//InputStream.getline ( sz_Line, 255 );
		//DOESN'T WORK sscanf_s ( sz_Line, "%c%c%c %d %d %d %d", &c_dummy, &c_dummy, &c_dummy, &i_Rows, &i_Columns, &i_NZ, &i_Dummy );
		string format;
		InputStream >> format >> i_Rows >> i_Columns >> i_NZ;
		InputStream.getline ( sz_Line, 255 );

		// line4: ignored
		InputStream.getline ( sz_Line, 255 );

		// check to see if "readmore" (RHSCRD) is presented in this file
		if ( i_RHSCRD > 0 )
		{
			// ignore that line
			InputStream.getline ( sz_Line, 255 );
		}


		// initialize stuff
		n = i_Rows + i_Columns;
	    
		// build up initial setup
		m_vi_LeftVertices.resize ( i_Rows+1, 0 );
		m_vi_RightVertices.resize ( i_Columns+1, 0 );
		m_vi_Edges.resize ( 2 * i_NZ, 0 );
		vi_Degree.resize ( n, 0 );
		vi_RowPointer.resize ( i_Rows, 0 );
		vi_Vertex.resize ( n + 1, 0 );
	    
		// read matrix
		InputStream >> i_Dummy;
		for ( i=1; i<=i_Columns; i++ )
		{
			InputStream >> i_Input;
			vi_Vertex [i] = i_Input - 1;
			vi_Degree [i-1] = vi_Vertex [i] - vi_Vertex [i-1];
		}
		for ( i=0; i<i_NZ; i++ )
		{
			InputStream >> i_Input;
			m_vi_Edges [i] = i_Columns + i_Input - 1;
			++vi_RowPointer [i_Input - 1];
			++vi_Degree [i_Columns + i_Input - 1];
		}
		// done with InputStream?
		InputStream.close ();
		// build 2nd half of graph
		i_RowCount = i_NZ;
		for ( i=1; i<=i_Rows; i++ )
		{
			i_RowCount += vi_RowPointer [i-1];
			vi_Vertex [i + i_Columns] = i_RowCount;
		}
		for ( i=0; i<i_Columns; i++ )
		{
			for ( j=vi_Vertex [i]; j<vi_Vertex [i+1]; j++ )
			{
				if ( vi_RowPointer [m_vi_Edges [j] - i_Columns] > 0 )
				{
					--vi_RowPointer [m_vi_Edges [j] - i_Columns];
					m_vi_Edges [vi_Vertex [m_vi_Edges [j]] + vi_RowPointer [m_vi_Edges [j] - i_Columns]] = i;
				}
			}
		}
	    
		// reverse everything
		for ( i=0; i<i_Columns; i++ )
		{
			m_vi_RightVertices [i] = vi_Vertex [i];
		}
			
		m_vi_RightVertices [i_Columns] = i_NZ;
	    
		for ( i=0; i<i_Rows; i++ )
		{
			m_vi_LeftVertices [i] = vi_Vertex [i+i_Columns];
		}
	    
		m_vi_LeftVertices [i_Rows] = 2*i_NZ;
	    
		for ( i=0; i<i_NZ; i++ )
		{
			m_vi_Edges [i] = m_vi_Edges [i] - i_Columns;
		}
	    
		CalculateVertexDegrees();

		return(_TRUE);
	}


	
	//Public Function 2258;3258
	int BipartiteGraphInputOutput::ReadGenericMatrixBipartiteGraph(string s_InputFile)
	{
		Clear();

		m_s_InputFile=s_InputFile;
		
		//initialize local data
		int rowCounter=0, colCounter=0, row=0, col=0, tempNum=0;
		istringstream in2;
		string line="";

		map< int,vector<int> > colList;

		ifstream InputStream (m_s_InputFile.c_str());
		if(!InputStream)
		{
			cout<<"Not Found File "<<m_s_InputFile<<endl;
		}
		else
		{
			cout<<"Found File "<<m_s_InputFile<<endl;
		}

		//now find the dimension of the matrix
		string sRow="", sCol="";
		int tempCounter=s_InputFile.size()-1;
		bool getRow=0, firstTime=1;
		//read the file name from right to left, get number of rows and cols
		for(; tempCounter>-1;tempCounter--)
		{
			if(s_InputFile[tempCounter]=='\\') {tempCounter=0; continue;}//end of the filename
			if(getRow)
			{
				if(s_InputFile[tempCounter]<'0'||s_InputFile[tempCounter]>'9')
				{
					if(firstTime) continue;
					else  break;
				}
				else firstTime=0;
				sRow=s_InputFile[tempCounter]+sRow;
			}
			else
			{
				//touch the "by", switch to getRow
				if(s_InputFile[tempCounter]<'0'||s_InputFile[tempCounter]>'9')
				{
					if(firstTime) continue;
					else {firstTime=1;getRow=1; continue;}
				}
				else firstTime=0;
				sCol=s_InputFile[tempCounter]+sCol; //finish with sCol, switch to sRow
			}
		}
		if (tempCounter==-1)
		{
			cout<<"Input file\""<<s_InputFile<<"\" has a wrong name format"<<endl;
			return _FALSE;
		}
		in2.clear();in2.str(sRow);in2>>row;
		in2.clear();in2.str(sCol);in2>>col;
		cout<<"Matrix: "<<row<<" x "<<col<<endl;

		//Start reading the graph, row by row
		m_vi_LeftVertices.push_back(m_vi_Edges.size());
		for(;rowCounter<row;rowCounter++)
		{
			colCounter=0;
			getline(InputStream,line);
			if(line=="") break;
			in2.clear(); in2.str(line);
			while(in2>>tempNum)
			{
				if(tempNum)
				{
					m_vi_Edges.push_back(colCounter);
					colList[colCounter].push_back(rowCounter);
				}
				colCounter++;
			}
			m_vi_LeftVertices.push_back(m_vi_Edges.size());
			if (colCounter!=col)
			{
				cerr<<"WARNING: BipartiteGraphInputOutput::ReadGenericMatrixBipartiteGraph()"<<endl;
				cerr<<"Input file\""<<s_InputFile<<"\" has a wrong format. The number of entries in 1 column < # of columns"<<endl;
				return _FALSE;
			}
		}
		if (rowCounter!=row)
		{
			cerr<<"WARNING: BipartiteGraphInputOutput::ReadGenericMatrixBipartiteGraph()"<<endl;
			cout<<"Input file\""<<s_InputFile<<"\" has a wrong format. The number of rows is less than what it suppose to be"<<endl;
			return _FALSE;
		}
		//put together the right vertices
		m_vi_RightVertices.push_back(m_vi_Edges.size());
		for(int i=0;i<col; i++) {
			m_vi_Edges.insert(m_vi_Edges.end(),colList[i].begin(),colList[i].end());
			m_vi_RightVertices.push_back(m_vi_Edges.size());
		}
	    
		CalculateVertexDegrees();

		return (_TRUE);
	}
	
	//Public Function 2259;3259
	int BipartiteGraphInputOutput::ReadGenericSquareMatrixBipartiteGraph(string s_InputFile)
	{
		Clear();

		m_s_InputFile=s_InputFile;
		//initialize local data
		int rowCounter=0, colCounter=0, counter=0, row=0, col=0;
		istringstream in2;
		string line="", templ="";

		map< int,vector<int> > colList;

		ifstream InputStream (m_s_InputFile.c_str());
		if(!InputStream)
		{
			cout<<"Not Found File "<<m_s_InputFile<<endl;
		}
		else
		{
			cout<<"Found File "<<m_s_InputFile<<endl;
		}

		//now find the dimension of the matrix
		string sRow="", sCol="";
		int tempCounter=s_InputFile.size()-1;
		bool getRow=0, firstTime=1;
		//read the file name from right to left, get number of rows and cols
		for(; tempCounter>-1;tempCounter--)
		{
			if(s_InputFile[tempCounter]=='\\') {tempCounter=0; continue;}//end of the filename
			if(getRow)
			{
				if(s_InputFile[tempCounter]<'0'||s_InputFile[tempCounter]>'9')
				{
					if(firstTime) continue;
					else  break;
				}
				else firstTime=0;
				sRow=s_InputFile[tempCounter]+sRow;
			}
			else
			{
				//touch the "by", switch to getRow
				if(s_InputFile[tempCounter]<'0'||s_InputFile[tempCounter]>'9')
				{
					if(firstTime) continue;
					else {firstTime=1;getRow=1; continue;}
				}
				else firstTime=0;
				sCol=s_InputFile[tempCounter]+sCol; //finish with sCol, switch to sRow
			}
		}
		if (tempCounter==-1)
		{
			cout<<"Input file\""<<s_InputFile<<"\" has a wrong name format"<<endl;
			return _FALSE;
		}
		in2.clear();in2.str(sRow);in2>>row;
		in2.clear();in2.str(sCol);in2>>col;
		if(row>col) row=col; else col=row;
		cout<<"Matrix: "<<row<<" x "<<col<<endl;

		//get data
		while(!InputStream.eof())
		{
			getline(InputStream,templ);
			line+=templ;
		}

		//Start reading the graph, entry by entry
		m_vi_LeftVertices.push_back(m_vi_Edges.size());
		counter=0;
		for(;rowCounter<row;rowCounter++)
		{
			for(colCounter=0;colCounter<row;colCounter++)
			{
				if(line[counter]=='1')
				{
					m_vi_Edges.push_back(colCounter);
					colList[colCounter].push_back(rowCounter);
				}
				counter++;
			}
			m_vi_LeftVertices.push_back(m_vi_Edges.size());
		}

		//put together the right vertices
		m_vi_RightVertices.push_back(m_vi_Edges.size());
		for(int i=0;i<col; i++) {
			m_vi_Edges.insert(m_vi_Edges.end(),colList[i].begin(),colList[i].end());
			m_vi_RightVertices.push_back(m_vi_Edges.size());
		}
	    
		CalculateVertexDegrees();

		return (_TRUE);
	}
	
	//Public Function 2260;3260
	void BipartiteGraphInputOutput::PrintBipartiteGraph()
	{   
		int i, j, k;
	 
		int i_LeftVertexCount, i_RightVertexCount;
	   
		int i_EdgeCount;

		int i_VertexDegree;

		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());
	   
		i_EdgeCount = (signed) m_vi_Edges.size();

		cout<<endl;
		cout<<"Bipartite Graph | Left Vertex Adjacency | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_LeftVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = m_vi_LeftVertices[STEP_UP(i)] - m_vi_LeftVertices[i];

			k = _FALSE;

			for(j=m_vi_LeftVertices[i]; j<m_vi_LeftVertices[STEP_UP(i)]; j++)
			{
				if(k == STEP_DOWN(i_VertexDegree))
				{
					cout<<STEP_UP(m_vi_Edges[j])<<" ("<<i_VertexDegree<<") ";
				}
				else
				{
					cout<<STEP_UP(m_vi_Edges[j])<<", ";
				}

				k++;
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"Bipartite Graph | Right Vertex Adjacency | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_RightVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = m_vi_RightVertices[STEP_UP(i)] - m_vi_RightVertices[i];

			k = _FALSE;

			for(j=m_vi_RightVertices[i]; j<m_vi_RightVertices[STEP_UP(i)]; j++)
			{
				if(k == STEP_DOWN(i_VertexDegree))
				{
					cout<<STEP_UP(m_vi_Edges[j])<<" ("<<i_VertexDegree<<") ";
				}
				else
				{
					cout<<STEP_UP(m_vi_Edges[j])<<", ";
				}

				k++;
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Left Vertices = "<<i_LeftVertexCount<<"; Right Vertices = "<<i_RightVertexCount<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
		cout<<endl;

		return;
	}
	
	//Public Function 2261;3261
	void BipartiteGraphInputOutput::PrintVertexDegrees() 
	{
		cout<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Maximum Row Vertex Degree | "<<m_i_MaximumLeftVertexDegree<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Maximum Column Vertex Degree | "<<m_i_MaximumRightVertexDegree<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Maximum Vertex Degree | "<<m_i_MaximumVertexDegree<<endl;
		cout<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Minimum Row Vertex Degree | "<<m_i_MinimumLeftVertexDegree<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Minimum Column Vertex Degree | "<<m_i_MinimumRightVertexDegree<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Minimum Vertex Degree | "<<m_i_MinimumVertexDegree<<endl;
		cout<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Average Row Vertex Degree | "<<m_d_AverageLeftVertexDegree<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Average Column Vertex Degree | "<<m_d_AverageRightVertexDegree<<endl;
		cout<<"Bipartite Graph | "<<m_s_InputFile<<" | Average Vertex Degree | "<<m_d_AverageVertexDegree<<endl;
		cout<<endl;
	    
		return;
	}

	int BipartiteGraphInputOutput::BuildBPGraph_ADOLC(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount) {
		return CompressedSparseRowFormat2BipartiteGraph(uip2_JacobianSparsityPattern, i_RowCount, i_ColumnCount);
	}

	int BipartiteGraphInputOutput::CompressedSparseRowFormat2BipartiteGraph(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount) {
	  //initialize local data
	  int i;
	  unsigned int j;
	  map< int,vector<int> > colList;
	  
	  m_vi_LeftVertices.clear();
	  m_vi_RightVertices.clear();
	  m_vi_Edges.clear();
	  m_vi_LeftVertices.push_back(m_vi_Edges.size());

	  for(i=0; i < i_RowCount; i++) {
		  for(j=1; j <= uip2_JacobianSparsityPattern[i][0]; j++) {
			  m_vi_Edges.push_back(uip2_JacobianSparsityPattern[i][j]);
			  colList[ uip2_JacobianSparsityPattern[i][j] ].push_back(i);
		  }
		  m_vi_LeftVertices.push_back(m_vi_Edges.size());
	  }
	  
	  //put together the right vertices
	  map< int,vector<int> >::iterator curr;
	  m_vi_RightVertices.push_back(m_vi_Edges.size());
	  for(int i=0; i < i_ColumnCount; i++) {
		  curr = colList.find(i);
		  if(curr !=colList.end()) {
			m_vi_Edges.insert(m_vi_Edges.end(),curr->second.begin(),curr->second.end());
		  }//else  We have an empty column
		  m_vi_RightVertices.push_back(m_vi_Edges.size());
	  }

	  CalculateVertexDegrees();
	  //cout<<"PrintBipartiteGraph()"<<endl;
	  //PrintBipartiteGraph();
	  //Pause();
	  return _TRUE;
	}

	int BipartiteGraphInputOutput::BipartiteGraph2CompressedSparseRowFormat(unsigned int *** uip3_JacobianSparsityPattern, unsigned int * uip1_RowCount, unsigned int * uip1_ColumnCount) {
	  //initialize local data
	  unsigned int i = 0;
	  unsigned int j = 0;
	  unsigned int numOfNonZeros = 0;
	  int offset = 0;

	  unsigned int i_RowCount = GetRowVertexCount();
	  (*uip1_RowCount) = i_RowCount;
	  (*uip1_ColumnCount) = GetColumnVertexCount();
	  
	  // Allocate memory and populate (*uip3_JacobianSparsityPattern)
	  (*uip3_JacobianSparsityPattern) = new unsigned int*[GetRowVertexCount()];
	  for(i=0; i < i_RowCount; i++) {
		  numOfNonZeros = m_vi_LeftVertices[i + 1] - m_vi_LeftVertices[i];
		  (*uip3_JacobianSparsityPattern)[i] = new unsigned int[numOfNonZeros + 1]; // Allocate memory
		  (*uip3_JacobianSparsityPattern)[i][0] = numOfNonZeros; // Populate the first entry, which contains the number of Non-Zeros

		  // Populate the entries
		  offset = m_vi_LeftVertices[i] - 1;
		  for(j=1; j <= numOfNonZeros; j++) {
			  (*uip3_JacobianSparsityPattern)[i][j] = m_vi_Edges[offset + j];
		  }
	  }

	  return _TRUE;
	}

	int BipartiteGraphInputOutput::ReadBipartiteGraph(string s_InputFile, string s_fileFormat) {
		if (s_fileFormat == "AUTO_DETECTED" || s_fileFormat == "") {
			File file(s_InputFile);
			string fileExtension = file.GetFileExtension();
			if (isHarwellBoeingFormat(fileExtension)) {
				cout<<"ReadHarwellBoeingBipartiteGraph"<<endl;
				ReadHarwellBoeingBipartiteGraph(s_InputFile);
			}
			else if (isMeTiSFormat(fileExtension)) {
				cout<<"ReadMeTiSBipartiteGraph"<<endl;
				ReadMeTiSBipartiteGraph(s_InputFile);
			}
			else if (fileExtension == "gen") {
				cout<<"ReadGenericMatrixBipartiteGraph"<<endl;
				ReadGenericMatrixBipartiteGraph(s_InputFile);
			}
			else if (fileExtension == "gens") {
				cout<<"ReadGenericSquareMatrixBipartiteGraph"<<endl;
				ReadGenericSquareMatrixBipartiteGraph(s_InputFile);
			}
			else if (isMatrixMarketFormat(fileExtension)) {
				cout<<"ReadMatrixMarketBipartiteGraph"<<endl;
				ReadMatrixMarketBipartiteGraph(s_InputFile);
			}
			else { //other extensions
				cout<<"unfamiliar extension, use ReadMatrixMarketBipartiteGraph"<<endl;
				ReadMatrixMarketBipartiteGraph(s_InputFile);
			}
		}
		else if (s_fileFormat == "MM") {
			cout<<"ReadMatrixMarketBipartiteGraph"<<endl;
			ReadMatrixMarketBipartiteGraph(s_InputFile);
		}
		else if (s_fileFormat == "HB") {
			cout<<"ReadHarwellBoeingBipartiteGraph"<<endl;
			ReadHarwellBoeingBipartiteGraph(s_InputFile);
		}
		else if (s_fileFormat == "MeTiS") {
			cout<<"ReadMeTiSBipartiteGraph"<<endl;
			ReadMeTiSBipartiteGraph(s_InputFile);
		}
		else if (s_fileFormat == "GEN") {
			cout<<"ReadGenericMatrixBipartiteGraph"<<endl;
			ReadGenericMatrixBipartiteGraph(s_InputFile);
		}
		else if (s_fileFormat == "GENS") {
			cout<<"ReadGenericSquareMatrixBipartiteGraph"<<endl;
			ReadGenericSquareMatrixBipartiteGraph(s_InputFile);
		}
		else {
			cerr<<"BipartiteGraphInputOutput::ReadBipartiteGraph s_fileFormat is not recognized"<<endl;
			return _FALSE;
		}

		return(_TRUE);
	}
}
