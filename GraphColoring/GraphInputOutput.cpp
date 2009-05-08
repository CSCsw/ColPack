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
	//Private Function 1201
	int GraphInputOutput::ParseWidth(string FortranFormat)
	{
		int i;

		int LetterCount;

		char PresentLetter;

		string FieldWidth;

		boolean FOUND;

		FieldWidth.clear();

		FOUND = FALSE;

		LetterCount = (signed) FortranFormat.size();

		for(i=0; i<LetterCount; i++)
		{
			PresentLetter = FortranFormat[i];

			if(FOUND == TRUE)
			{
			  FieldWidth += PresentLetter;
			}   

			if(PresentLetter == 'I' || PresentLetter == 'Z' || PresentLetter == 'F' || PresentLetter == 'E' || PresentLetter == 'G' || PresentLetter == 'D' || PresentLetter == 'L' || PresentLetter == 'A')
			{
				FOUND = TRUE;
			}
			else
			if(PresentLetter == '.' || PresentLetter == ')') 
			{
				FOUND = FALSE;

				 break;
			}
		}
	    
		return(atoi(FieldWidth.c_str()));
	}


	//Private Function 1202
	void GraphInputOutput::CalculateVertexDegrees()
	{
		int i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		for(int i = 0; i < i_VertexCount; i++)
		{
			int i_VertexDegree = m_vi_Vertices[i + 1] - m_vi_Vertices[i];

			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;
			}

			if(m_i_MinimumVertexDegree == _UNKNOWN)
			{
				m_i_MinimumVertexDegree = i_VertexDegree;
			}
			else
			if(m_i_MinimumVertexDegree > i_VertexDegree)
			{
				m_i_MinimumVertexDegree = i_VertexDegree;
			}
		}

		m_d_AverageVertexDegree = (double)m_vi_Edges.size()/i_VertexCount;

		return;

	}


	//Public Constructor 1251
	GraphInputOutput::GraphInputOutput()
	{
		Clear();

		GraphCore::Clear();
	}

	
	//Public Destructor 1252
	GraphInputOutput::~GraphInputOutput()
	{
		Clear();
	}

	//Virtual Function 1253
	void GraphInputOutput::Initialize()
	{
 
	}

	//Virtual Function 1254
	void GraphInputOutput::Clear()
	{
		GraphCore::Clear();

		return;
	}
	
	//Public Function 1255
	string GraphInputOutput::GetInputFile()
	{
		return(m_s_InputFile);
	}

