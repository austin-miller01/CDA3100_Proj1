#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>


//convert hex to binary
std::string hexToBinary(std::string hex)
{
	std::string binary;
	for (int i = 0; i < hex.length(); i++)
	{
		switch (hex[i])
		{
		case '0':
			binary += "0000";
			break;
		case '1':
			binary += "0001";
			break;
		case '2':
			binary += "0010";
			break;
		case '3':
			binary += "0011";
			break;
		case '4':
			binary += "0100";
			break;
		case '5':
			binary += "0101";
			break;
		case '6':
			binary += "0110";
			break;
		case '7':
			binary += "0111";
			break;
		case '8':
			binary += "1000";
			break;
		case '9':
			binary += "1001";
			break;
		case 'A':
			binary += "1010";
			break;
		case 'B':
			binary += "1011";
			break;
		case 'C':
			binary += "1100";
			break;
		case 'D':
			binary += "1101";
			break;
		case 'E':
			binary += "1110";
			break;
		case 'F':
			binary += "1111";
			break;
		}
	}

	if(binary.length() != 32){
		while(binary.length() != 32){
			binary = "0" + binary;
		}
	}
	return binary;
}

int main(int argc, char * argv[]){
	
	std::ifstream myfile;
	myfile.open(argv[1]);
	std::vector<std::string> lines;
	
	std::string inLine;
	std::string binary = "";

	while(myfile.is_open()){

		while(std::getline(myfile,inLine)){
			binary = hexToBinary(inLine);
			lines.push_back(binary);
		}
		
		myfile.close();
	}

	for(int i = 0; i < lines.size(); i++){
		for(int j = 0;j < lines[i].length(); j++){
			if(j%4 == 0 && j != 0){
				std::cout << " ";
			}
			std::cout << lines[i][j];
		}
		std::cout << std::endl;
	}
	
	return 0;

}
