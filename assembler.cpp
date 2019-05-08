#include <bits/stdc++.h>
using namespace std;
#define checkBit(n,p) (bool)(n&(1<<p))
map<string,string> getMap(string path)//for mapping the register/instruction to the corresponding opcode
{
    string word,value;
    map<string,string> m;
    ifstream file(path);
    while(file>>word>>value)
    {
        m[word] = value;
    }
    file.close();
    return m;
}
string decToBin(int num,int length)
{
    string result = "";
    for(int i=0;i<length;i++)
    {
        if(checkBit(num,i))
        {
            result = "1" + result;
        }
        else
        {
            result = "0" + result;
        }
    }
    return result;
}
int main()
{
    cout << "MACHINE CODE:" << endl;
    int val;
    string ins,op,r1,r2,r3,cmnt;
    map<string,string> R_TYPE,I_TYPE,J_TYPE,RegisterValue;
    R_TYPE = getMap("op_code_files/R_TYPE.txt");
    I_TYPE = getMap("op_code_files/I_TYPE.txt");
    J_TYPE = getMap("op_code_files/J_TYPE.txt");
    RegisterValue = getMap("op_code_files/RegisterValue.txt");
    ifstream inputFile("io_files/input.txt");
    ofstream outputFile("io_files/output.txt");
    //ifstream inputFile("sample_test_cases/invalidtestcases.txt");
    //ifstream inputFile("sample_test_cases/validtestcases.txt");
    getline(inputFile,ins);
     if(ins!="main:")
        {
            cout<<"Error: main is missing, cannot proceed further"<<endl;
            outputFile<<"Error: main is missing, cannot proceed further"<<endl;
            return 0;
        }
    while(getline(inputFile,ins))
    {
        stringstream ss;
        cout << ins << endl;//printing the given assembly code line by line(optional)
        //Checking if instruction is empty or not

        if(ins.size()==0)
        {
            cout << "Error: No instruction given" << endl;
            outputFile << "Error: No instruction given" << endl;
            continue;
        }
        ss << ins;
        ss >> op;
        //If operation code is J Type then the program will go in here
        if(J_TYPE.find(op)!=J_TYPE.end())
        {
            val = -1;
            ss >> val;    
            if(val<0)
            {
                cout << "Error: value can not be negative" << endl;
                outputFile << "Error: value can not be negative" << endl;
            }
            else if(val>67108869)
            {
                cout << "Error: value is too large" << endl;
                outputFile << "invalid: value is too large" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='#'))
            {
            	cout << "Error: comment structure not maintained" << endl;
            	outputFile << "Error: comment structure not maintained" << endl;
            }
            //Printing machine code
            else
            {
                cout << J_TYPE[op] << " " << decToBin(val,26) << endl;
                outputFile << J_TYPE[op] << " " << decToBin(val,26) << endl;
            }
            continue;
        }
        ss >> r1;
        if(op!="jr")
        {
            if(r1[0]!='$')
            {
                cout << "Error: '$' sign is missing before rd" << endl;
                outputFile << "Error: '$' sign is missing before rd" << endl;
                continue;
            }
           //comma after the first register
            else if(r1[r1.size()-1]!=',')
            {
                cout << "Error: ',' is missing after rd" << endl;
                outputFile << "Error: ',' is missing after rd" << endl;
                continue;
            }

            r1.erase(r1.end()-1);
        }
        //If operation code is R Type then the program will go in here
        if(R_TYPE.find(op)!=R_TYPE.end())
        {
            val = 0;
            if(op=="jr")
            {
                r2 = r3 = "$zero";
                goto print;
            }
            if(op=="sll"||op=="srl"||op=="sqrt")
            {
                r2 = "$zero";
            }
            else
            {
                ss >> r2;
                if(r2[0]!='$')
                {
                    cout << "Error: '$' sign is missing before rs" << endl;
                    outputFile << "Error: '$' sign is missing before rs" << endl;
                    continue;
                }
                else if(r2[r2.size()-1]!=',')
                {
                    cout << "Error: ',' is missing after rs" << endl;
                    outputFile << "Error: ',' is missing after rs" << endl;
                    continue;
                }
                r2.erase(r2.end()-1);
            }
            ss >> r3;
            if(op=="log")
            {
            	if(r3=="$zero"||r2=="$zero")
            	{
            		cout << "Error: For Logarithm either of the inputs cannot be zero" << endl;
                	outputFile << "Error: For Logarithm either of the inputs cannot be zero" << endl;
                	continue;		
            	}
            }
            if(r3[0]!='$')
            {
                cout << "Error: '$' sign is missing before rt" << endl;
                outputFile << "Error: '$' sign is missing before rt" << endl;
                continue;
            }
            if(op=="sll"||op=="srl"||op=="sqrt")
            {
                if(r3[r3.size()-1]!=',')
                {
                    cout << "Error: ',' is missing after rt" << endl;
                    outputFile << "Error: ',' is missing after rt" << endl;
                    continue;
                }
                r3.erase(r3.end()-1);
                val = -1;
                ss >> val;
            }
            //Checking instruction validities
            print:
            if(r1=="$zero")
            {
                cout << "Error: $zero can not be used" << endl;
                outputFile << "Error: $zero can not be used" << endl;
            }
            else if(RegisterValue.find(r1)==RegisterValue.end())
            {
                cout << "Error: rd not found" << endl;
                outputFile << "Error: rd not found" << endl;
            }
            else if(RegisterValue.find(r2)==RegisterValue.end())
            {
                cout << "Error: rs not found" << endl;
                outputFile << "Error: rs not found" << endl;
            }
            else if(RegisterValue.find(r3)==RegisterValue.end())
            {
                cout << "Error: rt not found" << endl;
                outputFile << "Error: rt not found" << endl;
            }
            else if(val<0)
            {
                cout << "Error: value can not be negative" << endl;
                outputFile << "Error: value can not be negative" << endl;
            }
            else if(val>31)
            {
                cout << "Error: value is too large" << endl;
                outputFile << "Error: value is too large" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='#'))
            {
            	cout << "Error: comment structure not maintained" << endl;
            	outputFile << "Error: comment structure not maintained" << endl;
            }
            //Printing machine code if instruction is valid
            else
            {
                cout << "000000" << " " << RegisterValue[r2] << " " << RegisterValue[r3] << " " << RegisterValue[r1] << " " << decToBin(val,5) << " " << R_TYPE[op] << endl;
                outputFile << "000000" << " " << RegisterValue[r2] << " " << RegisterValue[r3] << " " << RegisterValue[r1] << " " << decToBin(val,5) << " " << R_TYPE[op] << endl;
            }
        }
        //If operation code is I Type then the program will go in here
        else if(I_TYPE.find(op)!=I_TYPE.end())
        {
        	val = -1;
        	if(op=="lw"||op=="sw")
        	{
        		ss >> val >> r2;
        		if(r2[0]!='(')
                {
                    cout << "Error: '(' is missing before rs" << endl;
                    outputFile << "Error: '(' is missing before rs" << endl;
                    continue;
                }
                else if(r2[r2.size()-1]!=')')
                {
                    cout << "Error: ')' is missing after rs" << endl;
                    outputFile << "Error: ')' is missing after rs" << endl;
                    continue;
                }
                else if(r2[1]!='$')
                {
                    cout << "Error: '$' sign is missing before rs" << endl;
                    outputFile << "Error: '$' sign is missing before rs" << endl;
                    continue;
                }
        		r2.erase(r2.begin());
        		r2.erase(r2.end()-1);
        		swap(r1,r2);
        		if(r1=="$zero")
                {
                    r1 = "";
                    cout << "Error: $zero can to be used" << endl;
                    outputFile << "Error: $zero can to be used" << endl;
                    continue;
                }
                else if(r2=="$zero")
                {
                    r2 = "";
                    cout << "Error: $zero can to be used" << endl;
                    outputFile << "Error: $zero can to be used" << endl;
                    continue;
                }
        	}
        	else
        	{
        		ss >> r2 >> val;
        		if(r2[0]!='$')
                {
                    cout << "Error: '$' sign is missing before rs" << endl;
                    outputFile << "Error: '$' sign is missing before rs" << endl;
                    continue;
                }
                else if(r2[r2.size()-1]!=',')
                {
                    cout << "Error: ',' is missing after rs" << endl;
                    outputFile << "Error: ',' is missing after rs" << endl;
                    continue;
                }
        		r2.erase(r2.end()-1);
        		if(op=="addi"||op=="andi"||op=="ori")
        		{
        		    swap(r1,r2);
        		    if(r1=="$zero")
                    {
                        r1 = "";
                        cout << "Error: $zero cannot be used" << endl;
                        outputFile << "Error: $zero cannot be used" << endl;
                        continue;
                    }
        		}
        		else
        		{
        		    if(val<0)
                    {
                        val -= 10;
                    }
        			val = ceil(val/4.0);
        		}
        	}
        	//Checking instruction validities
            if(RegisterValue.find(r1)==RegisterValue.end())
            {
                cout << "Error: rd not found" << endl;
                outputFile << "Error: rd not found" << endl;
            }
            else if(RegisterValue.find(r2)==RegisterValue.end())
            {
                cout << "Error: rs not found" << endl;
                outputFile << "Error: rs not found" << endl;
            }
            else if(op!="addi"&&op!="andi"&&op!="ori"&&val<0)
            {
                cout << "Error: value can not be neg" << endl;
                outputFile << "Error: value can not be neg" << endl;
            }
            else if(val>32767)
            {
                cout << "Error: value is too large" << endl;
                outputFile << "Error: value is too large" << endl;
            }
            else if(val<-32768)
            {
                cout << "Error: value is too small" << endl;
                outputFile << "Error: value is too small" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='#'))
            {
            	cout << "Error: comment structure not maintained" << endl;
            	outputFile << "Error: comment structure not maintained" << endl;
            }
            //Printing machine code if instruction is valid
            else
            {
            	cout << I_TYPE[op] << " " << RegisterValue[r1] << " " << RegisterValue[r2] << " " << decToBin(val,16) << endl;
            	outputFile << I_TYPE[op] << " " << RegisterValue[r1] << " " << RegisterValue[r2] << " " << decToBin(val,16) << endl;
            }
        }
        else
        {
        	cout << "Error: Op code not found" << endl;
        	outputFile << "Error: Op code not found" << endl;
        }
    }
    inputFile.close();
}