/*
	//Public Function 1256
	int GraphInputOutput::ReadMatrixMarketAdjacencyGraph(string s_InputFile)
	{
cout<<"Here!"<<endl;
Pause();
		int i, j;

		int _VALUES;

		int i_LineCount;

		int i_VertexOne, i_VertexTwo;

		int i_VertexCount, i_VertexDegree;

		int i_EdgeCount;

		int i_RowCount, i_ColumnCount;

		double d_NonzeroValue;

		string _GAP(" ");
	    
		string s_InputLine;
	    
		ifstream InputStream;
	    
		vector<string> vs_InputTokens;
	    
		vector< vector<int> > v2i_VertexAdjacency;

		vector< vector<double> > v2d_NonzeroValues;

		Clear();

		m_s_InputFile = s_InputFile;

		InputStream.open(m_s_InputFile.c_str());

		i_EdgeCount = _FALSE;
		i_LineCount = _FALSE;
	    
		_VALUES = _FALSE;

		do
		{
			getline(InputStream, s_InputLine);
			
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

				if(i_RowCount != i_ColumnCount)
				{
					cerr<<endl;
					cerr<<"ERROR 1256 | Matrix Market | Failed Reading Adjacency | Matrix Rectangular"<<endl;
					cerr<<endl;

					return(_FALSE);
				}

				i_VertexCount = i_RowCount;
				i_EdgeCount = atoi(vs_InputTokens[2].c_str());

				v2i_VertexAdjacency.clear();
				v2i_VertexAdjacency.resize((unsigned) i_VertexCount);

				v2d_NonzeroValues.clear();
				v2d_NonzeroValues.resize((unsigned) i_VertexCount);

			}
			else
			if(i_LineCount == STEP_UP(_TRUE))
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
			    
				vs_InputTokens.clear();
			    
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}

				i_VertexOne = atoi(vs_InputTokens[0].c_str());
				i_VertexTwo = atoi(vs_InputTokens[1].c_str());
			    
				if(i_VertexOne == i_VertexTwo)
				{
					v2i_VertexAdjacency[STEP_DOWN(i_VertexOne)].push_back(STEP_DOWN(i_VertexOne));
				
					if(vs_InputTokens.size() > STEP_UP(_TRUE))
					{
						d_NonzeroValue = atof(vs_InputTokens[2].c_str());
					    
						v2d_NonzeroValues[STEP_DOWN(i_VertexOne)].push_back(d_NonzeroValue);
					    
						_VALUES = TRUE;
					}
				}
				else
				{
					v2i_VertexAdjacency[STEP_DOWN(i_VertexOne)].push_back(STEP_DOWN(i_VertexTwo));
					v2i_VertexAdjacency[STEP_DOWN(i_VertexTwo)].push_back(STEP_DOWN(i_VertexOne));
					
					if(vs_InputTokens.size() > STEP_UP(_TRUE))
					{
						d_NonzeroValue = atof(vs_InputTokens[2].c_str());
					    
						v2d_NonzeroValues[STEP_DOWN(i_VertexOne)].push_back(d_NonzeroValue);
						v2d_NonzeroValues[STEP_DOWN(i_VertexTwo)].push_back(d_NonzeroValue);
					    
						_VALUES = TRUE;
					}
				}
			}
			else
			if((i_LineCount > STEP_UP(_TRUE)) && (i_LineCount <= STEP_UP(i_EdgeCount))) 
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
			    
				vs_InputTokens.clear();
			    
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}

				i_VertexOne = atoi(vs_InputTokens[0].c_str());
				i_VertexTwo = atoi(vs_InputTokens[1].c_str());

				if(i_VertexOne == i_VertexTwo)
				{
					v2i_VertexAdjacency[STEP_DOWN(i_VertexOne)].push_back(STEP_DOWN(i_VertexOne));

					if(_VALUES == _TRUE)
					{
						d_NonzeroValue = atof(vs_InputTokens[2].c_str());
					    
						v2d_NonzeroValues[STEP_DOWN(i_VertexOne)].push_back(d_NonzeroValue);
					}
				}
				else
				{
					v2i_VertexAdjacency[STEP_DOWN(i_VertexOne)].push_back(STEP_DOWN(i_VertexTwo));
					v2i_VertexAdjacency[STEP_DOWN(i_VertexTwo)].push_back(STEP_DOWN(i_VertexOne));
					
					if(_VALUES == _TRUE)
					{
						d_NonzeroValue = atof(vs_InputTokens[2].c_str());
					    
						v2d_NonzeroValues[STEP_DOWN(i_VertexOne)].push_back(d_NonzeroValue);
						v2d_NonzeroValues[STEP_DOWN(i_VertexTwo)].push_back(d_NonzeroValue);
					}
				}
			}
			
			i_LineCount++;

		}
		while(InputStream);
	    
		InputStream.close();

		i_VertexCount = (signed) v2i_VertexAdjacency.size();

		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_Vertices.push_back((signed) m_vi_Edges.size());

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;
			}

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_VertexAdjacency[i][j]);

				if(_VALUES == _TRUE)
				{
					m_vd_Values.push_back(v2d_NonzeroValues[i][j]);
				}
			}
		}

		m_vi_Vertices.push_back((signed) m_vi_Edges.size());

		CalculateVertexDegrees();

#if DEBUG == 1256
	    
		if(_VALUES == _FALSE)
		{
			cout<<endl;
			cout<<"DEBUG 1256 | Graph Coloring | Graph Structure | "<<m_s_InputFile<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"DEBUG 1256 | Graph Coloring | Graph Structure With Nonzero Values | "<<m_s_InputFile<<endl;
			cout<<endl;
		}

		i_VertexCount = (signed) m_vi_Vertices.size();
		i_EdgeCount = (signed) m_vi_Edges.size();

		cout<<"Vertices"<<"\t"<<" : ";

		for(i=0; i<i_VertexCount; i++)
		{
			if(i == STEP_DOWN(i_VertexCount))
			{
				cout<<STEP_UP(m_vi_Vertices[i])<<" ("<<i_VertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_Vertices[i])<<", ";
			}
		}

		cout<<"Edges"<<"\t\t"<<" : ";

		for(i=0; i<i_EdgeCount; i++)
		{
			if(i == STEP_DOWN(i_EdgeCount))
			{
				cout<<STEP_UP(m_vi_Edges[i])<<" ("<<i_EdgeCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_Edges[i])<<", ";
			}
		}

		if(_VALUES == _TRUE)
		{
			cout<<"Nonzeros"<<"\t"<<" : ";
			
			for(i=0; i<i_EdgeCount; i++)
			{
				if(i == STEP_DOWN(i_EdgeCount))
				{
					cout<<m_vd_Values[i]<<" ("<<i_EdgeCount<<")"<<endl;
				}
				else
				{
					cout<<m_vd_Values[i]<<", ";
				}
			}
		}

		if(_VALUES == _FALSE)
		{
			cout<<endl;
			cout<<"[Vertices = "<<STEP_DOWN(i_VertexCount)<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"[Vertices = "<<STEP_DOWN(i_VertexCount)<<"; Edges = "<<i_EdgeCount/2<<"; Nonzeros = "<<i_EdgeCount/2<<"]"<<endl;
			cout<<endl;
		}

#endif

cout<<"Here Finish!"<<endl;
Pause();
		return(_TRUE);
	}
	//*/

	
	//Public Function 1257
	int GraphInputOutput::ReadMatrixMarketAdjacencyGraph(string s_InputFile, bool b_getStructureOnly)
	{
		//Pause();
		Clear();

		m_s_InputFile=s_InputFile;
		ifstream in (m_s_InputFile.c_str());
		if(!in) {
			cout<<m_s_InputFile<<" not Found!"<<endl;
			return (_FALSE);
		}
		else cout<<"Found file "<<m_s_InputFile<<endl;
		
		//initialize local data
		int rowCounter=0, col=0, row=0, nonzeros=0, rowIndex=0, colIndex=0;
		//int num=0, numCount=0;
		float value;
		bool b_getValue = !b_getStructureOnly;
		istringstream in2;
		string line="";
		map<int,vector<int> > nodeList;
		map<int,vector<float> > valueList;

		getline(in,line);
		rowCounter++;
		while(line.size()>0&&line[0]=='%') {//ignore comment line
			getline(in,line);
		}
		in2.str(line);
		in2>>row>>col>>nonzeros;
		//cout<<"row="<<row<<"; col="<<col<<"; nonzeros="<<nonzeros<<endl;
		if(row!=col) {
			cout<<"* WARNING: GraphInputOutput::ReadMatrixMarketAdjacencyGraph()"<<endl;
			cout<<"*\t row!=col. This is not a square matrix. Can't process."<<endl;
			return _FALSE;
		}

		while(!in.eof() && rowCounter<nonzeros+1) //there should be (nonzeros+1) lines in the input file
		{
			getline(in,line);
			rowCounter++;
			//cout<<"Line "<<rowCounter<<"="<<line<<endl;
			if(line!="")
			{
				in2.clear();
				in2.str(line);
				in2>>rowIndex>>colIndex; 
				if(rowIndex!=colIndex) {
					rowIndex--;
					colIndex--;
					//cout<<"\t"<<setw(4)<<rowIndex<<setw(4)<<colIndex<<endl;
					nodeList[rowIndex].push_back(colIndex);
					nodeList[colIndex].push_back(rowIndex);

					if(b_getValue) {
						in2>>value;
						//cout<<"Value = "<<value<<endl;
						valueList[rowIndex].push_back(value);
						valueList[colIndex].push_back(value);
					}
				}
				else continue;
			}
			else
			{
				cerr<<"* WARNING: GraphInputOutput::ReadMatrixMarketAdjacencyGraph()"<<endl;
				cerr<<"*\t line == \"\" at row "<<rowCounter<<". Empty line. Wrong input format. Can't process."<<endl;
				return _FALSE;
			}
		}
		if(rowCounter<nonzeros+1) { //rowCounter should be == nonzeros+1
				cerr<<"* WARNING: GraphInputOutput::ReadMatrixMarketAdjacencyGraph()"<<endl;
				cerr<<"*\t rowCounter<nonzeros+1. Wrong input format. Can't process."<<endl;
				return _FALSE;
		}


		//now construct the graph
		m_vi_Vertices.push_back(m_vi_Edges.size()); //m_vi_Edges.size() == 0 at this point

		for(int i=0;i<row; i++) {
			m_vi_Edges.insert(m_vi_Edges.end(),nodeList[i].begin(),nodeList[i].end());
			m_vi_Vertices.push_back(m_vi_Edges.size());
		}
		if(b_getValue) {
			for(int i=0;i<row; i++) {
				m_vd_Values.insert(m_vd_Values.end(),valueList[i].begin(),valueList[i].end());
			}
		}

//PrintGraph();
//cout<<"DONE PrintGraph();"<<endl;

			/*
		//this way of insertion is really BAD!!!! because the for loop will skip the nodeList[?] that is empty
		map<int,vector<int> >::iterator curr=nodeList.begin();
		m_vi_Vertices.push_back(m_vi_Edges.size());
		for(;curr!=nodeList.end();curr++)
		{
			m_vi_Edges.insert(m_vi_Edges.end(),curr->second.begin(),curr->second.end());
			m_vi_Vertices.push_back(m_vi_Edges.size());
	cout<<endl<<"Vertices vector:";
	for(int i=0;i<m_vi_Vertices.size();i++) {
		if (i%5==0) cout<<endl<<"\t";
		cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Vertices[i]<<"; ";
	}
	cout<<endl<<"Edges vector:";
	for(int i=0;i<m_vi_Edges.size();i++) {
		if (i%5==0) cout<<endl<<"\t";
		cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Edges[i]<<"; ";
	}
	cout<<endl<<endl;
	Pause();
		}
		if(b_getValue) {
			map<int,vector<float> >::iterator curr2=valueList.begin();
			for(;curr2!=valueList.end();curr2++) {
				/*
				cout<<endl<<"Values vector for :"<<curr2->first;
				for(int i=0;i<curr2->second.size();i++) {
					if (i%5==0) cout<<endl<<"\t";
					cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<curr2->second[i]<<"; ";
				}
				//*-/
				m_vd_Values.insert(m_vd_Values.end(),curr2->second.begin(),curr2->second.end());
			}
		}
		//*/

		/*
		cout<<"--------------------------------------------------------"<<endl;
		cout<<"nonzeros+1="<<nonzeros+1<<endl;
		cout<<"rowCounter="<<rowCounter<<endl;
		cout<<endl<<"Vertices vector:";
		for(int i=0;i<m_vi_Vertices.size();i++) {
			if (i%5==0) cout<<endl<<"\t";
			cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Vertices[i]<<"; ";
		}
		cout<<endl<<"Edges vector:";
		for(int i=0;i<m_vi_Edges.size();i++) {
			if (i%5==0) cout<<endl<<"\t";
			cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Edges[i]<<"; ";
		}
		if(b_getValue){
			cout<<endl<<"Values vector:";
			for(int i=0;i<m_vd_Values.size();i++) {
				if (i%5==0) cout<<endl<<"\t";
				cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vd_Values[i]<<"; ";
			}
		}
		cout<<endl<<"--------------------------------------------------------"<<endl;
		Pause();
		//*/
		CalculateVertexDegrees();

		return(_TRUE);
	}

	
	//Public Function 1257
	int GraphInputOutput::ReadMatrixMarketAdjacencyGraph2(string s_InputFile, bool b_getStructureOnly)
	{
		//Pause();
		Clear();

		m_s_InputFile=s_InputFile;
		ifstream in (m_s_InputFile.c_str());
		if(!in) cout<<m_s_InputFile<<" not Found!"<<endl;
		else cout<<"Found file "<<m_s_InputFile<<endl;
		
		//initialize local data
		int rowCounter=0, col=0, row=0, nonzeros=0, rowIndex=0, colIndex=0;
		//int num=0, numCount=0;
		float value;
		bool b_getValue = !b_getStructureOnly;
		istringstream in2;
		string line="";
		map<int,vector<int> > nodeList;
		map<int,vector<float> > valueList;

		getline(in,line);
		rowCounter++;
		while(line.size()>0&&line[0]=='%') {//ignore comment line
			getline(in,line);
		}
		in2.str(line);
		in2>>row>>col>>nonzeros;
		//cout<<"row="<<row<<"; col="<<col<<"; nonzeros="<<nonzeros<<endl;
		if(row!=col) {
			cout<<"* WARNING: GraphInputOutput::ReadMatrixMarketAdjacencyGraph2()"<<endl;
			cout<<"*\t row!=col. This is not a square matrix. Can't process."<<endl;
			return _FALSE;
		}

		while(!in.eof() && rowCounter<nonzeros+1) //there should be (nonzeros+1) lines in the input file
		{
			getline(in,line);
			rowCounter++;
			//cout<<"Line "<<rowCounter<<"="<<line<<endl;
			if(line!="")
			{
				in2.clear();
				in2.str(line);
				in2>>rowIndex>>colIndex; 
				if(rowIndex>colIndex) {
					rowIndex--;
					colIndex--;
//cout<<"\t"<<setw(4)<<rowIndex<<setw(4)<<colIndex<<endl;
					nodeList[rowIndex].push_back(colIndex);
					nodeList[colIndex].push_back(rowIndex);

					if(b_getValue) {
						in2>>value;
//cout<<"Value = "<<value<<endl;
						valueList[rowIndex].push_back(value);
						valueList[colIndex].push_back(value);
					}
				}
				else continue;
			}
			else
			{
				cerr<<"* WARNING: GraphInputOutput::ReadMatrixMarketAdjacencyGraph2()"<<endl;
				cerr<<"*\t line == \"\" at row "<<rowCounter<<". Empty line. Wrong input format. Can't process."<<endl;
				return _FALSE;
			}
		}
		if(rowCounter<nonzeros+1) { //rowCounter should be == nonzeros+1
				cerr<<"* WARNING: GraphInputOutput::ReadMatrixMarketAdjacencyGraph2()"<<endl;
				cerr<<"*\t rowCounter<nonzeros+1. Wrong input format. Can't process."<<endl;
				return _FALSE;
		}


		//now construct the graph
		m_vi_Vertices.push_back(m_vi_Edges.size()); //m_vi_Edges.size() == 0 at this point

		for(int i=0;i<row; i++) {
			m_vi_Edges.insert(m_vi_Edges.end(),nodeList[i].begin(),nodeList[i].end());
			m_vi_Vertices.push_back(m_vi_Edges.size());
		}
		if(b_getValue) {
			for(int i=0;i<row; i++) {
				m_vd_Values.insert(m_vd_Values.end(),valueList[i].begin(),valueList[i].end());
			}
		}

//PrintGraph();
//cout<<"DONE PrintGraph();"<<endl;

			/*
		//this way of insertion is really BAD!!!! because the for loop will skip the nodeList[?] that is empty
		map<int,vector<int> >::iterator curr=nodeList.begin();
		m_vi_Vertices.push_back(m_vi_Edges.size());
		for(;curr!=nodeList.end();curr++)
		{
			m_vi_Edges.insert(m_vi_Edges.end(),curr->second.begin(),curr->second.end());
			m_vi_Vertices.push_back(m_vi_Edges.size());
	cout<<endl<<"Vertices vector:";
	for(int i=0;i<m_vi_Vertices.size();i++) {
		if (i%5==0) cout<<endl<<"\t";
		cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Vertices[i]<<"; ";
	}
	cout<<endl<<"Edges vector:";
	for(int i=0;i<m_vi_Edges.size();i++) {
		if (i%5==0) cout<<endl<<"\t";
		cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Edges[i]<<"; ";
	}
	cout<<endl<<endl;
	Pause();
		}
		if(b_getValue) {
			map<int,vector<float> >::iterator curr2=valueList.begin();
			for(;curr2!=valueList.end();curr2++) {
				/*
				cout<<endl<<"Values vector for :"<<curr2->first;
				for(int i=0;i<curr2->second.size();i++) {
					if (i%5==0) cout<<endl<<"\t";
					cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<curr2->second[i]<<"; ";
				}
				//*-/
				m_vd_Values.insert(m_vd_Values.end(),curr2->second.begin(),curr2->second.end());
			}
		}
		//*/

		/*
		cout<<"--------------------------------------------------------"<<endl;
		cout<<"nonzeros+1="<<nonzeros+1<<endl;
		cout<<"rowCounter="<<rowCounter<<endl;
		cout<<endl<<"Vertices vector:";
		for(int i=0;i<m_vi_Vertices.size();i++) {
			if (i%5==0) cout<<endl<<"\t";
			cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Vertices[i]<<"; ";
		}
		cout<<endl<<"Edges vector:";
		for(int i=0;i<m_vi_Edges.size();i++) {
			if (i%5==0) cout<<endl<<"\t";
			cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vi_Edges[i]<<"; ";
		}
		if(b_getValue){
			cout<<endl<<"Values vector:";
			for(int i=0;i<m_vd_Values.size();i++) {
				if (i%5==0) cout<<endl<<"\t";
				cout<<"["<<setw(3)<<i<<"] "<<setw(5)<<m_vd_Values[i]<<"; ";
			}
		}
		cout<<endl<<"--------------------------------------------------------"<<endl;
		Pause();
		//*/
		CalculateVertexDegrees();

		return(_TRUE);
	}

	int GraphInputOutput::ReadMatrixMarketAdjacencyGraphStructure(string s_InputFile) {
		return ReadMatrixMarketAdjacencyGraph(s_InputFile, true);
	}

	int GraphInputOutput::ReadMatrixMarketAdjacencyGraphStructure2(string s_InputFile) {
		return ReadMatrixMarketAdjacencyGraph2(s_InputFile, true);
	}

	/* Arijit's function
	int GraphInputOutput::ReadMatrixMarketAdjacencyGraphStructure(string s_InputFile)
	{
		int i, j;

		int i_LineCount;
	    
		int i_RowCount, i_ColumnCount;

		int i_VertexOne, i_VertexTwo;

		int i_VertexCount, i_VertexDegree;

		int i_EdgeCount;
	    
		string _GAP(" ");
	    
		string s_InputLine;
	    
		ifstream InputStream;
	    
		vector<string> vs_InputTokens;
	    
		vector< vector<int> > v2i_VertexAdjacency;

		Clear();

		m_s_InputFile = s_InputFile;

		InputStream.open(m_s_InputFile.c_str());

		i_EdgeCount = _FALSE;
		i_LineCount = _FALSE;
	    
		do
		{
			getline(InputStream, s_InputLine);
			
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

				if(i_RowCount != i_ColumnCount)
				{
					cerr<<endl;
					cerr<<"ERROR 1257 | Matrix Market | Failed Reading Adjacency | Matrix Rectangular"<<endl;
					cerr<<endl;

					return(_FALSE);
				}

				i_VertexCount = i_RowCount;
				i_EdgeCount = atoi(vs_InputTokens[2].c_str());

				v2i_VertexAdjacency.clear();
				v2i_VertexAdjacency.resize((unsigned) i_VertexCount);
			    
			}
		
			if((i_LineCount > _TRUE) && (i_LineCount <= STEP_UP(i_EdgeCount))) 
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
			    
				vs_InputTokens.clear();
			    
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}

				i_VertexOne = atoi(vs_InputTokens[0].c_str());
				i_VertexTwo = atoi(vs_InputTokens[1].c_str());

				if(i_VertexOne != i_VertexTwo)
				{
					v2i_VertexAdjacency[STEP_DOWN(i_VertexOne)].push_back(STEP_DOWN(i_VertexTwo));
					v2i_VertexAdjacency[STEP_DOWN(i_VertexTwo)].push_back(STEP_DOWN(i_VertexOne));
				}
			}
		
			i_LineCount++;

		}
		while(InputStream);
	    
		InputStream.close();
	    
		i_VertexCount = (signed) v2i_VertexAdjacency.size();

		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_Vertices.push_back((signed) m_vi_Edges.size());

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_VertexAdjacency[i][j]);
			}
		}

		m_vi_Vertices.push_back((signed) m_vi_Edges.size());

		CalculateVertexDegrees();

#if DEBUG == 1257
	    
		cout<<endl;
		cout<<"DEBUG 1257 | Graph Coloring | Vertex Adjacency | "<<m_s_InputFile<<endl;
		cout<<endl;

		i_EdgeCount = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			cout<<"Vertex "<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				if(j == STEP_DOWN(i_VertexDegree))
				{
					cout<<STEP_UP(v2i_VertexAdjacency[i][j])<<" ("<<i_VertexDegree<<")";

					i_EdgeCount++;
				}
				else
				{
					cout<<STEP_UP(v2i_VertexAdjacency[i][j])<<", ";

					i_EdgeCount++;
				}
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Vertices = "<<i_VertexCount<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}
	//*/

	
	//Public Function 1258
	int GraphInputOutput::ReadHarwellBoeingAdjacencyGraph(string s_InputFile)
	{
		int i, j;

		int i_VertexCount, i_VertexDegree;

		int LineCount;

		int AssemblyCount, ColumnCount, DataCount, RowCount;

		int ColumnFormatWidth, RowFormatWidth, DataFormatWidth;

		int ColumnStartLine, DataStartLine, RowStartLine;

		int IndexCount;

		int RightCount, RightIndexCount;

		int RightHeader;

		int TotalColumnLines, TotalDataLines, TotalEntryLines, TotalRowLines;
	  
		int TotalRightDataLines;

		string InputLine;

		string MatrixTitle, MatrixKey, MatrixType;

		string ColumnFormat, RowFormat, DataFormat;

		string RightFormat;

		string RightType;

		string __GAP(" ");

		ifstream InputStream;

		vector<int> ColumnIndices, RowIndices;

		vector<double> DataValues;

		vector<string> InputTokens;

		vector< vector<int> > v2i_VertexAdjacency;

		Clear();

		ColumnIndices.clear();
		RowIndices.clear();
		DataValues.clear();

		m_s_InputFile = s_InputFile;

		InputStream.open(m_s_InputFile.c_str());
		if(!InputStream) {
			cout<<m_s_InputFile<<" not Found!"<<endl;
			return (_FALSE);
		}
		else cout<<"Found file "<<m_s_InputFile<<endl;

		TotalColumnLines = TotalRowLines = TotalDataLines = _FALSE;

		ColumnFormatWidth = RowFormatWidth = DataFormatWidth = _FALSE;

		TotalRightDataLines = _FALSE;
	  
		RightHeader = _FALSE;
	  
		LineCount = _FALSE;

		do
		{
			InputLine.clear();

			getline(InputStream, InputLine);

			if(!InputStream)
			{
				break;
			}
	  
			if(LineCount == 0)
			{
				MatrixTitle = InputLine.substr(0, 72);
	      
				MatrixKey = InputLine.substr(72, 8);
			}

			if(LineCount == 1)
			{
				TotalEntryLines = atoi(InputLine.substr(0, 403).c_str());
				TotalColumnLines = atoi(InputLine.substr(403, 403).c_str());
				TotalRowLines = atoi(InputLine.substr(28, 403).c_str());
				TotalDataLines = atoi(InputLine.substr(42, 403).c_str());
				TotalRightDataLines = atoi(InputLine.substr(56, 403).c_str());
			}

			if(LineCount == 2)
			{
				MatrixType = InputLine.substr(0, 3);

				RowCount = atoi(InputLine.substr(403, 403).c_str());
				ColumnCount = atoi(InputLine.substr(28, 403).c_str());
				DataCount = atoi(InputLine.substr(42, 403).c_str());
				AssemblyCount = atoi(InputLine.substr(56, 403).c_str());
			}

			if(LineCount == 3)
			{
				ColumnFormat = InputLine.substr(0, 16);
				RowFormat = InputLine.substr(16, 16);
				DataFormat = InputLine.substr(32, 20);
				RightFormat= InputLine.substr(52, 20);

				ColumnFormatWidth = ParseWidth(ColumnFormat);
				RowFormatWidth = ParseWidth(RowFormat);
				DataFormatWidth = ParseWidth(DataFormat);
			}    

			if(LineCount == 4)
			{
				if(TotalRightDataLines != _FALSE)
				{
					RightType = InputLine.substr(0, 3);
					RightCount = atoi(InputLine.substr(403, 403).c_str());
					RightIndexCount = atoi(InputLine.substr(28, 403).c_str());

					RightHeader = _TRUE;
				}
			}
	    
			ColumnStartLine = RightHeader + 4;

			if(LineCount >= ColumnStartLine  && LineCount < ColumnStartLine + TotalColumnLines)
			{
				IndexCount = _FALSE;

				while(IndexCount <(signed) InputLine.size())
				{
					ColumnIndices.push_back(atoi(InputLine.substr(IndexCount, ColumnFormatWidth).c_str()));

					IndexCount += ColumnFormatWidth;
				}
			}

			RowStartLine = ColumnStartLine + TotalColumnLines;

			if(LineCount >= RowStartLine && LineCount < RowStartLine + TotalRowLines)
			{
				IndexCount = _FALSE;

				while(IndexCount <(signed) InputLine.size())
				{
					RowIndices.push_back(atoi(InputLine.substr(IndexCount, RowFormatWidth).c_str()));

					IndexCount += RowFormatWidth;
				}
			}

			DataStartLine = RowStartLine + TotalRowLines;

			if(LineCount >= DataStartLine && LineCount < DataStartLine + TotalDataLines)
			{
				IndexCount = _FALSE;

				while(IndexCount <(signed) InputLine.size())
				{
					DataValues.push_back(atof(InputLine.substr(IndexCount, DataFormatWidth).c_str()));

					IndexCount += DataFormatWidth;
				}
			}

			LineCount++;

		}
		while(InputStream);

		InputStream.close();

		IndexCount = (signed) ColumnIndices.size();
	  
		v2i_VertexAdjacency.clear();
		v2i_VertexAdjacency.resize((unsigned) STEP_DOWN(IndexCount));

		for(i=0; i<STEP_DOWN(IndexCount); i++)
		{
			for(j=STEP_DOWN(ColumnIndices[i]); j<STEP_DOWN(ColumnIndices[STEP_UP(i)]); j++)
			{
				if(i == STEP_DOWN(RowIndices[j]))
				{  
					continue;
				}

				v2i_VertexAdjacency[i].push_back(STEP_DOWN(RowIndices[j]));
				v2i_VertexAdjacency[STEP_DOWN(RowIndices[j])].push_back(i);
			}
		}

		i_VertexCount = (signed) v2i_VertexAdjacency.size();
	  
		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_Vertices.push_back((signed) m_vi_Edges.size());
	      
			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();
	      
			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_VertexAdjacency[i][j]);
			}
		}
	  
		m_vi_Vertices.push_back((signed) m_vi_Edges.size());

		CalculateVertexDegrees();

#if DEBUG == 1258

		cout<<endl;
	  
		cout<<"Matrix Title = "<<MatrixTitle<<endl;
		cout<<"Matrix Key = "<<MatrixKey<<endl;
	  
		cout<<endl;
	  
		cout<<"Total Entry Lines = "<<TotalEntryLines<<endl;
		cout<<"Total Column Lines = "<<TotalColumnLines<<endl;
		cout<<"Total Row Lines = "<<TotalRowLines<<endl;
		cout<<"Total Data Lines = "<<TotalDataLines<<endl;
		cout<<"Total Right Data Lines = "<<TotalRightDataLines<<endl;
	  
		cout<<endl;
	  
		cout<<"Matrix Type = "<<MatrixType<<endl;
		cout<<"Row Count = "<<RowCount<<endl;
		cout<<"Column Count = "<<ColumnCount<<endl;
		cout<<"Data Count = "<<DataCount<<endl;
		cout<<"Assembly Count = "<<AssemblyCount<<endl;
	    
		cout<<endl;
	      
		cout<<"Column Format = "<<ColumnFormat<<endl;
		cout<<"Row Format = "<<RowFormat<<endl;
		cout<<"Data Format = "<<DataFormat<<endl;
		cout<<"Right Format = "<<RightFormat<<endl;

		cout<<endl;

		cout<<"Column Format Width = "<<ColumnFormatWidth<<endl;
		cout<<"Row Format Width = "<<RowFormatWidth<<endl;
		cout<<"Data Format Width = "<<DataFormatWidth<<endl;

		cout<<endl;
	    
		if(TotalRightDataLines != _FALSE)
		{
			cout<<"Right Type = "<<RightType<<endl;
			cout<<"Right Count = "<<RightCount<<endl;
			cout<<"Right Index Count = "<<RightIndexCount<<endl;
	  
			cout<<endl;
		}

		IndexCount = (signed) ColumnIndices.size();

		cout<<"Column Indices = ";

		for(i=0; i<IndexCount; i++)
		{
			if(i == STEP_DOWN(IndexCount))
			{
				cout<<ColumnIndices[i]<<" ("<<IndexCount<<")"<<endl;
			}
			else
			{
				cout<<ColumnIndices[i]<<", ";
			}
		}
	    
		if(!IndexCount)
		{
			cout<<"Not Found"<<endl;
		}

		cout<<endl;

		IndexCount = (signed) RowIndices.size();

		cout<<"Row Indices = ";

		for(i=0; i<IndexCount; i++)
		{
			if(i == STEP_DOWN(IndexCount))
			{
				cout<<RowIndices[i]<<" ("<<IndexCount<<")"<<endl;
			}
			else
			{
				cout<<RowIndices[i]<<", ";
			}
		}
	    
		if(!IndexCount)
		{
			cout<<"Not Found"<<endl;
		}

		cout<<endl;

		IndexCount = (signed) DataValues.size();

		cout<<"Data Values = ";

		for(i=0; i<IndexCount; i++)
		{
			if(i == STEP_DOWN(IndexCount))
			{
				cout<<DataValues[i]<<" ("<<IndexCount<<")"<<endl;
			}
			else
			{
				cout<<DataValues[i]<<", ";
			}
		}
	    
		if(!IndexCount)
		{
			cout<<"Not Found"<<endl;
		}

		cout<<endl;
	    
#endif

#if DEBUG == 1258
	    
		int i_EdgeCount;

		cout<<endl;
		cout<<"DEBUG 1258 | Acyclic Coloring | Vertex Adjacency | "<<InputFile<<endl;
		cout<<endl;

		i_EdgeCount = _FALSE;
		i_VertexCount = RowCount;

		for(i=0; i<i_VertexCount; i++)
		{
			cout<<"Vertex "<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				if(j == STEP_DOWN(i_VertexDegree))
				{
				cout<<STEP_UP(v2i_VertexAdjacency[i][j])<<" ("<<i_VertexDegree<<")";

				i_EdgeCount++;
				}
				else
				{
					cout<<STEP_UP(v2i_VertexAdjacency[i][j])<<", ";

					i_EdgeCount++;
				}
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Vertices = "<<i_VertexCount<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	
	//Public Function 1259
	int GraphInputOutput::ReadMeTiSAdjacencyGraph(string s_InputFile)
	{
		int i, j;

		int i_LineCount, i_TokenCount;
	    
		int i_Vertex;

		int i_VertexCount, i_VertexDegree;

		int i_EdgeCount;
	    
		int i_VertexWeights, i_EdgeWeights;

		string _GAP(" ");
	    
		string s_InputLine;
	    
		ifstream InputStream;
	    
		vector<string> vs_InputTokens;

		vector<double> vi_EdgeWeights;
		vector<double> vi_VertexWeights;

		vector< vector<int> > v2i_VertexAdjacency;

		vector< vector<double> > v2i_VertexWeights;

		Clear();
	 
		m_s_InputFile = s_InputFile;

		InputStream.open(m_s_InputFile.c_str());
		if(!InputStream) {
			cout<<m_s_InputFile<<" not Found!"<<endl;
			return (_FALSE);
		}
		else cout<<"Found file "<<m_s_InputFile<<endl;

		vi_EdgeWeights.clear();

		v2i_VertexWeights.clear();
	      
		i_VertexWeights = i_EdgeWeights = _FALSE;

		i_LineCount = _FALSE;
	    
		do
		{
			getline(InputStream, s_InputLine);
			
			if(!InputStream)
			{
				break;
			}

			if(s_InputLine[0] == '%')
			{
				continue;
			}

			if(i_LineCount == _FALSE)
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
			    
				vs_InputTokens.clear();
			    
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}
			    
				i_VertexCount = atoi(vs_InputTokens[0].c_str());
				i_EdgeCount = atoi(vs_InputTokens[1].c_str());
			    
				i_VertexWeights = _FALSE;
				i_EdgeWeights = _FALSE;

				if(vs_InputTokens.size() > 2)
				{
					if(atoi(vs_InputTokens[2].c_str()) == 1)
					{
						i_EdgeWeights = _TRUE;
					}
					else
					if(atoi(vs_InputTokens[2].c_str()) == 10)
					{
						i_VertexWeights = _TRUE;
					}
					else
					if(atoi(vs_InputTokens[2].c_str()) == 11)
					{
						i_EdgeWeights = _TRUE;
						i_VertexWeights = _TRUE;
					}
			   }

				if(vs_InputTokens.size() > 3)
				{
					i_VertexWeights = atoi(vs_InputTokens[3].c_str());
				}

				v2i_VertexAdjacency.clear();
				v2i_VertexAdjacency.resize((unsigned) i_VertexCount);
		    
				i_LineCount++;

			}
			else
			{
				StringTokenizer GapTokenizer(s_InputLine, _GAP);
		    
				vs_InputTokens.clear();
	   
				while(GapTokenizer.HasMoreTokens())
				{
					vs_InputTokens.push_back(GapTokenizer.GetNextToken());
				}

				i_TokenCount = (signed) vs_InputTokens.size();

				vi_VertexWeights.clear();

				for(i=0; i<i_VertexWeights; i++)
				{
					vi_VertexWeights.push_back(atoi(vs_InputTokens[i].c_str()));
				}

				if(i_VertexWeights != _FALSE)
				{
					v2i_VertexWeights.push_back(vi_VertexWeights);
				}

				if(i_EdgeWeights == _FALSE)
				{
					for(i=i_VertexWeights; i<i_TokenCount; i++)
					{
						i_Vertex = STEP_DOWN(atoi(vs_InputTokens[i].c_str()));

						if(i_Vertex != STEP_DOWN(i_LineCount))
						{
							v2i_VertexAdjacency[STEP_DOWN(i_LineCount)].push_back(i_Vertex);
						}							  
					}
				}
				else
				{
					for(i=i_VertexWeights; i<i_TokenCount; i=i+2)
					{
						i_Vertex = STEP_DOWN(atoi(vs_InputTokens[i].c_str()));

						if(i_Vertex != STEP_DOWN(i_LineCount))
						{
							v2i_VertexAdjacency[STEP_DOWN(i_LineCount)].push_back(i_Vertex);

							vi_EdgeWeights.push_back(STEP_DOWN(atof(vs_InputTokens[STEP_UP(i)].c_str())));
						}					  
					}
				}

				i_LineCount++;
			}

		}
		while(InputStream);
	    
		InputStream.close();

		i_VertexCount = (signed) v2i_VertexAdjacency.size();

		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_Vertices.push_back((signed) m_vi_Edges.size());

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_VertexAdjacency[i][j]);
			}
		}

		m_vi_Vertices.push_back((signed) m_vi_Edges.size());

		CalculateVertexDegrees();

#if DEBUG == 1259
	 
		cout<<endl;
		cout<<"DEBUG 1259 | Graph Coloring | Vertex Adjacency | "<<m_s_InputFile<<endl;
		cout<<endl;

		i_EdgeCount = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			cout<<"Vertex "<<STEP_UP(i)<<"\t"<<" : ";

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				if(j == STEP_DOWN(i_VertexDegree))
				{
					cout<<STEP_UP(v2i_VertexAdjacency[i][j])<<" ("<<i_VertexDegree<<")";

					i_EdgeCount++;
				}
				else
				{
					cout<<STEP_UP(v2i_VertexAdjacency[i][j])<<", ";

					i_EdgeCount++;
				}
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Vertices = "<<i_VertexCount<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);

	}

	
	//Public Function 1260
	int GraphInputOutput::PrintGraph()
	{
		int i;

		int i_VertexCount, i_EdgeCount;

		i_VertexCount = (signed) m_vi_Vertices.size();

		cout<<endl;
		cout<<"Graph Coloring | Vertex List | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_VertexCount; i++)
		{
			if(i == STEP_DOWN(i_VertexCount))
			{
				cout<<STEP_UP(m_vi_Vertices[i])<<" ("<<i_VertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_Vertices[i])<<", ";
			}
		}

		i_EdgeCount = (signed) m_vi_Edges.size();

		cout<<endl;
		cout<<"Graph Coloring | Edge List | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			if(i == STEP_DOWN(i_EdgeCount))
			{
				cout<<STEP_UP(m_vi_Edges[i])<<" ("<<i_EdgeCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_Edges[i])<<", ";
			}
		}

		if(m_vd_Values.size() > _FALSE)
		{

			cout<<endl;
			cout<<"Graph Coloring | Nonzero List | "<<m_s_InputFile<<endl;
			cout<<endl;

			for(i=0; i<i_EdgeCount; i++)
			{
				if(i == STEP_DOWN(i_EdgeCount))
				{
					cout<<m_vd_Values[i]<<" ("<<i_EdgeCount<<")"<<endl;
				}
				else
				{
					cout<<m_vd_Values[i]<<", ";
				}
			}

			cout<<endl;
			cout<<"[Vertices = "<<STEP_DOWN(i_VertexCount)<<"; Edges = "<<i_EdgeCount/2<<"; Nonzeros = "<<i_EdgeCount/2<<"]"<<endl;
			cout<<endl;	
		}
		else
		{
			cout<<endl;
			cout<<"[Vertices = "<<STEP_DOWN(i_VertexCount)<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
			cout<<endl;	
		
		}

		return(_TRUE);
	}

	
	//Public Function 1261
	int GraphInputOutput::PrintGraphStructure()
	{
		int i;

		int i_VertexCount, i_EdgeCount;

		i_VertexCount = (signed) m_vi_Vertices.size();

		cout<<endl;
		cout<<"Graph Coloring | Vertex List | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_VertexCount; i++)
		{
			if(i == STEP_DOWN(i_VertexCount))
			{
				cout<<STEP_UP(m_vi_Vertices[i])<<" ("<<i_VertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_Vertices[i])<<", ";
			}
		}

		i_EdgeCount = (signed) m_vi_Edges.size();

		cout<<endl;
		cout<<"Graph Coloring | Edge List | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			if(i == STEP_DOWN(i_EdgeCount))
			{
				cout<<STEP_UP(m_vi_Edges[i])<<" ("<<i_EdgeCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_Edges[i])<<", ";
			}
		}

		cout<<endl;
		cout<<"[Vertices = "<<STEP_DOWN(i_VertexCount)<<"; Edges = "<<i_EdgeCount/2<<"]"<<endl;
		cout<<endl;

		return(_TRUE);
	}

	
	//Public Function 1262
	int GraphInputOutput::PrintMatrix()
	{
		int i, j;

		int i_VertexCount;

		cout<<endl;
		cout<<"Graph Coloring | Matrix Elements | "<<m_s_InputFile<<endl;
		cout<<endl;

		i_VertexCount = (signed) m_vi_Vertices.size();

		for(i=0; i<i_VertexCount-1; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				cout<<"Element["<<STEP_UP(i)<<"]["<<STEP_UP(m_vi_Edges[j])<<"] = "<<m_vd_Values[j]<<endl;
			}
		}

		cout<<endl;

		return(_TRUE);
	}

	
	//Public Function 1263
	int GraphInputOutput::PrintMatrix(vector<int> & vi_Vertices, vector<int> & vi_Edges, vector<double> & vd_Values)
	{
		int i, j;

		int i_VertexCount;

		cout<<endl;
		cout<<"Graph Coloring | Matrix Elements | "<<m_s_InputFile<<endl;
		cout<<endl;
		i_VertexCount = (signed) vi_Vertices.size();

		for(i=0; i<i_VertexCount-1; i++)
		{
			for(j=vi_Vertices[i]; j<vi_Vertices[STEP_UP(i)]; j++)
			{
				cout<<"Element["<<STEP_UP(i)<<"]["<<STEP_UP(vi_Edges[j])<<"] = "<<vd_Values[j]<<endl;
			}
		}

		cout<<endl;

		return(_TRUE);
	}

	
	//Public Function 1264
	void GraphInputOutput::PrintVertexDegrees()
	{
		cout<<endl;
		cout<<"Graph | "<<m_s_InputFile<<" | Maximum Vertex Degree | "<<m_i_MaximumVertexDegree<<endl;
		cout<<"Graph | "<<m_s_InputFile<<" | Minimum Vertex Degree | "<<m_i_MinimumVertexDegree<<endl;
		cout<<"Graph | "<<m_s_InputFile<<" | Average Vertex Degree | "<<m_d_AverageVertexDegree<<endl;
		cout<<endl;

		return;
	}
	
	int GraphInputOutput::BuildGraph_ADOLC(unsigned int ** uip2_HessianSparsityPattern, int i_RowCount) {
	  int i, j;
	  
	  int i_ElementCount, i_PositionCount;
	  
	  int i_HighestDegree;
	  
#if DEBUG == 1
	  
	  cout<<endl;
	  cout<<"DEBUG | Graph Coloring | Sparsity Pattern"<<endl;
	  cout<<endl;
	  
	  for(i=0; i<i_RowCount; i++)
	    {
	      cout<<i<<"\t"<<" : ";
	      
	      i_PositionCount = uip2_HessianSparsityPattern[i][0];
	      
	      for(j=0; j<i_PositionCount; j++)
		{
		  if(j == STEP_DOWN(i_PositionCount))
		    {
		      cout<<uip2_HessianSparsityPattern[i][STEP_UP(j)]<<" ("<<i_PositionCount<<")";
		    }
		  else
		    {
		      cout<<uip2_HessianSparsityPattern[i][STEP_UP(j)]<<", ";
		    }
		  
		}
	      
	      cout<<endl;
	    }
	  
	  cout<<endl;
	  
#endif
	  
	  m_vi_Vertices.clear();
	  m_vi_Vertices.push_back(_FALSE);
	  
	  m_vi_Edges.clear();
	  
	  i_HighestDegree = _UNKNOWN;
	  
	  for(i=0; i<i_RowCount; i++)
	    {
	      i_ElementCount = _FALSE;
	      
	      i_PositionCount = uip2_HessianSparsityPattern[i][0];
	      
	      if(i_HighestDegree < i_PositionCount)
		{
		  i_HighestDegree = i_PositionCount;
		}
	      
	      for(j=0; j<i_PositionCount; j++)
		{
		  if((signed) uip2_HessianSparsityPattern[i][STEP_UP(j)] != i)
		    {
		      m_vi_Edges.push_back((signed) uip2_HessianSparsityPattern[i][STEP_UP(j)]);
		      
		      i_ElementCount++;
		    }
		  
		}
	      
	      m_vi_Vertices.push_back(m_vi_Vertices.back() + i_ElementCount);
	    }
	  
#if DEBUG == 1
	  
	  int i_VertexCount, i_EdgeCount;
	  
	  cout<<endl;
	  cout<<"DEBUG | Graph Coloring | Graph Format"<<endl;
	  cout<<endl;
	  
	  cout<<"Vertices"<<"\t"<<" : ";
	  
	  i_VertexCount = (signed) m_vi_Vertices.size();
	  
	  for(i=0; i<i_VertexCount; i++)
	    {
	      if(i == STEP_DOWN(i_VertexCount))
		{
		  cout<<m_vi_Vertices[i]<<" ("<<i_VertexCount<<")";
		}
	      else
		{
		  cout<<m_vi_Vertices[i]<<", ";
		}
	    }
	  
	  cout<<endl;
	  
	  cout<<"Edges"<<"\t"<<" : ";
	  
	  i_EdgeCount = (signed) m_vi_Edges.size();
	  
	  for(i=0; i<i_EdgeCount; i++)
	    {
	      if(i == STEP_DOWN(i_EdgeCount))
		{
		  cout<<m_vi_Edges[i]<<" ("<<i_EdgeCount<<")";
		}
	      else
		{
		  cout<<m_vi_Edges[i]<<", ";
		}
	    }
	  
	  cout<<endl;
	  
#endif
	  
	  CalculateVertexDegrees();

	  return(i_HighestDegree);
	}
	
	int GraphInputOutput::ReadAdjacencyGraph(string s_InputFile, string s_fileFormat)
	{
		if (s_fileFormat == "AUTO_DETECTED" || s_fileFormat == "") {
			File file(s_InputFile);
			string fileExtension = file.GetFileExtension();
			if (isHarwellBoeingFormat(fileExtension)) {
				cout<<"ReadHarwellBoeingAdjacencyGraph"<<endl;
				return ReadHarwellBoeingAdjacencyGraph(s_InputFile);
			}
			else if (isMeTiSFormat(fileExtension)) {
				cout<<"ReadMeTiSAdjacencyGraph"<<endl;
				return ReadMeTiSAdjacencyGraph(s_InputFile);
			}
			else if (isMatrixMarketFormat(fileExtension)) {
				cout<<"ReadMatrixMarketAdjacencyGraph"<<endl;
				return ReadMatrixMarketAdjacencyGraph(s_InputFile);
			}
			else { //other extensions
				cout<<"unfamiliar extension \""<<fileExtension<<"\", use ReadMatrixMarketAdjacencyGraph"<<endl;
				return ReadMatrixMarketAdjacencyGraph(s_InputFile);
			}
		}
		else if (s_fileFormat == "MM") {
			cout<<"ReadMatrixMarketAdjacencyGraph"<<endl;
			return ReadMatrixMarketAdjacencyGraph(s_InputFile);
		}
		else if (s_fileFormat == "MM2") {
			cout<<"ReadMatrixMarketAdjacencyGraph2"<<endl;
			return ReadMatrixMarketAdjacencyGraph2(s_InputFile);
		}
		else if (s_fileFormat == "HB") {
			cout<<"ReadHarwellBoeingAdjacencyGraph"<<endl;
			return ReadHarwellBoeingAdjacencyGraph(s_InputFile);
		}
		else if (s_fileFormat == "MeTiS") {
			cout<<"ReadMeTiSAdjacencyGraph"<<endl;
			return ReadMeTiSAdjacencyGraph(s_InputFile);
		}
		else {
			cerr<<"GraphInputOutput::ReadAdjacencyGraph s_fileFormat is not recognized"<<endl;
			return _FALSE;
		}

		return(_TRUE);
	}
	
}
